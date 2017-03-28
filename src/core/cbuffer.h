#ifndef __CBUFFER_H__
#define __CBUFFER_H__

typedef struct {
   void **read;
   void **write;
   void **pool;
   unsigned int num_elements;
}CBUFF;

CBUFF *init_cbuffer(const unsigned int num_elements);
void clean_cbuffer(CBUFF *cbuff);

void* read_cbuffer(CBUFF *cbuff);
int write_cbuffer(CBUFF *cbuff, void *element);

#endif
