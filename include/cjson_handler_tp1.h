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

#define INITIAL_SIZE 1024
#define PATH_TO_CONFIG_FILE "../config.json"
#define FILE_BUFFER_SIZE 1024


typedef struct json_handler
{
    cJSON* json;
    int refresh_time;
    bool cpu;
    bool mem;
    bool disk;
    bool net;
    bool proc;

} json_handler;

int _get_json(json_handler* json_struct);

void read_json_content(json_handler* json_struct);

#endif
