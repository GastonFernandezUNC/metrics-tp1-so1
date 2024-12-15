#include <stdlib.h>
#include "mmemory.h"
#include "functions.h"

float fragmentation_rate(void)
{
    mem_data m_usage = {0, 0, 0};
    memory_usage(&m_usage);

    float frag_rate = 100 * ((float)m_usage.free_mem - (float)m_usage.biggest_block) / (float)m_usage.free_mem;
  
    if ( frag_rate < 0 || frag_rate > 100 || m_usage.free_mem == 0)
    {
        return -1;
    }
    
    else return frag_rate;

}

int _random_value(void)
{
    return rand() % 1024;
}

void simulator(void)
{

    srand(time(NULL));
    void *ptr1 = malloc(_random_value());
    void *ptr2 = malloc(_random_value());
    void *ptr3 = malloc(_random_value());
    void *ptr4 = malloc(_random_value());
    void *ptr5 = malloc(_random_value());
    void *ptr6 = malloc(_random_value());

    t_block block1 = get_block(ptr1);
    t_block block2 = get_block(ptr2);
    t_block block3 = get_block(ptr3);
    t_block block4 = get_block(ptr4);
    t_block block5 = get_block(ptr5);
    t_block block6 = get_block(ptr6);



    size_t request_sizes[] = {block1->size, block2->size, 
                              block3->size, block4->size,
                              block5->size, block6->size};

    int n_requests = sizeof(request_sizes) / sizeof(request_sizes[0]);

    evaluate_policies(request_sizes, n_requests);
    

    printf("\nFragmentation Before Free: %f\n",fragmentation_rate());
    
    free(ptr1);
    free(ptr2);
    free(ptr3);

    printf("\nFragmentation Between Free: %f\n",fragmentation_rate());

    free(ptr4);
    free(ptr5);    
    free(ptr6);

    printf("\nFragmentation After Free: %f\n",fragmentation_rate());
}

void main(void)
{
    
    simulator();

}