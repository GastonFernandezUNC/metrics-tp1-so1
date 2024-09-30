/**
 * @file metrics.h
 * @brief Funciones para obtener el uso de CPU y memoria desde el sistema de archivos /proc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

typedef struct{
    unsigned long long memTotal;
    unsigned long long memUsed;
    unsigned long long memAvailable;
    unsigned long long percentage;
} memInfo;

typedef struct{
    unsigned long long reads_completed_successfully;
    unsigned long long writes_completed;
    unsigned long long reads_per_second;
    unsigned long long writes_per_second;
} diskStats;

typedef struct{
    unsigned long long rx_bytes;
    unsigned long long tx_bytes;
    unsigned long long rx_packets;
    unsigned long long tx_packets;
    unsigned long long rx_errors;
    unsigned long long tx_errors;
}netStats;

typedef struct{

    unsigned long long running_processes;
    unsigned long long context_switching;

}proc_stats;

static proc_stats stat;
static memInfo mem;
static diskStats disk;
static netStats net;

proc_stats* get_procStats_usage();

netStats* get_net_usage();

diskStats* get_disk_usage();

/**
 * @brief Obtiene el porcentaje de uso de memoria desde /proc/meminfo.
 *
 * Lee los valores de memoria total y disponible desde /proc/meminfo y calcula
 * el porcentaje de uso de memoria.
 *
 * @return Uso de memoria como porcentaje (0.0 a 100.0), o -1.0 en caso de error.
 */
memInfo* get_memory_usage();

/**
 * @brief Obtiene el porcentaje de uso de CPU desde /proc/stat.
 *
 * Lee los tiempos de CPU desde /proc/stat y calcula el porcentaje de uso de CPU
 * en un intervalo de tiempo.
 *
 * @return Uso de CPU como porcentaje (0.0 a 100.0), o -1.0 en caso de error.
 */
double get_cpu_usage();
