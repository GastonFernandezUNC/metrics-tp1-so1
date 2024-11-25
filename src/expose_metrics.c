#include "expose_metrics.h"

/** Mutex para sincronización de hilos */
pthread_mutex_t lock, fifo_lock;

/** Métrica de Prometheus para el uso de CPU */
static prom_gauge_t* cpu_usage_metric;

/**
 * @brief Struct to save the mem metrics.
 */
typedef struct
{
    /**
     * @brief memory_percentage_metric
     */
    prom_gauge_t* memory_percentage_metric;

    /**
     * @brief memory_total_metric
     */
    prom_gauge_t* memory_total_metric;

    /**
     * @brief memory_available_metric
     */
    prom_gauge_t* memory_available_metric;

    /**
     * @brief memory_used_metric
     */
    prom_gauge_t* memory_used_metric;

} expose_mem_metrics;

/**
 * @brief Struct to save the disk metrics.
 */
typedef struct
{
    /**
     * @brief reads_completed_successfully_metric
     */
    prom_gauge_t* reads_completed_successfully_metric;

    /**
     * @brief writes_completed_metric
     */
    prom_gauge_t* writes_completed_metric;

    /**
     * @brief reads_per_second_metric
     */
    prom_gauge_t* reads_per_second_metric;

    /**
     * @brief writes_per_second_metric
     */
    prom_gauge_t* writes_per_second_metric;

} expose_disk_metrics;

/**
 * @brief Struct to save the network statistics.
 */
typedef struct
{
    /**
     * @brief rx_bytes_metric
     */
    prom_gauge_t* rx_bytes_metric;

    /**
     * @brief tx_bytes_metric
     */
    prom_gauge_t* tx_bytes_metric;

    /**
     * @brief rx_packets_metric
     */
    prom_gauge_t* rx_packets_metric;

    /**
     * @brief tx_packets_metric
     */
    prom_gauge_t* tx_packets_metric;

    /**
     * @brief rx_errors_metric
     */
    prom_gauge_t* rx_errors_metric;

    /**
     * @brief tx_errors_metric
     */
    prom_gauge_t* tx_errors_metric;

} expose_net_metrics;

/**
 * @brief Struct to save the processes statistics.
 */
typedef struct
{
    /**
     * @brief running_processes_metrics
     */
    prom_gauge_t* running_processes_metrics;

    /**
     * @brief context_switching_metrics
     */
    prom_gauge_t* context_switching_metrics;

} expose_procStats_metrics;

// Estructuras para almacenar las métricas de procesos
static expose_procStats_metrics statsMetrics;

// Estructuras para almacenar las métricas de red
static expose_net_metrics netMetrics;

// Estructuras para almacenar las métricas de disco
static expose_disk_metrics diskMetrics;

// Estructuras para almacenar las métricas de memoria
static expose_mem_metrics memMetrics;

