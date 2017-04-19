/**
 * @file mb_func.c
 * ModBUS functions implementation
 */
#include "mb_func.h"
#include "func_mb.h"
#include "mb_def.h"
#include "debug.h"

#include <stdlib.h>
#include <modbus.h>

MB_DATA_ST *init_mb(MB_CONF_ST *mb_conf)
{
    MB_DATA_ST  *mb_data = (MB_DATA_ST *)malloc(sizeof(MB_DATA_ST));
    mb_data->mb_query = (uint8_t*) malloc(MODBUS_RTU_MAX_ADU_LENGTH);

    mb_data->mb_conn = modbus_new_rtu(mb_conf->port_tty,
            mb_conf->port_baud,
            mb_conf->port_parity,
            mb_conf->port_ndbits,
            mb_conf->port_nsbits);
    modbus_set_slave(mb_data->mb_conn, mb_conf->id);

    modbus_set_debug(mb_data->mb_conn, mb_data->debug);
    modbus_set_error_recovery(mb_data->mb_conn,
            MODBUS_ERROR_RECOVERY_LINK |
            MODBUS_ERROR_RECOVERY_PROTOCOL);

    mb_data->mb_mapping = modbus_mapping_new(
            UT_BITS_NB_POINTS,
            UT_INPUT_BITS_NB_POINTS,
            UT_REGISTERS_NB_POINTS,
            UT_INPUT_REGISTERS_NB_POINTS);
    if(mb_data->mb_mapping == NULL)
    {
        free(mb_data);
        return NULL;
    }

    pthread_mutex_init(&mb_data->mutex,NULL);
    return mb_data;
}

void clean_mb(MB_DATA_ST *mb_data)
{
    if(mb_data == NULL)
        return;
    pthread_mutex_destroy(&mb_data->mutex);
    if(mb_data->mb_query)	    free(mb_data->mb_query);
    if(mb_data->mb_mapping)   modbus_mapping_free(mb_data->mb_mapping);
    if(mb_data->mb_conn) 	    modbus_free(mb_data->mb_conn);
    free(mb_data);
}

int connect_mb(MB_DATA_ST *mb_data)
{
    return modbus_connect(mb_data->mb_conn); 
}

int receive_mb(MB_DATA_ST *mb_data)
{
    return modbus_receive(mb_data->mb_conn, mb_data->mb_query);
}

int reply_mb(MB_DATA_ST *mb_data, int rc)
{
    if(rc > 0)
        return modbus_reply(mb_data->mb_conn, mb_data->mb_query, rc,
                mb_data->mb_mapping);
    return rc;
}

void rtu_loop_sever_mb(MB_DATA_ST *mb_data)
{
    int rc;
    while(1)
    {
        if((rc = receive_mb(mb_data)) == -1)
            continue;
        get_mbfunction(mb_data);
        reply_mb(mb_data, rc);
    }
}


/*
 * ModBUS processing 
 */

/**
 *  readDOCoil
 *  Read Discrete Output Coil
 *  TODO: Not implemented yet
 */
static int readDOCoil(MB_DATA_ST *mb_data)
{
  uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_ADDR);
  uint16_t value = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_SINGLE_VALUE);
  debug_printf(mb_data->debug, "NOT IMPLEMENTED %s [%d = %d]\n",__FUNCTION__, addr, value);
  return 0;
}

/**
 *  writeDOCoil
 *  Write Discrete Output Coil
 *  TODO: Not implemented yet
 */
static int writeDOCoil(MB_DATA_ST *mb_data)
{
  uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_ADDR);
  uint16_t value = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_SINGLE_VALUE);
  debug_printf(mb_data->debug, "NOT IMPLEMENTED %s [%d = %d]\n",__FUNCTION__, addr, value);
  return 0;
}

