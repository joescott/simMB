
#include <stdlib.h>
#include "cbuffer.h"

CBUFF *init_cbuffer(const unsigned int num_elements)
{
    CBUFF *cbuff = (CBUFF *)malloc(sizeof(CBUFF));
    cbuff->pool = (void**)calloc(num_elements,sizeof(void*));
    cbuff->num_elements = 0;
    cbuff->read = cbuff->write = cbuff->pool;
    return cbuff;
}

void clean_cbuffer(CBUFF *cbuff)
{
    free(cbuff->pool);
    free(cbuff);
}

void* read_cbuffer(CBUFF *cbuff)
{
    cbuff->num_elements++;
    return *cbuff->read++;
}

int write_cbuffer(CBUFF *cbuff, void *element)
{
    *cbuff->write++ = element;
    cbuff->num_elements++;
    return cbuff->num_elements; 
}
