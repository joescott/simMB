/**
 * @file test_cbuffer.c
 *  Circular buffer testing routines 
 */

#include "tdd.h"
#include "../../src/core/cbuffer.h"

#include <stdio.h>
#include <string.h>

int tests_run = 0;

static _inc_readcbuffer(CBUFF *cb)
{
    printf("INC R\n");
    return inc_readcbuffer(cb);
}

static _dec_readcbuffer(CBUFF *cb)
{
    printf("DEC R\n");
    return dec_readcbuffer(cb);
}

static _inc_writecbuffer(CBUFF *cb)
{
    printf("INC W\n");
    return inc_writecbuffer(cb);
}

static void print_cb(CBUFF *cb)
{
    printf("------------------------\n");
    printf("R: %d <- %s\n",cb->read-cb->pool,  *(char**)cb->read);
    printf("W: %d <- %s\n",cb->write-cb->pool, *(char**)cb->write);
}

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
    print_cb(cb);
    /** Add a second string */ 
    i = write_cbuffer(cb,(void*)"TEST2");
    _assert(i == 2);
    print_cb(cb);
    /** Add a third string */ 
    i = write_cbuffer(cb,(void*)"TEST3");
    _assert(i == 3);
    print_cb(cb);
    /** Add a fourth string but losing first one */ 
    i = write_cbuffer(cb,(void*)"TEST4");
    _assert(i == 3);
    print_cb(cb);

    /** Read second string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST2")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST3")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST4")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST4")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST3")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST2")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST2")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST3")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST4")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST4")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST3")==0);
    print_cb(cb);
    /** Add a fifth string but losing second one */ 
    i = write_cbuffer(cb,(void*)"TEST5");
    _assert(i == 3);
    print_cb(cb);

    /** Increment Read pointer */
    _dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(i == 3);
    _assert(strcmp(c,"TEST4")==0);
    print_cb(cb);

    return 0;
}

int test_cbuffer()
{
    _verify(test_init_cbuffer);
    _verify(test_write_read_cbuffer);
    _verify(test_increment_cbuffer);
    return 0;
}