// Funciones para actualizar los valores de las métricas de CPU
void update_cpu_gauge()
{
    double usage = get_cpu_usage();
    if (usage >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(cpu_usage_metric, usage, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el uso de CPU\n");
    }
}

// Funciones para actualizar los valores de las métricas de memoria
void update_memory_gauge()
{
    memInfo mem = *get_memory_usage();

    pthread_mutex_lock(&lock);
    prom_gauge_set(memMetrics.memory_percentage_metric, mem.percentage, NULL);
    prom_gauge_set(memMetrics.memory_available_metric, mem.memAvailable, NULL);
    prom_gauge_set(memMetrics.memory_used_metric, mem.memUsed, NULL);
    prom_gauge_set(memMetrics.memory_total_metric, mem.memTotal, NULL);
    pthread_mutex_unlock(&lock);
}

// Funciones para actualizar los valores de las métricas de disco
void update_disk_gauge()
{
    diskStats disk = *get_disk_usage();

    pthread_mutex_lock(&lock);
    prom_gauge_set(diskMetrics.reads_completed_successfully_metric, disk.reads_completed_successfully, NULL);
    prom_gauge_set(diskMetrics.writes_completed_metric, disk.writes_completed, NULL);
    prom_gauge_set(diskMetrics.reads_per_second_metric, disk.reads_per_second, NULL);
    prom_gauge_set(diskMetrics.writes_per_second_metric, disk.writes_per_second, NULL);
    pthread_mutex_unlock(&lock);
}

// Funciones para actualizar los valores de las métricas de red
void update_net_gauge()
{
    netStats net = *get_net_usage();

    pthread_mutex_lock(&lock);
    prom_gauge_set(netMetrics.rx_bytes_metric, net.rx_bytes, NULL);
    prom_gauge_set(netMetrics.tx_bytes_metric, net.tx_bytes, NULL);
    prom_gauge_set(netMetrics.rx_packets_metric, net.rx_packets, NULL);
    prom_gauge_set(netMetrics.tx_packets_metric, net.tx_packets, NULL);
    prom_gauge_set(netMetrics.rx_errors_metric, net.rx_errors, NULL);
    prom_gauge_set(netMetrics.tx_errors_metric, net.tx_errors, NULL);
    pthread_mutex_unlock(&lock);
}

// Funciones para actualizar los valores de las métricas de procesos
void update_procStats_gauge()
{

    proc_stats _stat = *get_procStats_usage();

    pthread_mutex_lock(&lock);
    prom_gauge_set(statsMetrics.context_switching_metrics, _stat.context_switching, NULL);
    prom_gauge_set(statsMetrics.running_processes_metrics, _stat.running_processes, NULL);
    pthread_mutex_unlock(&lock);
}

// Función del hilo para exponer las métricas vía HTTP en el puerto 8000
void* expose_metrics(void* arg)
{
    (void)arg; // Argumento no utilizado

    // Aseguramos que el manejador HTTP esté adjunto al registro por defecto
    promhttp_set_active_collector_registry(NULL);

    // Iniciamos el servidor HTTP en el puerto 8000
    struct MHD_Daemon* daemon = promhttp_start_daemon(MHD_USE_SELECT_INTERNALLY, 8000, NULL, NULL);
    if (daemon == NULL)
    {
        fprintf(stderr, "Error al iniciar el servidor HTTP\n");
        return NULL;
    }

    // Mantenemos el servidor en ejecución
    while (1)
    {
        sleep(1);
    }

    // Nunca debería llegar aquí
    MHD_stop_daemon(daemon);
    return NULL;
}

// Iniciamos cada una de las metricas
void _init_memory_metrics()
{

    memMetrics.memory_percentage_metric =
        prom_gauge_new("memory_usage_percentage_in_MB", "Porcentaje de uso de memoria_en_MB", 0, NULL);
    memMetrics.memory_available_metric = prom_gauge_new("available_memory_in_MB", "Memoria Disponible_en_MB", 0, NULL);
    memMetrics.memory_used_metric = prom_gauge_new("currently_used_memory_in_MB", "Memoria utilizada_en_MB", 0, NULL);
    memMetrics.memory_total_metric =
        prom_gauge_new("Total_memory_amount_in_MB", "Cantidad Total de Memoria_en_MB", 0, NULL);

    prom_collector_registry_must_register_metric(memMetrics.memory_percentage_metric);
    prom_collector_registry_must_register_metric(memMetrics.memory_available_metric);
    prom_collector_registry_must_register_metric(memMetrics.memory_used_metric);
    prom_collector_registry_must_register_metric(memMetrics.memory_total_metric);
}

/* initialices cpu metrics */
void _init_cpu_metrics()
{

    cpu_usage_metric = prom_gauge_new("cpu_usage_percentage", "Porcentaje de uso de CPU", 0, NULL);
    prom_collector_registry_must_register_metric(cpu_usage_metric);
}

/* initialices disk metrics */
void _init_disk_metrics()
{
    diskMetrics.reads_completed_successfully_metric =
        prom_gauge_new("reads_completed_successfully", "Lecturas Completadas Exitosamente", 0, NULL);
    diskMetrics.writes_completed_metric = prom_gauge_new("writes_completed", "Escrituras Completadas", 0, NULL);
    diskMetrics.reads_per_second_metric = prom_gauge_new("reads_per_second", "Lecturas por segundo", 0, NULL);
    diskMetrics.writes_per_second_metric = prom_gauge_new("writes_per_second", "Escrituras por segundo", 0, NULL);

    prom_collector_registry_must_register_metric(diskMetrics.reads_completed_successfully_metric);
    prom_collector_registry_must_register_metric(diskMetrics.writes_completed_metric);
    prom_collector_registry_must_register_metric(diskMetrics.reads_per_second_metric);
    prom_collector_registry_must_register_metric(diskMetrics.writes_per_second_metric);
}

/* initialices net metrics */
void _init_net_metrics()
{

    netMetrics.rx_bytes_metric = prom_gauge_new("rx_bytes", "Bytes Recibidos", 0, NULL);
    netMetrics.tx_bytes_metric = prom_gauge_new("tx_bytes", "Bytes Transmitidos", 0, NULL);
    netMetrics.rx_packets_metric = prom_gauge_new("rx_packets", "Paquetes Recibidos", 0, NULL);
    netMetrics.tx_packets_metric = prom_gauge_new("tx_packets", "Paquetes Transmitidos", 0, NULL);
    netMetrics.rx_errors_metric = prom_gauge_new("rx_errors", "Errores de Recepcion", 0, NULL);
    netMetrics.tx_errors_metric = prom_gauge_new("tx_errors", "Errores de Transmision", 0, NULL);

    prom_collector_registry_must_register_metric(netMetrics.rx_bytes_metric);
    prom_collector_registry_must_register_metric(netMetrics.tx_bytes_metric);
    prom_collector_registry_must_register_metric(netMetrics.rx_packets_metric);
    prom_collector_registry_must_register_metric(netMetrics.tx_packets_metric);
    prom_collector_registry_must_register_metric(netMetrics.rx_errors_metric);
    prom_collector_registry_must_register_metric(netMetrics.tx_errors_metric);
}

/* initialices procStats metrics */
void _init_procStats_metrics()
{

    statsMetrics.context_switching_metrics = prom_gauge_new("context_switching", "Cambios de Contexto", 0, NULL);
    statsMetrics.running_processes_metrics = prom_gauge_new("running_processes", "Procesos en ejecucion", 0, NULL);

    prom_collector_registry_must_register_metric(statsMetrics.context_switching_metrics);
    prom_collector_registry_must_register_metric(statsMetrics.running_processes_metrics);
}

void _write_fifo(json_handler* json_struct){


    pthread_mutex_lock(&fifo_lock);
    
    _get_json(json_struct);
    read_json_content(json_struct);

    int fd;
    ssize_t bytesWritten;
    char buffer[FIFO_BUFFER_SIZE];

    // char m_cpu  [BUFFER_SIZE];
    char m_mem  [BUFFER_SIZE];
    // char m_disk [BUFFER_SIZE];
    // char m_net  [BUFFER_SIZE];
    // char m_proc [BUFFER_SIZE];
    // double cpu = 0;
    memInfo*    mem = NULL;
    // diskStats*  disk = NULL;
    // netStats*   net = NULL;
    // proc_stats* proc = NULL;

    // // if(json_struct->cpu == true){
    // //     cpu = get_cpu_usage();
    // //     m_cpu = cpu;
    // // }

    if(json_struct->mem == true){
        mem = get_memory_usage();
        memInfo_toString(mem, m_mem, sizeof(m_mem));
    }

    // if(json_struct->disk == true){
    //     disk = get_disk_usage();
    //     m_disk = diskStats_toString(disk);
    // }

    // if(json_struct->net == true){
    //     net = get_net_usage();
    //     m_net = netStats_toString(net);
    // }

    // if(json_struct->proc == true){
    //     proc = get_procStats_usage();
    //     m_proc = procStats_toString(proc);
    // }

    fd = open(PATH_TO_FIFO, O_WRONLY);
    if (fd == -1)
    {
        perror("Could not open the FIFO");        
    }
    
    // if(m_cpu != ""){
    //     // bytesWritten = write(fd, m_cpu, strlen(m_cpu));
    //     printf("\n%s",m_cpu);
    //     if (bytesWritten == -1)
    //     {
    //         perror("write");
    //     }
    // }
    if(m_mem != NULL){
        // bytesWritten = write(fd, m_mem, strlen(m_mem));
        printf("\n%s",m_mem);
        if (bytesWritten == -1)
        {
            perror("write");
        }
    }
    // if(m_disk != ""){
    //     bytesWritten = write(fd, m_disk, strlen(m_disk));
    //     if (bytesWritten == -1)
    //     {
    //         perror("write");
    //     }
    // }
    // if(m_net != ""){
    //     // bytesWritten = write(fd, m_net, strlen(m_net));
    //     printf("\n%s",m_net);
    //     if (bytesWritten == -1)
    //     {
    //         perror("write");
    //     }
    // }
    // if(m_proc != ""){
    //     // bytesWritten = write(fd, m_proc, strlen(m_proc));
    //     printf("\n%s",m_proc);
    //     if (bytesWritten == -1)
    //     {
    //         perror("write");
    //     }
    // }
        
    char *end = "status_stop";
    bytesWritten = write(fd, end, strlen(end));
    printf("\n\n%s\n\n",end);
    if (bytesWritten == -1)
    {
        perror("write");
    }

    char *clean_message = "clean\n";
    bytesWritten = write(fd, clean_message, strlen(clean_message));
    printf("\n\n%s\n\n",clean_message);
    if (bytesWritten == -1)
    {
        perror("write");
    }

    // printf("escribiendo en fifo");
    close(fd);
    //free(m_mem); //free(m_cpu);  free(m_disk); free(m_net); free(m_proc);
    pthread_mutex_unlock(&fifo_lock);
}

void _read_fifo(json_handler* json_struct){

    char buffer[FIFO_BUFFER_SIZE];
    int fd;
    ssize_t bytesRead;
    fd = open(PATH_TO_FIFO, O_RDONLY);
    while(fd == -1)
    {   
        mkfifo(PATH_TO_FIFO, FILE_PERMISSIONS);
        return _read_fifo(json_struct);
    }

    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0'; // Asegurarse de terminar la cadena
        // Procesar el mensaje si coincide con "status"
        if (strncasecmp(buffer, "status", strlen("status")) == 0)
        {
            close(fd);
            _write_fifo(json_struct);
            fd = open(PATH_TO_FIFO, O_RDONLY);

        }
    }
}


// Inicializamos el mutex y las métricas
void init_metrics()
{
    // Inicializamos el mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        fprintf(stderr, "Error al inicializar el mutex\n");
        return (void)EXIT_FAILURE;
    }

    // Inicializamos el registro de coleccionistas de Prometheus
    if (prom_collector_registry_default_init() != 0)
    {
        fprintf(stderr, "Error al inicializar el registro de Prometheus\n");
        return (void)EXIT_FAILURE;
    }

    _init_memory_metrics();
    _init_cpu_metrics();
    _init_disk_metrics();
    _init_net_metrics();
    _init_procStats_metrics();
}

void* monitoring(void* arg)
{
    json_handler* json_struct = (json_handler*)arg;
    // mkfifo(PATH_TO_FIFO, FILE_PERMISSIONS);
    _read_fifo(json_struct);
    // while (1){}
}

// Destruimos el mutex
void destroy_mutex()
{
    pthread_mutex_destroy(&fifo_lock);
    pthread_mutex_destroy(&lock);
}
