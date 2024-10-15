/**
 * @file metrics.h
 * @brief Funciones para obtener el uso de CPU y memoria desde el sistema de archivos /proc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Size of the buffer used for storing data.
 */
#define BUFFER_SIZE 256 // Tamaño del buffer para leer archivos

/*
* @brief 
*/
#define NAME_LENGHT 32

/*
    Struct to save the memory information
*/
/**
 * @brief Struct to save the memory information.
 */
typedef struct
{
    /**
     * @brief Total memory available.
     */
    unsigned long long memTotal;

    /**
     * @brief Free memory available.
     */
    unsigned long long memAvailable;

    /**
     * @brief Used memory.
     */
    unsigned long long memUsed;

    /**
     * @brief Percentage of used memory.
     */
    unsigned long long percentage;

} memInfo;

/*
    Struct to save the disk information
*/
/**
 * @brief Struct to save the disk information.
 */
typedef struct
{
    /**
     * @brief Reads completed successfully.
     */
    unsigned long long reads_completed_successfully;

    /**
     * @brief Writes completed.
     */
    unsigned long long writes_completed;

    /**
     * @brief Reads per second.
     */
    unsigned long long reads_per_second;

    /**
     * @brief Writes per second.
     */
    unsigned long long writes_per_second;

} diskStats;

/**
 * @brief Struct to save the network statistics.
 */
typedef struct
{
    /**
     * @brief Total bytes received.
     */
    unsigned long long rx_bytes;

    /**
     * @brief Total bytes transmitted.
     */
    unsigned long long tx_bytes;

    /**
     * @brief Total packets received.
     */
    unsigned long long rx_packets;

    /**
     * @brief Total packets transmitted.
     */
    unsigned long long tx_packets;

    /**
     * @brief Total receive errors.
     */
    unsigned long long rx_errors;

    /**
     * @brief Total transmit errors.
     */
    unsigned long long tx_errors;

} netStats;

/*
    Struct to save the processes information
*/
/**
 * @brief Struct to save the processes information.
 */
typedef struct
{
    /**
     * @brief Number of running processes.
     */
    unsigned long long running_processes;

    /**
     * @brief Number of context switches.
     */
    unsigned long long context_switching;

} proc_stats;

/*
    static values to be accessed from the functions
*/

static proc_stats stat; // struct to save the process information
static memInfo mem;     // struct to save the memory information
static diskStats disk;  // struct to save the disk information
static netStats net;    // struct to save the network information

/**
 * @brief Retrieves the usage statistics of the process.
 *
 * This function collects and returns various usage statistics
 * related to the process, such as CPU usage, memory usage, etc.
 *
 * @return A structure containing the process usage statistics.
 */
proc_stats* get_procStats_usage();

/**
 * @brief Returns a pointer to the struct with the network information.
 *
 * This function provides access to the network usage statistics
 * by returning a pointer to the structure that contains this information.
 *
 * @return A pointer to the structure containing the network usage statistics.
 */
netStats* get_net_usage();

/**
 * @brief Returns a pointer to the struct with the disk information.
 *
 * This function provides access to the disk usage statistics
 * by returning a pointer to the structure that contains this information.
 *
 * @return A pointer to the structure containing the disk usage statistics.
 */
diskStats* get_disk_usage();

/**
 * @brief Returns a pointer to the struct with the memory information.
 *
 * This function provides access to the memory usage statistics
 * by returning a pointer to the structure that contains this information.
 *
 * @return A pointer to the structure containing the memory usage statistics.
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
