#ifndef __CBUFFER_H__
#define __CBUFFER_H__

typedef struct {
   void *read;
   void *write;
   void *pool;
   unsigned int num_total;
   struct {
   		unsigned int inc_read_allow:1;
   		unsigned int dec_read_allow:1;
   }ctrl;
}CBUFF;

CBUFF *init_cbuffer(const unsigned int num_elements);
void clean_cbuffer(CBUFF *cbuff);

void* read_cbuffer(CBUFF *cbuff);
int write_cbuffer(CBUFF *cbuff, void *element);

int get_cbuff_elements(CBUFF *cbuff);
int inc_readcbuffer(CBUFF *cbuff);
int dec_readcbuffer(CBUFF *cbuff);
int inc_writecbuffer(CBUFF *cbuff);

#endif
