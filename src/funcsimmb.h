#ifndef __FUNCHMI_H__
#define __FUNCHMI_H__

#include "simmb.h"
/**
 * funchmi.h
 */
#define SLVMB_PROC_DEF(x)  int x(DATA_ST *d_st,uint8_t *mb_query)
typedef int (*SLVMB_PROC)(DATA_ST *, uint8_t*); 

typedef struct {
	const uint8_t id;
	const uint8_t argc;
	SLVMB_PROC proc; 
}SLVMB_FUNCTION_ST;

typedef struct {
	const unsigned int addr;
	SLVMB_PROC proc; 
}SLVMB_READ_OHR_ST;

/**
 * Functions regarded to Output Holding Registers
 */
SLVMB_PROC_DEF(init_var);
SLVMB_PROC_DEF(get_var);
SLVMB_PROC_DEF(set_var);

#endif
