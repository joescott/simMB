#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_HIST 10 

char *hist[NUM_HIST];
char **phist_read;
char **phist_write;

void 
inithist(void)
{
    phist_read = phist_write = hist;
}

int 
addtohist(char *buff)
{
    if(strlen(buff) > 0)
    {
        if(phist_write == NULL) 
            free(phist_write);

        *phist_write = strdup(buff);
        if(phist_write - hist < NUM_HIST)
            phist_write++;
        phist_read = (phist_write-1);
    }
    return 0; 
}

char * 
upfromhist(void)
{
    return (phist_write-phist_read-1 > 0)?
        *++phist_read:*phist_read;
}

char * 
downfromhist(void)
{
    return (phist_read - hist > 0)?
        *--phist_read:*phist_read;
}

