
#include <stdlib.h>
#include "cbuffer.h"

int get_cbuff_elements(CBUFF *cbuff)
{
    int num; 
    if(cbuff->read > cbuff->write)
        num = (cbuff->write - cbuff->pool) + 
            (cbuff->pool + cbuff->num_total - cbuff->read) + 1;
    else if(cbuff->read < cbuff->write)
        num = cbuff->write - cbuff->read;
    else
    {
        if(*cbuff->write == NULL)
            num = cbuff->write - cbuff->read;
        else
            num = cbuff->num_total;
    }
    return num;
}

int inc_readcbuffer(CBUFF *cbuff)
{
    void **lread = cbuff->read;
    if(cbuff->ctrl.inc_read_allow || cbuff->read != cbuff->write)
    {
        cbuff->ctrl.inc_read_allow = 0;
        cbuff->read = cbuff->pool + (cbuff->read + 1 - cbuff->pool)%cbuff->num_total;
        if(cbuff->read == cbuff->write)
        {
            cbuff->ctrl.dec_read_allow = 1;
            cbuff->read = lread;
            return 0;
        } 
        return 1;
    }
    return 0;
}

int dec_readcbuffer(CBUFF *cbuff)
{
    void **lread = cbuff->read;
    if(cbuff->ctrl.dec_read_allow || cbuff->read != cbuff->write)
    {
        cbuff->ctrl.dec_read_allow = 0;
        cbuff->read = cbuff->pool + ((cbuff->read - 1 - cbuff->pool)%cbuff->num_total);
        if(cbuff->read == cbuff->write)
        { 
            cbuff->ctrl.inc_read_allow = 1;
            cbuff->read = lread;
            return 0;
        }
        return 1;
    }
    return 0;
}

int inc_writecbuffer(CBUFF *cbuff)
{
    cbuff->write = cbuff->pool + ((cbuff->write + 1 - cbuff->pool)%cbuff->num_total);

    if(*cbuff->write)
    {
        cbuff->ctrl.inc_read_allow = 1;
        cbuff->read = cbuff->write;
    }

    return get_cbuff_elements(cbuff);
}

CBUFF *init_cbuffer(const unsigned int num_elements)
{
    CBUFF *cbuff = (CBUFF *)malloc(sizeof(CBUFF));
    cbuff->pool = (CBUFF_ELEMENT_TYPE*)calloc(num_elements,sizeof(CBUFF_ELEMENT_TYPE));
    cbuff->num_total = num_elements;
    cbuff->read = cbuff->write = cbuff->pool; 
    return cbuff;
}

void clean_cbuffer(CBUFF *cbuff)
{
    free(cbuff->pool);
    free(cbuff);
}

CBUFF_ELEMENT_TYPE read_cbuffer(CBUFF *cbuff)
{
    return *cbuff->read;
}

int write_cbuffer(CBUFF *cbuff, CBUFF_ELEMENT_TYPE element)
{
    *cbuff->write = element;
    inc_writecbuffer(cbuff);
    return get_cbuff_elements(cbuff); 
}
