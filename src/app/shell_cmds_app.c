/**
 * @file shell_cmds_app.c
 * @author Jorge Courett
 * @date 12/14/2016
 * @brief Application command Shell definitions.
 *
 * Implementation commands of the shell interface for custom application.
 *
 */

/**
 * Implementation headers
 */
#include "common.h"
#include "utils.h"
#include "mb_map.h"
/**/

#include "shell.h"
#include "shell_cmds.h"
#include "sim.h"
#include "sim_app.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** 
 * Command to turn on
 */
static RTN_CMD_PROC do_cmd_turn(SHELL *shell)
{
    int rtn = RTN_CMD_FAIL;
    DATA_ST *d_st = (DATA_ST*) shell->data;
    SIM_CORE_ST *sim = &d_st->sim;
    SIM_DATA_ST *app = (SIM_DATA_ST*) sim->app;

    if((strcmp("on",shell->argv[1]) == 0))
    {
        valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(STATUS, 1));
        rtn = RTN_CMD_OK;
    }else if((strcmp("off",shell->argv[1]) == 0)) {
        valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(STATUS, 0));
        rtn = RTN_CMD_OK;
    }

    if(rtn==RTN_CMD_OK)
        (*shell->printf)(CMD_OK_RESPONSE);
    return rtn;
}

/** 
 * Command to set simulation parameters
 */
static RTN_CMD_PROC do_cmd_set_sim(SHELL *shell)
{
    int m, z;
    DATA_ST *d_st = (DATA_ST*) shell->data;
    SIM_CORE_ST *sim = &d_st->sim;
    SIM_DATA_ST *app = (SIM_DATA_ST*) sim->app;

    if(sscanf(shell->argv[1],"%d", &m) != 1)
        return RTN_CMD_FAIL;
    if(sscanf(shell->argv[2],"%d", &z) != 1)
        return RTN_CMD_FAIL;
    
    app->m_factor = m;
    app->z_factor = z;
    (*shell->printf)(CMD_OK_RESPONSE);
    return RTN_CMD_OK;
}

/** 
 * Reset command. 
 *  It resets all variables.
 */
static RTN_CMD_PROC do_cmd_reset_sim(SHELL *shell)
{
    DATA_ST *d_st = (DATA_ST*) shell->data;
    SIM_CORE_ST *sim = &d_st->sim;
    SIM_DATA_ST *app = (SIM_DATA_ST*) sim->app;

    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(PESO,   0));
    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(DESP,   0));
    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(NONE,   0));
    valueToMB(d_st->mb->mb_mapping->tab_registers, SET_MB_HR(STATUS, 0));
    (*shell->printf)(CMD_OK_RESPONSE);
    return RTN_CMD_OK;
}

/**
 * Get MB 
 */
static RTN_CMD_PROC do_cmd_get_mb(SHELL *shell)
{
    int addr,num,value,cnt;
    DATA_ST *d_st = (DATA_ST*) shell->data;
    SIM_CORE_ST *sim = &d_st->sim;
    SIM_DATA_ST *app = (SIM_DATA_ST*) sim->app;

    if(sscanf(shell->argv[1],"%d", &addr) != 1)
        return RTN_CMD_FAIL;
    if(sscanf(shell->argv[2],"%d", &num) != 1)
        return RTN_CMD_FAIL;

    cnt = 1;
    if(sscanf(shell->argv[3],"%d", &cnt) != 1)
            return RTN_CMD_FAIL;

    for(;cnt--;)
    {
        (*shell->printf)("Get ");
        value = MBToValue(d_st->mb->mb_mapping->tab_registers, addr, num);
        (*shell->printf)("[%04d] = %d\n", addr, value);
        addr += (num==16)?1:2;
    }
    return RTN_CMD_OK;
}

/**
 * Set MB 
 */
static RTN_CMD_PROC do_cmd_set_mb(SHELL *shell)
{
    int addr,num,value;
    DATA_ST *d_st = (DATA_ST*) shell->data;
    SIM_CORE_ST *sim = &d_st->sim;
    SIM_DATA_ST *app = (SIM_DATA_ST*) sim->app;

    if(sscanf(shell->argv[1],"%d", &addr) != 1)
        return RTN_CMD_FAIL;
    if(sscanf(shell->argv[2],"%d", &num) != 1)
        return RTN_CMD_FAIL;
    if(sscanf(shell->argv[3],"%d", &value) != 1)
        return RTN_CMD_FAIL;

    valueToMB(d_st->mb->mb_mapping->tab_registers, addr, value, num);
    (*shell->printf)("Set ");
    value = MBToValue(d_st->mb->mb_mapping->tab_registers, addr, num);
    (*shell->printf)("[%04d] = %d\n", addr, value);
    return RTN_CMD_OK;
}




const SHELL_CMD shell_cmd_app_tbl[]={
    {"resetsim",0, "Resets ALL variable values to zero.", do_cmd_reset_sim },
    {"turn",    1, "Turn on/off: turn [on|off]",          do_cmd_turn      },
    {"setsim",  2, "Set sim parameters.",                 do_cmd_set_sim   },

    {"get_mb",  3, "Get MB: get_mb <addr> 16|32 cnt",     do_cmd_get_mb    },
    {"set_mb",  3, "Set MB: set_mb <addr> 16|32 <value>", do_cmd_set_mb    },
    {NULL,0,NULL,NULL}
};

