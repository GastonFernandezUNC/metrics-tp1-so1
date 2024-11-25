#include "cjson_handler.h"

int _get_json(json_handler* json_struct)
{
    FILE* fp;
    char* content = NULL;
    size_t content_size = 0;
    char buffer[FILE_BUFFER_SIZE];
    // Opening file
    fp = fopen(PATH_TO_CONFIG_FILE, "r");
    if (fp == NULL)
    {
        printf("%s\n", PATH_TO_CONFIG_FILE);
        perror("Error opening file CONFIG");
        return -1;
    }

    // Allocate initial memory for content
    content = malloc(INITIAL_SIZE);
    if (content == NULL)
    {
        perror("Memory allocation failed");
        fclose(fp);
        return -1;
    }
    content[0] = '\0'; // Initialize empty string

    // Read file line by line and append to content
    while (fgets(buffer, sizeof(buffer), fp))
    {
        size_t line_length = strlen(buffer);

        // Resize content buffer if necessary
        if (content_size + line_length + 1 > INITIAL_SIZE)
        {
            char* temp = realloc(content, content_size + line_length + 1);
            if (temp == NULL)
            {
                perror("Memory reallocation failed");
                free(content);
                fclose(fp);
                return -1;
            }
            content = temp;
        }

        // Append current line to content
        strcat(content, buffer);
        content_size += line_length;
    }

    json_struct->json = cJSON_Parse(content);
    fclose(fp);
    free(content);
}

void read_json_content(json_handler* json_struct)
{
    cJSON* json = json_struct->json;
    cJSON* metrics = cJSON_GetObjectItem(json, "metrics");
    cJSON* refresh_time = cJSON_GetObjectItem(json, "refresh_time");

    if (refresh_time)
    {
        json_struct->refresh_time = refresh_time->valueint;
    }

    if (metrics)
    {
        cJSON* cpu = cJSON_GetObjectItem(metrics, "cpu");
        if (cJSON_IsObject(cpu)) {
            cJSON* enabled = cJSON_GetObjectItemCaseSensitive(cpu, "enabled");
            json_struct->cpu = cJSON_IsTrue(enabled);
        }
        
        cJSON* memory = cJSON_GetObjectItem(metrics, "memory");
        if (cJSON_IsObject(memory)) {
            cJSON* enabled = cJSON_GetObjectItemCaseSensitive(memory, "enabled");
            json_struct->mem = cJSON_IsTrue(enabled);
        }
        
        cJSON* disk = cJSON_GetObjectItem(metrics, "disk");
        if (cJSON_IsObject(disk)) {
            cJSON* enabled = cJSON_GetObjectItemCaseSensitive(disk, "enabled");
            json_struct->disk = cJSON_IsTrue(enabled);
        }

        cJSON* network = cJSON_GetObjectItem(metrics, "network");
        if (cJSON_IsObject(network)) {
            cJSON* enabled = cJSON_GetObjectItemCaseSensitive(network, "enabled");
            json_struct->net = cJSON_IsTrue(enabled);
        }
        cJSON* proc_stats = cJSON_GetObjectItem(metrics, "proc_stats");
        if (cJSON_IsObject(proc_stats)) {
            cJSON* enabled = cJSON_GetObjectItemCaseSensitive(proc_stats, "enabled");
            json_struct->proc = cJSON_IsTrue(enabled);
        }

    }
}