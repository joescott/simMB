/**
 * @file func_mb.c
 *  Implementation of ModBus routines
 */

#include <math.h>
#include <modbus.h>

#include "mb_map.h"
#include "func_mb.h"
#include "debug.h"
#include "utils.h"

/**
 * Read Output Holding Registers
 */
MB_PROC_DEF(get_var)
{
    int addr, rnum;
    addr = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_ADDR);
    rnum = MODBUS_GET_INT16_FROM_INT8(mb_data->mb_query, MB_DATA_RNUM);
    debug_printf(mb_data->debug,"GET_VAR: Addr[%4d] Nreg:[%5d]\n",addr, rnum);
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
    debug_printf(mb_data->debug, "SET_VAR: Addr[%4d] Value:[%5d]\n",addr, value);
    valueToMB(mb_data->mb_mapping->tab_registers, addr, value, 16);
    return 0;
}
