/**
 * @file metrics.h
 * @brief Funciones para obtener el uso de CPU y memoria desde el sistema de archivos /proc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256 // Tamaño del buffer para leer archivos

/*
    Struct to save the memory information
*/
typedef struct
{
    // total memory
    unsigned long long memTotal;

    // used memory
    unsigned long long memUsed;

    // available memory
    unsigned long long memAvailable;

    // memory percentage
    unsigned long long percentage;

} memInfo;

/*
    Struct to save the disk information
*/
typedef struct
{
    // reads completed successfully
    unsigned long long reads_completed_successfully;

    // writes completed
    unsigned long long writes_completed;

    // reads per second
    unsigned long long reads_per_second;

    // writes per second
    unsigned long long writes_per_second;

} diskStats;

/*
    Struct to save the network information
*/
typedef struct
{
    // bytes received
    unsigned long long rx_bytes;

    // bytes transmitted
    unsigned long long tx_bytes;

    // packets received
    unsigned long long rx_packets;

    // packets transmitted
    unsigned long long tx_packets;

    // errors in reception
    unsigned long long rx_errors;

    // errors in transmission
    unsigned long long tx_errors;

} netStats;

/*
    Struct to save the processes information
*/
typedef struct
{
    // running processes
    unsigned long long running_processes;

    // context switching
    unsigned long long context_switching;

} proc_stats;

/*
    static values to be accessed from the functions
*/

static proc_stats stat; // struct to save the process information
static memInfo mem;     // struct to save the memory information
static diskStats disk;  // struct to save the disk information
static netStats net;    // struct to save the network information

/*
    Returns a pointer to the struct with the process information
*/
proc_stats* get_procStats_usage();

/*
    Returns a pointer to the struct with the net information
*/
netStats* get_net_usage();

/*
    Returns a pointer to the struct with the disk information
*/
diskStats* get_disk_usage();

/*
    Returns a pointer to the struct with the memory information
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
