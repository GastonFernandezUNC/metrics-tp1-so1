/**
 * @file expose_metrics.h
 * @brief Programa para leer el uso de CPU y memoria y exponerlos como métricas de Prometheus.
 */

#include "metrics.h"
#include "cjson_handler_tp1.h"
#include <errno.h>
#include <prom.h>
#include <promhttp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



/**
 * @brief Size of the buffer used for storing data.
 */
#define BUFFER_SIZE 256

#define PATH_TO_FIFO "/tmp/monitor_fifo"
#define FIFO_BUFFER_SIZE 1024
#define FILE_PERMISSIONS 0666

/**
 * @brief Actualiza las métricas de running processes
 * y de context switching.
 */
void update_procStats_gauge();

/**
 * @brief Actualiza las métricas de uso de red.
 */
void update_net_gauge();

/**
 * @brief Actualiza las métricas de uso de disco.
 */
void update_disk_gauge();

/**
 * @brief Actualiza la métrica de uso de CPU.
 */
void update_cpu_gauge();

/**
 * @brief Actualiza la métrica de uso de memoria.
 */
void update_memory_gauge();

/**
 * @brief Función del hilo para exponer las métricas vía HTTP en el puerto 8000.
 * @param arg Argumento no utilizado.
 * @return NULL
 */
void* expose_metrics(void* arg);

void* monitoring(void* arg);

/**
 * @brief Inicializar mutex y métricas.
 */
void init_metrics();

/**
 * @brief Destructor de mutex
 */
void destroy_mutex();
