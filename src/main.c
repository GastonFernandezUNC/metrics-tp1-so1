/**
 * @file main.c
 * @brief Entry point of the system
 */

#include "expose_metrics.h"
#include <stdbool.h>

/**
 * @brief sleep
 */
#define SLEEP_TIME 1

/**
 * @brief main
 */
int main(int argc, char* argv[])
{

    // Creamos un hilo para exponer las métricas vía HTTP
    pthread_t tid;
    if (pthread_create(&tid, NULL, expose_metrics, NULL) != 0)
    {
        fprintf(stderr, "Error al crear el hilo del servidor HTTP\n");
        return EXIT_FAILURE;
    }

    // Call the function to initialize the metrics
    init_metrics();
    // Bucle principal para actualizar las métricas cada segundo
    while (true)
    {
        update_cpu_gauge();
        update_memory_gauge();
        update_disk_gauge();
        update_net_gauge();
        update_procStats_gauge();
        sleep(SLEEP_TIME);
    }

    /**
     * @brief exit function
     */
    return EXIT_SUCCESS;
}