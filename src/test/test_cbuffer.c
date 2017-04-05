/**
 * @file test_cbuffer.c
 *  Circular buffer testing routines 
 */

#include "tdd.h"
#include "../../src/core/cbuffer.h"

#include <stdio.h>
#include <string.h>

int tests_run = 0;

int test_init_cbuffer()
{
    CBUFF *cb = init_cbuffer(10);
    _assert(cb != NULL);
    return 0;
}

int test_write_read_cbuffer()
{
    CBUFF *cb = init_cbuffer(10);
    int i = write_cbuffer(cb,(void*)"TEST");
    char *c = (char *) read_cbuffer(cb);
    _assert(i == 1);
    _assert(strcmp(c,"TEST")==0);
    return 0;
}


int test_increment_cbuffer()
{
    int i;
    char *c;
    /** Init 3 elements Buffer */
    CBUFF *cb = init_cbuffer(3);
    /** Add a first string */ 
    i = write_cbuffer(cb,(void*)"TEST1");
    _assert(i == 1);
    /** Add a second string */ 
    i = write_cbuffer(cb,(void*)"TEST2");
    _assert(i == 2);
    /** Add a third string */ 
    i = write_cbuffer(cb,(void*)"TEST3");
    _assert(i == 3);
    /** Add a fourth string but losing first one */ 
    i = write_cbuffer(cb,(void*)"TEST4");
    _assert(i == 3);
    /** Read second string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST2")==0);
    /** Increment Read pointer */
    inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST3")==0);
    /** Increment Read pointer */
    inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST4")==0);
    /** Increment Read pointer */
    inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST4")==0);
    /** Increment Read pointer */
    dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST3")==0);
    /** Increment Read pointer */
    dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST2")==0);
    /** Increment Read pointer */
    dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST2")==0);
    /** Increment Read pointer */
    inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST3")==0);
    /** Increment Read pointer */
    inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST4")==0);
    /** Increment Read pointer */
    inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST4")==0);
    /** Increment Read pointer */
    dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST3")==0);
    /** Add a fifth string but losing second one */ 
    i = write_cbuffer(cb,(void*)"TEST5");
    _assert(i == 3);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    printf("%d %s\n",i ,c);
    _assert(i == 3);
    _assert(strcmp(c,"TEST5")==0);
    
    return 0;
}

int test_cbuffer()
{
    _verify(test_init_cbuffer);
    _verify(test_write_read_cbuffer);
    _verify(test_increment_cbuffer);
    return 0;
}
