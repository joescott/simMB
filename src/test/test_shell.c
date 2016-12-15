/**
 * @file test_shell.c
 *  Shell testing routines 
 */

#include "tdd.h"

#include <stdio.h>

int tests_run = 0;

int dummy_fail()
{
    _assert(1 == 2);
    return 0;
}

int dummy_pass()
{
    _assert(1 != 2);
    return 0;
}

int all_test()
{
    _verify(dummy_pass);
    _verify(dummy_fail);
    return 0;
}

int main(int argc, char **argv)
{
    int rtn;
    if((rtn=all_test()) == 0)
        printf("PASSED\n");
    printf("Test run %d\n", tests_run);
    return rtn != 0;
}
