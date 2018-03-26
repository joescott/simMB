/**
 * @file func_mb.c
 *  Implementation of ModBus routines
 */

#include <math.h>
#include <modbus.h>

#include "func_mb.h"
#include "debug.h"
#include "log.h"
#include "utils.h"

/**
 * Read Output Holding Registers
 */
MB_PROC_DEF(get_var)
{
    int addr, rnum, value;
    addr  = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_ADDR);
    rnum  = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_RNUM);
    sappend_logfile(APP_LOG,"GET_VAR: Addr[%4d] Nreg:[%5d]",addr,rnum);
    for(;rnum > 0;rnum--,addr++)
    {
        value = MBToValue(mb_data->mb_mapping->tab_registers, addr, 16);
        sappend_logfile(APP_LOG,"         Addr[%4d] = Value[%5d]", addr, value);
    }
    return 0;
}

/**
 * Write Output Holding Registers
 */
MB_PROC_DEF(set_var)
{
    int addr, value;
    addr = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_ADDR);
    value= MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_SINGLE_VALUE);
    sappend_logfile(APP_LOG,"SET_VAR: Addr[%4d] Value:[%5d]",addr, value);
    valueToMB(mb_data->mb_mapping->tab_registers, addr, value, 16);
    return 0;
}
