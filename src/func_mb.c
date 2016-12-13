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
 * Init Output Holding Registers
 */
SLVMB_PROC_DEF(init_var)
{
    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(PESO,     0));
    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(DESP,     0));
    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(NONE,     0));
    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(STATUS,   0));
    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(PESO_MAX, 7000));
    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(INI_GRAF, 200));
    return 0;
}

/**
 * Read Output Holding Registers
 */
SLVMB_PROC_DEF(get_var)
{
    int addr, rnum;
    addr = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_ADDR);
    rnum = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_RNUM);
    debug_printf(d_st->debug_modbus,"GET_VAR: Addr[%d] Nreg:[%d]\n",addr, rnum);
    return 0;
}

/**
 * Write Output Holding Registers
 */
SLVMB_PROC_DEF(set_var)
{
    int addr, value;
    addr = MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_ADDR);
    value= MODBUS_GET_INT16_FROM_INT8(mb_query, MB_DATA_SINGLE_VALUE);
    debug_printf(d_st->debug_modbus, "SET_VAR: Addr[%d] Value:[%d]\n",addr, value);
    valueToMB(d_st->mb_mapping->tab_registers, addr, value, 16);
    return 0;
}
