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

static RTN_CMD_PROC do_cmd_print_mb(SHELL *shell)
{

    DATA_ST *d_st = (DATA_ST*) shell->data;
    SIM_CORE_ST *sim = &d_st->sim;
    SIM_DATA_ST *app = (SIM_DATA_ST*) sim->app;

    int value, addr;
    (*shell->printf)("Print MB values\n");
    addr = GET_MB_HR_ADDR(PESO);
    value = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(STATUS));
    (*shell->printf)("[%04d] = %d\n", addr, value);
    addr = GET_MB_HR_ADDR(DESP);
    value = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(DESP));
    (*shell->printf)("[%04d] = %d\n", addr, value);
    addr = GET_MB_HR_ADDR(STATUS);
    value = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(STATUS));
    (*shell->printf)("[%04d] = %d\n", addr, value);
    addr = GET_MB_HR_ADDR(PESO_MAX);
    value = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(PESO_MAX));
    (*shell->printf)("[%04d] = %d\n", addr, value);
    addr = GET_MB_HR_ADDR(INI_GRAF);
    value = MBToValue(d_st->mb->mb_mapping->tab_registers, GET_MB_HR(INI_GRAF));
    (*shell->printf)("[%04d] = %d\n", addr, value);
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

const SHELL_CMD shell_cmd_app_tbl[]={
    {"resetsim",0, "Resets ALL variable values to zero.", do_cmd_reset_sim },
    {"turn",    1, "Turn on/off: turn [on|off]", do_cmd_turn },
    {"setsim",  2, "Set sim parameters.", do_cmd_set_sim },
    {"print",   0, "Print Variables.", do_cmd_print_mb },
    {NULL,0,NULL,NULL}
};

