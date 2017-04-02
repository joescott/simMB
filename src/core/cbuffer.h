#ifndef __CBUFFER_H__
#define __CBUFFER_H__

typedef struct {
   void **read;
   void **write;
   void **pool;
   unsigned int num_total;
}CBUFF;

CBUFF *init_cbuffer(const unsigned int num_elements);
void clean_cbuffer(CBUFF *cbuff);

void* read_cbuffer(CBUFF *cbuff);
int write_cbuffer(CBUFF *cbuff, void *element);

int get_cbuff_elements(CBUFF *cbuff);
int inc_readcbuffer(CBUFF *cbuff,int force);
int dec_readcbuffer(CBUFF *cbuff,int force);
int inc_writecbuffer(CBUFF *cbuff);

#endif
