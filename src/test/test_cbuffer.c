/**
 * @file test_cbuffer.c
 *  Circular buffer testing routines 
 */

#include "tdd.h"
#include "../../src/core/cbuffer.h"

#include <stdio.h>
#include <string.h>

int tests_run = 0;

static CBUFF *cb;

#define CBUFFER_DEBUG
#ifndef CBUFFER_DEBUG

#define _inc_readcbuffer inc_readcbuffer
#define _dec_readcbuffer dec_readcbuffer
#define _inc_writecbuffer inc_writecbuffer 
#define print_cb

#else

static int _inc_readcbuffer(CBUFF *cb)
{
    printf("\nAccion INC R\n");
    return inc_readcbuffer(cb);
}

static int _dec_readcbuffer(CBUFF *cb)
{
    printf("\nAccion DEC R\n");
    return dec_readcbuffer(cb);
}

static int _inc_writecbuffer(CBUFF *cb)
{
    printf("\nAccion INC W\n");
    return inc_writecbuffer(cb);
}

static void print_cb(CBUFF *cb)
{
    printf("\n------------------------\n");
    printf("R: %d <- %s\n",cb->read-cb->pool,  *(char**)cb->read);
    printf("W: %d <- %s\n",cb->write-cb->pool, *(char**)cb->write);
}
#endif

int test_init_cbuffer()
{
    cb = init_cbuffer(3);
    _assert(cb != NULL);
    return 0;
}

int test_write_read_cbuffer()
{
    int i = write_cbuffer(cb,(void*)"TEST1");
    char *c = (char *) read_cbuffer(cb);
    _assert(i == 1);
    _assert(strcmp(c,"TEST1")==0);
    print_cb(cb);
    return 0;
}

int test_increment_cbuffer()
{
    int i;
    char *c;
    /** Add a second string */ 
    i = write_cbuffer(cb,(void*)"TEST2");
    _assert(i == 2);
    print_cb(cb);
    /** Add a third string */ 
    i = write_cbuffer(cb,(void*)"TEST3");
    _assert(i == 3);
    print_cb(cb);

    /** Read second string added */
    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST1")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST2")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST3")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST3")==0);
    print_cb(cb);

    return 0;
}

int test_decrement_cbuffer()
{
    char *c;
    /** Increment Read pointer */
    _dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST2")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST1")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _dec_readcbuffer(cb);
    /** Read third string added */
    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST1")==0);
    print_cb(cb);
    return 0;
}

int test_overwrite_cbuffer()
{
    int i;
    char *c;
    print_cb(cb);
    i = write_cbuffer(cb,(void*)"TEST4");
    _assert(i == 3);
    print_cb(cb);
    i = write_cbuffer(cb,(void*)"TEST5");
    _assert(i == 3);
    print_cb(cb);
    i = write_cbuffer(cb,(void*)"TEST6");
    _assert(i == 3);
    print_cb(cb);
    i = write_cbuffer(cb,(void*)"TEST7");
    _assert(i == 3);
    print_cb(cb);

    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST5")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST6")==0);
    print_cb(cb);
    /** Increment Read pointer */
    _inc_readcbuffer(cb);
    c = (char *) read_cbuffer(cb);
    _assert(strcmp(c,"TEST7")==0);
    print_cb(cb);
    return 0;
}

int test_cbuffer()
{
    _verify(test_init_cbuffer);
    _verify(test_write_read_cbuffer);
    _verify(test_increment_cbuffer);
    _verify(test_decrement_cbuffer);
    _verify(test_overwrite_cbuffer);
    return 0;
}
