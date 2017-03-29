#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

#include "mb_def.h"
#include "mb_func.h"
#include "sim.h"
#include "log.h"

/**
 * Transactional data 
 */
typedef struct{
    bool quit;

    /**
     * ModBUS
     */
    MB_DATA_ST *mb;

    /** 
     * Global Debug
     */
    bool debug;

    /**
     * Log
     */
    int log_id;
    char *log_filename;

    /**
     * Simulation
     */
    SIM_CORE_ST sim;
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

/**
 * Log definitions
 */

#define APP_LOG 0
#define LOG_SIZE TSIZE_MB(5) 

#endif
