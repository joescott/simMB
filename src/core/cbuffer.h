#ifndef __CBUFFER_H__
#define __CBUFFER_H__

typedef void * CBUFF_ELEMENT_TYPE;

typedef struct {
   CBUFF_ELEMENT_TYPE *read;
   CBUFF_ELEMENT_TYPE *write;
   CBUFF_ELEMENT_TYPE *pool;
   unsigned int num_total;
   struct {
   		unsigned int inc_read_allow:1;
   		unsigned int dec_read_allow:1;
   }ctrl;
}CBUFF;

CBUFF *init_cbuffer(const unsigned int num_elements);
void clean_cbuffer(CBUFF *cbuff);

CBUFF_ELEMENT_TYPE read_cbuffer(CBUFF *cbuff);
int write_cbuffer(CBUFF *cbuff, CBUFF_ELEMENT_TYPE element);

int get_cbuff_elements(CBUFF *cbuff);
int inc_readcbuffer(CBUFF *cbuff);
int dec_readcbuffer(CBUFF *cbuff);
int inc_writecbuffer(CBUFF *cbuff);

#endif
