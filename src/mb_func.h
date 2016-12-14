/**
 * @file mb_func.h
 * ModBUS functions headers
 */

#ifndef __MB_FUNC__
#define __MB_FUNC__

#include <modbus.h>
#include <pthread.h>
#include <stdbool.h>

/**
 * Default Modbus server id
 */
#define  SERVER_ID_DFT 		1


typedef struct {
    int id;

	char port_tty[20];
	int  port_baud;
	char port_parity;
	int  port_ndbits;
	int  port_nsbits;


} MB_CONF_ST;


typedef struct {
    modbus_t *mb_conn;
    modbus_mapping_t  *mb_mapping;
    uint8_t*  mb_query;
    MB_CONF_ST *conf;
    bool debug;
    pthread_mutex_t mutex;
}MB_DATA_ST;

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

/**
 * init modBUS data
 */
MB_DATA_ST *init_mb(MB_CONF_ST *mb_conf);

/**
 * Clean modBUS data
 */
void clean_mb(MB_DATA_ST *mb_data);

/**
 * modBUS connect
 */
int connect_mb(MB_DATA_ST *mb_data);

/**
 * get_mbfunction
 * Look up table search routine for modBUS function processing
 */
int get_mbfunction(MB_DATA_ST *mb_data);

#endif
