#include "metrics.h"

#define SLEEP_TIME 1.5

netStats* get_net_usage(){
    FILE* fp;
    char buffer[BUFFER_SIZE];
    char device_name[32];
    unsigned long long rx_bytes = 0, rx_packets = 0, rx_errors = 0, tx_bytes = 0, tx_packets = 0, tx_errors = 0;
    unsigned long long trash;

    // Abrir el archivo /proc/diskstats
    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/net/dev");
        return NULL;
    }

    // Leer los valores de estadísticas de disco
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "%31s %llu %llu %llu %*u %*u %*u %*u %*u %llu %llu %llu", 
            device_name, &rx_bytes, &rx_packets, &rx_errors, &tx_bytes, &tx_packets, &tx_errors) == 7){
                if (strcmp(device_name, "wlp2s0:") == 0) break; // Exit the loop if the desired device is found
        }
        
    }
    sleep(SLEEP_TIME);
    // rewind(fp);

    fclose(fp);

    net.rx_bytes = rx_bytes;
    net.rx_packets = rx_packets;
    net.rx_errors = rx_errors;
    net.tx_bytes = tx_bytes;
    net.tx_packets = tx_packets;
    net.tx_errors = tx_errors;
    
    return &net;
}

diskStats* get_disk_usage()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    char device_name[32];
    unsigned long long reads_completed_successfully[2], writes_completed[2];
    unsigned long long reads_per_second = 0, writes_per_second = 0;
    unsigned long long trash;

    // Abrir el archivo /proc/diskstats
    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/diskstats");
        return NULL;
    }

    // Leer los valores de estadísticas de disco
    for(int i = 0; i < 2 ; i ++){
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            if (sscanf(buffer, "%llu %llu %31s %llu %llu %llu %llu %llu", 
                &trash, &trash, device_name, &reads_completed_successfully[i], &trash,
                &trash, &trash, &writes_completed[i]) == 8){
                    if(strcmp(device_name, "sda") == 0) break; // gets out of the loop
                }
        }
        sleep(SLEEP_TIME);
        rewind(fp);
    }
    fclose(fp);

    disk.reads_completed_successfully = reads_completed_successfully[1];
    disk.writes_completed = writes_completed[1];
    disk.reads_per_second = (reads_completed_successfully[1]-reads_completed_successfully[0]/SLEEP_TIME);
    disk.writes_per_second = (writes_completed[1]-writes_completed[0])/SLEEP_TIME;

    return &disk;
}

memInfo* get_memory_usage()
{
    FILE* fp;
    char buffer[BUFFER_SIZE];
    unsigned long long total_mem = 0, free_mem = 0;

    // Abrir el archivo /proc/meminfo
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/meminfo");
        return NULL;
    }

    // Leer los valores de memoria total y disponible
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (sscanf(buffer, "MemTotal: %llu kB", &total_mem) == 1)
        {
            continue; // MemTotal encontrado
        }
        if (sscanf(buffer, "MemAvailable: %llu kB", &free_mem) == 1)
        {
            break; // MemAvailable encontrado, podemos dejar de leer
        }
    }

    fclose(fp);

    // Verificar si se encontraron ambos valores
    if (total_mem == 0 || free_mem == 0)
    {
        fprintf(stderr, "Error al leer la información de memoria desde /proc/meminfo\n");
        return NULL;
    }

    // Calcular el porcentaje de uso de memoria
    double used_mem = total_mem - free_mem;
    double mem_usage_percent = (used_mem / total_mem) * 100.0;

    mem.memAvailable = free_mem/1000; // en MB
    mem.memUsed = used_mem/1000;      // en MB
    mem.memTotal = total_mem/1000;    // en MB
    mem.percentage = mem_usage_percent;

    return &mem;
}

double get_cpu_usage()
{
    static unsigned long long prev_user = 0, prev_nice = 0, prev_system = 0, prev_idle = 0, prev_iowait = 0,
                              prev_irq = 0, prev_softirq = 0, prev_steal = 0;
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    unsigned long long totald, idled;
    double cpu_usage_percent;

    // Abrir el archivo /proc/stat
    FILE* fp = fopen("/proc/stat", "r");
    if (fp == NULL)
    {
        perror("Error al abrir /proc/stat");
        return -1.0;
    }

    char buffer[BUFFER_SIZE * 4];
    if (fgets(buffer, sizeof(buffer), fp) == NULL)
    {
        perror("Error al leer /proc/stat");
        fclose(fp);
        return -1.0;
    }
    fclose(fp);

    // Analizar los valores de tiempo de CPU
    int ret = sscanf(buffer, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait,
                     &irq, &softirq, &steal);
    if (ret < 8)
    {
        fprintf(stderr, "Error al parsear /proc/stat\n");
        return -1.0;
    }

    // Calcular las diferencias entre las lecturas actuales y anteriores
    unsigned long long prev_idle_total = prev_idle + prev_iowait;
    unsigned long long idle_total = idle + iowait;

    unsigned long long prev_non_idle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
    unsigned long long non_idle = user + nice + system + irq + softirq + steal;

    unsigned long long prev_total = prev_idle_total + prev_non_idle;
    unsigned long long total = idle_total + non_idle;

    totald = total - prev_total;
    idled = idle_total - prev_idle_total;

    if (totald == 0)
    {
        fprintf(stderr, "Totald es cero, no se puede calcular el uso de CPU!\n");
        return -1.0;
    }

    // Calcular el porcentaje de uso de CPU
    cpu_usage_percent = ((double)(totald - idled) / totald) * 100.0;

    // Actualizar los valores anteriores para la siguiente lectura
    prev_user = user;
    prev_nice = nice;
    prev_system = system;
    prev_idle = idle;
    prev_iowait = iowait;
    prev_irq = irq;
    prev_softirq = softirq;
    prev_steal = steal;

    return cpu_usage_percent;
}
