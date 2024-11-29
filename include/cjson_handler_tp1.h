#ifndef _JSON_HANDLER_TP1_H
#define _JSON_HANDLER_TP1_H
#include "cjson/cJSON.h"
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH_TO_CONFIG_FILE "../config.json"

/**
 * @brief Initial size for the buffer.
 */
#define INITIAL_SIZE 1024
 
/**
 * @brief Size of the buffer used for storing data.
 */
#define FILE_BUFFER_SIZE 1024

/**
 * @brief Struct to store the JSON data.
 */
typedef struct json_handler
{
    /**
     * @brief Pointer to the json object.
     */
    cJSON* json;

    /**
     * @brief Refresh time for the metrics.
     */
    int refresh_time;

    /**
     * @brief Flag to indicate if the CPU metrics are enabled.
     */
    bool cpu;

    /**
     * @brief Flag to indicate if the memory metrics are enabled.
     */
    bool mem;

    /**
     * @brief Flag to indicate if the disk metrics are enabled.
     */
    bool disk;

    /**
     * @brief Flag to indicate if the network metrics are enabled.
     */
    bool net;

    /**
     * @brief Flag to indicate if the process metrics are enabled.
     */
    bool proc;

} json_handler;

/**
 * @brief Function to get the JSON object to be parsed, and its data.
 * 
 * @param json_struct Struct to store the JSON data.
 * @return int 0 if the JSON data was obtained successfully, -1 otherwise.
 */
int _get_json(json_handler* json_struct);

/**
 * @brief Function to read the JSON content and store it in the struct.
 * 
 * @param json_struct Struct to store the JSON data.
 */
void read_json_content(json_handler* json_struct);

#endif
