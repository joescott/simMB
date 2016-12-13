#ifndef __SLAVEMBHMI_H__
#define __SLAVEMBHMI_H__

#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <modbus.h>

#include "mbhmi.h"

#define  SERVER_ID_DFT 		1

/**
 * Estructura de datos 
 * slavemb
 */
typedef struct {
    int id;

	char port_tty[20];
	int  port_baud;
	char port_parity;
	int  port_ndbits;
	int  port_nsbits;

} SLVMB_ST;

/**
 * Estructura de datos para la 
 * configuracion del slavemb
 */
typedef struct {
	char *tag;
	char *value;
}PROPERTIE_ST;


/**
 * Transactional data 
 */
typedef struct{
    /**
     * ModBUS
     */
    modbus_t *mb_conn;
    modbus_mapping_t  *mb_mapping;

    /**
     * Thread Safe
     */
    pthread_mutex_t mutex;

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
	int (*proc)(DATA_ST *, uint8_t*); 
}MBFUNC_PROC_ST;

/**
 * Tipo de salida de slavemb
 */
enum {EXIT_SLVMB, WARM_SLVMB };

/**
 * The Slave Address
 * The Function Code 16
 * The Data Address of the first register
 * The number of registers to write 
 * The number of data bytes to follow
 * The value to write to register
 * C6F0: The CRC (cyclic redundancy check) for error checking.
 */
enum{
	MB_SLV_ADDR, 
	MB_FUNC_CODE,
	MB_DATA_ADDR,
	MB_DATA_ADDRH = MB_DATA_ADDR,
	MB_DATA_ADDRL,
	MB_DATA_RNUM, 
	MB_DATA_SINGLE_VALUE = MB_DATA_RNUM, 
	MB_DATA_RNUMH = MB_DATA_RNUM,
	MB_DATA_RNUML,
	MB_DATA_BYTES,
	MB_DATA_REG
};
#define MB_DATA_REG(x) (MB_DATA_REG + (x*2))
#define SLVMB_ARGC(x)  (x+1)  /* NUM_VARS + FUNC */

#endif
