#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

#include "mb_def.h"
#include "mb_func.h"


/**
 * Transactional data 
 */
typedef struct{
    /**
     * ModBUS
     */
    MB_DATA_ST *mb;

    /**
     * Thread Safe
     */

    /** 
     * Debug
     */
    bool debug;
    bool debug_modbus;

    /**
     * Simulation
     */
    int  sim_status;
    int  sim_cnt;
    int  sim_time;
    int  sim_m_factor;
    int  sim_z_factor;
}DATA_ST;


/**
 * Estructura para la tabla de funciones de 
 * procesamiento de modbus
 */
typedef struct {
	const uint8_t id; 
	int (*proc)(MB_DATA_ST *); 
}MBFUNC_PROC_ST;

/**
 * Tipo de salida de slavemb
 */
enum {
    EXIT_CODE, 
    EXIT_WARM_CODE 
};

#endif
