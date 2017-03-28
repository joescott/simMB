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

int test_cbuffer()
{
    _verify(test_init_cbuffer);
    _verify(test_write_read_cbuffer);
    return 0;
}
