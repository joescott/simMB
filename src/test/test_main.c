/**
 * @file test_main.c
 *  Testing routines 
 */

#include "tdd.h"

extern int test_cbuffer();
extern int test_hist();

#include <stdio.h>

int all_test()
{
	int rtn = 0;
    rtn += test_cbuffer();
    rtn += test_hist();
    return rtn;
}

int main(int argc, char **argv)
{
    int rtn;
    if((rtn=all_test()) == 0)
        printf("TEST PASSED\n");
    return rtn != 0;
}