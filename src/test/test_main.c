/**
 * @file test_main.c
 *  Testing routines 
 */

#include "tdd.h"

extern int test_cbuffer();

#include <stdio.h>

int all_test()
{
    test_cbuffer();
    return 0;
}

int main(int argc, char **argv)
{
    int rtn;
    if((rtn=all_test()) == 0)
        printf("PASSED\n");
    return rtn != 0;
}
