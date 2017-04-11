#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cbuffer.h"

#define NUM_HIST 10 
static CBUFF *cb;

void 
inithist(void)
{
	cb = init_cbuffer(NUM_HIST);
}

int 
addtohist(char *buff)
{
    return write_cbuffer(cb,(void*)buff);
}

char * 
upfromhist(void)
{
    return "NOP";
}

char * 
downfromhist(void)
{
    return "NOP";
}

