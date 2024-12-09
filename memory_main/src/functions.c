#include "mmemory.h"
#include "functions.h"



float fragmentation_rate(int)
{
    struct mem_usage* mem_usage;
    memory_usage(mem_usage);

    float frag_rate = 100 * (float)mem_usage->biggest_block / (float)mem_usage->free_mem;

    

    return -1;
}