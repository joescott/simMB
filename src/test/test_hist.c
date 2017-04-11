/**
 * @file test_hist.c
 *  History testing routines 
 */

#include "tdd.h"
#include "../../src/core/hist.h"

#include <stdio.h>
#include <string.h>


int test_init_hist()
{
    inithist();
    return 0;
}

int test_add_hist()
{
    int i;
    i = addtohist("TEST1");
    _assert(i == 1);
    i = addtohist("TEST2");
    _assert(i == 2);
    i = addtohist("TEST3");
    _assert(i == 3);
    i = addtohist("TEST4");
    _assert(i == 4);
    i = addtohist("TEST5");
    _assert(i == 5);
    i = addtohist("TEST6");
    _assert(i == 6);
    i = addtohist("TEST7");
    _assert(i == 7);
    i = addtohist("TEST8");
    _assert(i == 8);
    i = addtohist("TEST9");
    _assert(i == 9);
    i = addtohist("TEST10");
    _assert(i == 10);
    return 0;
}

int test_up_hist()
{
    char *c; 
    c = upfromhist();  
    _assert(strcmp(c,"TEST10")==0);
    c = upfromhist();  
    _assert(strcmp(c,"TEST9")==0);
    c = upfromhist();  
    _assert(strcmp(c,"TEST8")==0);
    c = upfromhist();  
    _assert(strcmp(c,"TEST7")==0);
    c = upfromhist();  
    _assert(strcmp(c,"TEST6")==0);
    c = upfromhist();  
    _assert(strcmp(c,"TEST5")==0);
    c = upfromhist();  
    _assert(strcmp(c,"TEST4")==0);
    c = upfromhist();  
    _assert(strcmp(c,"TEST3")==0);
    c = upfromhist();  
    _assert(strcmp(c,"TEST2")==0);
    c = upfromhist();  
    _assert(strcmp(c,"TEST1")==0);
    return 0;
}

int test_down_hist()
{
    char *c; 
    return 0;
}

int test_hist()
{
    _verify(test_init_hist);
    _verify(test_add_hist);
    _verify(test_up_hist);
    _verify(test_down_hist);
    return 0;
}