MB_READ_OHR_ST mb_confreadohr_tbl[]=
{
  {ANY_ADDRESS,         get_var      }
};
#define MB_READ_OHR_TBL_SIZE (sizeof(mb_confreadohr_tbl)/sizeof(MB_READ_OHR_ST))

/**
 *  readAOHReg
 *  Read Analog Holding Register
 */
static int readAOHReg(MB_DATA_ST *mb_data)
{
    int rtn = 0;
    MB_READ_OHR_ST *pmb_conf_readohr;
    uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_ADDR);
    //debug_printf(mb_data->debug, ">%s[%d]\n",__FUNCTION__, addr);
    pthread_mutex_lock(&(mb_data->mutex));
    for(pmb_conf_readohr = mb_confreadohr_tbl;
            (pmb_conf_readohr-mb_confreadohr_tbl) < MB_READ_OHR_TBL_SIZE; pmb_conf_readohr++ )
        if(pmb_conf_readohr->addr == addr || pmb_conf_readohr->addr == ANY_ADDRESS)
            rtn = (*pmb_conf_readohr->proc)(mb_data);
    pthread_mutex_unlock(&(mb_data->mutex));
    return rtn;
}

/**
 *  writeAOHReg
 *  Write Analog Holding Register
 */
static int writeAOHReg(MB_DATA_ST *mb_data)
{
    int rtn = 0;
    uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_ADDR);
    uint16_t value = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_SINGLE_VALUE);
    debug_printf(mb_data->debug, ">%s[%d = %d]\n",__FUNCTION__, addr, value);
    pthread_mutex_lock(&(mb_data->mutex));
    set_var(mb_data);
    pthread_mutex_unlock(&(mb_data->mutex));
    return rtn;
}

/**
 *  writeMAOHReg
 *  Write Multiple Analog Holding Registers
 */
static int writeAOMHReg(MB_DATA_ST *mb_data)
{
    uint16_t addr = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_ADDR);
    uint16_t value = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_SINGLE_VALUE);
    debug_printf(mb_data->debug, ">%s[%d = %d]\n",__FUNCTION__, addr, value);
    pthread_mutex_lock(&(mb_data->mutex));
    set_var(mb_data);
    pthread_mutex_unlock(&(mb_data->mutex));
    return 0;
}

/**
 *	Function Code	Action		      Name
 *	01 (01 hex)   Read		        Discrete Output Coils
 *	05 (05 hex)   Write single	    Discrete Output Coil
 *	15 (0F hex)   Write multiple	Discrete Output Coils
 *	02 (02 hex)   Read              Discrete Input Contacts
 *	04 (04 hex)   Read              Analog Input Registers
 *	03 (03 hex)   Read              Analog Output Holding Registers
 *	06 (06 hex)   Write single	    Analog Output Holding Register
 *	16 (10 hex)   Write multiple	Analog Output Holding Registers
 */
static MBFUNC_PROC_ST mbfunc_tbl[]=
{
  { 0x01, readDOCoil	  },
  { 0x05, writeDOCoil	  },
  { 0x0F, NULL		      },
  { 0x02, NULL		      },
  { 0x04, NULL		      },
  { 0x03, readAOHReg	  },
  { 0x06, writeAOHReg	  },
  { 0x10, writeAOMHReg	  }
};
#define MBFUNC_PROC_TBL_SIZE (sizeof(mbfunc_tbl)/sizeof(MBFUNC_PROC_ST))

/**
 * get_mbfunction
 * Look up table search routine for modBUS function processing
 */
int get_mbfunction(MB_DATA_ST *mb_data)
{
  MBFUNC_PROC_ST *pfproc;
  int header_length = modbus_get_header_length(mb_data->mb_conn);
  for(pfproc=mbfunc_tbl; (pfproc - mbfunc_tbl) < MBFUNC_PROC_TBL_SIZE; pfproc++)
    if(pfproc->id == mb_data->mb_query[header_length] && pfproc->proc != NULL)
      return (*pfproc->proc)(mb_data);
  return 0;
}





