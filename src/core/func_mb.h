/**
 * @file func_mb.h
 */
#ifndef __FUNC_MB_H__
#define __FUNC_MB_H__

#include "common.h"
#define MB_PROC_DEF(x)  int x(MB_DATA_ST *mb_data)
typedef int (*MB_PROC)(MB_DATA_ST *); 

typedef struct {
	const uint8_t id;
	const uint8_t argc;
	MB_PROC proc; 
}MB_FUNCTION_ST;

typedef struct {
	const unsigned int addr;
	MB_PROC proc; 
}MB_READ_OHR_ST;

/**
 * Functions regarded to Output Holding Registers
 */
MB_PROC_DEF(get_var);
MB_PROC_DEF(set_var);

#endif
