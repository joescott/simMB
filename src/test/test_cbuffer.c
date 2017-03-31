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
    CBUFF *cb = init_cbuffer(3);
    i = write_cbuffer(cb,(void*)"TEST1");
    _assert(i == 1);
    i = write_cbuffer(cb,(void*)"TEST2");
    _assert(i == 2);
    i = write_cbuffer(cb,(void*)"TEST3");
    _assert(i == 3);
    i = write_cbuffer(cb,(void*)"TEST4");
    _assert(i == 3);
    c = (char *) read_cbuffer(cb);
    printf("%s\n",c);
    _assert(strcmp(c,"TEST2")==0);
    return 0;
}

int test_cbuffer()
{
    _verify(test_init_cbuffer);
    _verify(test_write_read_cbuffer);
    _verify(test_increment_cbuffer);
    return 0;
}
