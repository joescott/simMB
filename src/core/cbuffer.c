
#include <stdlib.h>
#include "cbuffer.h"

static int get_cbuff_elements(CBUFF *cbuff)
{
    if(cbuff->read < cbuff->write)
        return cbuff->write - cbuff->read;
    else if(cbuff->read > cbuff->write)
        return (cbuff->write - cbuff->pool) + 
            ((cbuff->pool+cbuff->num_total_elements) - cbuff->read)+1;
    else
        return 0;
}

static void inc_readcbuffer(CBUFF *cbuff)
{
    if(cbuff->read != cbuff->write)
    {
        if(!((cbuff->read + 1 - cbuff->pool) % cbuff->num_total_elements))
            cbuff->read = cbuff->pool;
        else
            cbuff->read++;
    }
}

static void inc_writecbuffer(CBUFF *cbuff)
{
    if(!((cbuff->write + 1 - cbuff->pool) % cbuff->num_total_elements))
        cbuff->write = cbuff->pool;
    else
        cbuff->write++;

    if(cbuff->read == cbuff->write)
            cbuff->read++;
}

CBUFF *init_cbuffer(const unsigned int num_elements)
{
    CBUFF *cbuff = (CBUFF *)malloc(sizeof(CBUFF));
    cbuff->pool = (void**)calloc(num_elements,sizeof(void*));
    cbuff->num_total_elements = num_elements;
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
    return *cbuff->read;
}

int write_cbuffer(CBUFF *cbuff, void *element)
{
    *cbuff->write = element;
    inc_writecbuffer(cbuff);
    return get_cbuff_elements(cbuff); 
}
