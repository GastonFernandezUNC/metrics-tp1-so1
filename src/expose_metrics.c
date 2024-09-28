#include "expose_metrics.h"

/** Mutex para sincronización de hilos */
pthread_mutex_t lock;

/** Métrica de Prometheus para el uso de CPU */
static prom_gauge_t* cpu_usage_metric;

/** Métrica de Prometheus para el uso de memoria */
typedef struct{

    prom_gauge_t* memory_percentage_metric;
    prom_gauge_t* memory_total_metric;
    prom_gauge_t* memory_available_metric;
    prom_gauge_t* memory_used_metric;

}expose_mem_metrics;

static expose_mem_metrics memMetrics;

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

void update_memory_gauge()
{
    memInfo mem = *get_memory_usage();
    
    if (mem.percentage >= 0 || mem.memAvailable >= 0 || mem.memUsed >= 0 || mem.memTotal >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(memMetrics.memory_percentage_metric, mem.percentage, NULL);
        prom_gauge_set(memMetrics.memory_available_metric, mem.memAvailable, NULL);
        prom_gauge_set(memMetrics.memory_used_metric, mem.memUsed, NULL);
        prom_gauge_set(memMetrics.memory_total_metric, mem.memTotal, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el uso de memoria\n");
    }
}

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


// Creamos la métrica para el uso de memoria
void _init_memory_metrics(){

    memMetrics.memory_percentage_metric = prom_gauge_new("memory_usage_percentage", "Porcentaje de uso de memoria", 0, NULL);
        if (memMetrics.memory_percentage_metric == NULL)
        {
            fprintf(stderr, "Error al crear la métrica de uso de memoria\n");
            return EXIT_FAILURE;
        }

    memMetrics.memory_available_metric = prom_gauge_new("available_memory", "Memoria Disponible", 0, NULL);
        if (memMetrics.memory_available_metric == NULL)
        {
            fprintf(stderr, "Error al crear la métrica de uso de memoria\n");
            return EXIT_FAILURE;
        }

    memMetrics.memory_used_metric = prom_gauge_new("currently_used_memory", "Memoria utilizada", 0, NULL);
        if (memMetrics.memory_used_metric == NULL)
        {
            fprintf(stderr, "Error al crear la métrica de uso de memoria\n");
            return EXIT_FAILURE;
        }

    memMetrics.memory_total_metric = prom_gauge_new("Total_memory_amount", "Cantidad Total de Memoria", 0, NULL);
        if (memMetrics.memory_total_metric == NULL)
        {
            fprintf(stderr, "Error al crear la métrica de uso de memoria\n");
            return EXIT_FAILURE;
        }

    if(prom_collector_registry_must_register_metric(memMetrics.memory_percentage_metric)    != 0 &&
        prom_collector_registry_must_register_metric(memMetrics.memory_available_metric)    != 0 &&
        prom_collector_registry_must_register_metric(memMetrics.memory_used_metric)         != 0 &&
        prom_collector_registry_must_register_metric(memMetrics.memory_total_metric)        != 0)
    {
        fprintf(stderr, "Error al registrar las métricas\n");
        return EXIT_FAILURE;
    }
}

void _init_cpu_metrics(){

}

void init_metrics()
{
    // Inicializamos el mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        fprintf(stderr, "Error al inicializar el mutex\n");
        return EXIT_FAILURE;
    }

    // Inicializamos el registro de coleccionistas de Prometheus
    if (prom_collector_registry_default_init() != 0)
    {
        fprintf(stderr, "Error al inicializar el registro de Prometheus\n");
        return EXIT_FAILURE;
    }

    // Creamos la métrica para el uso de CPU
    cpu_usage_metric = prom_gauge_new("cpu_usage_percentage", "Porcentaje de uso de CPU", 0, NULL);
    if (cpu_usage_metric == NULL)
    {
        fprintf(stderr, "Error al crear la métrica de uso de CPU\n");
        return EXIT_FAILURE;
    }

    _init_memory_metrics();
    _init_cpu_metrics();

    // Registramos las métricas en el registro por defecto
    if (prom_collector_registry_must_register_metric(cpu_usage_metric) != 0){
        fprintf(stderr, "Error al registrar las métricas\n");
        return EXIT_FAILURE;
    }
}

void destroy_mutex()
{
    pthread_mutex_destroy(&lock);
}
