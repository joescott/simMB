/**
 * @file shell_cmds.c
 * @author Jorge Courett
 * @date 11/23/2016
 * @brief Command Shell definitions.
 *
 * Implementation commands of the shell interface.
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** 
 * OK generic response
 */
#define CMD_OK_RESPONSE     "OK\r\n"

/** 
 * Fail generic response
 */
#define CMD_FAIL_RESPONSE   "FAIL\r\n"

/* 
 * Command's routines 
 */

/** 
 * Command to turn on
 */
static RTN_CMD_PROC do_cmd_turn(SHELL *shell)
{
    int rtn = RTN_CMD_FAIL;
    DATA_ST *d_st = (DATA_ST*) shell->data;

    if((strcmp("on",shell->argv[1]) == 0))
    {
        valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(STATUS, 1));
        rtn = RTN_CMD_OK;
    }else if((strcmp("off",shell->argv[1]) == 0)) {
        valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(STATUS, 0));
        rtn = RTN_CMD_OK;
    }

    if(rtn==RTN_CMD_OK)
        (*shell->printf)(CMD_OK_RESPONSE);
    return rtn;
}

/** 
 * Command to set debug mode
 */
static RTN_CMD_PROC do_cmd_debug(SHELL *shell)
{
    char type;
    int on_off;
    int rtn = RTN_CMD_FAIL;
    DATA_ST *d_st = (DATA_ST*) shell->data;

    if(sscanf(shell->argv[1],"%c", &type) != 1)
        return RTN_CMD_FAIL;
    if(sscanf(shell->argv[2],"%d", &on_off) != 1)
        return RTN_CMD_FAIL;
    switch(type){
        case 'g':
            d_st->debug = on_off;
            rtn = RTN_CMD_OK;
            break;
        case 'm':
            d_st->debug_modbus = on_off;
            rtn = RTN_CMD_OK;
            break;
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

    if(sscanf(shell->argv[1],"%d", &m) != 1)
        return RTN_CMD_FAIL;
    if(sscanf(shell->argv[2],"%d", &z) != 1)
        return RTN_CMD_FAIL;
    
    d_st->sim_m_factor = m;
    d_st->sim_z_factor = z;
    (*shell->printf)(CMD_OK_RESPONSE);
    return RTN_CMD_OK;
}

static RTN_CMD_PROC do_cmd_print_mb(SHELL *shell)
{

    DATA_ST *d_st = (DATA_ST*) shell->data;
    int value, addr;
    (*shell->printf)("Print MB values\n");
    addr = GET_MB_HR_ADDR(PESO);
    value = MBToValue(d_st->mb_mapping->tab_registers, GET_MB_HR(STATUS));
    (*shell->printf)("[%04d] = %d\n", addr, value);
    addr = GET_MB_HR_ADDR(DESP);
    value = MBToValue(d_st->mb_mapping->tab_registers, GET_MB_HR(DESP));
    (*shell->printf)("[%04d] = %d\n", addr, value);
    addr = GET_MB_HR_ADDR(STATUS);
    value = MBToValue(d_st->mb_mapping->tab_registers, GET_MB_HR(STATUS));
    (*shell->printf)("[%04d] = %d\n", addr, value);
    addr = GET_MB_HR_ADDR(PESO_MAX);
    value = MBToValue(d_st->mb_mapping->tab_registers, GET_MB_HR(PESO_MAX));
    (*shell->printf)("[%04d] = %d\n", addr, value);
    addr = GET_MB_HR_ADDR(INI_GRAF);
    value = MBToValue(d_st->mb_mapping->tab_registers, GET_MB_HR(INI_GRAF));
    (*shell->printf)("[%04d] = %d\n", addr, value);
    return RTN_CMD_OK;
}

/** 
 * Command to get version.
 *  It prints the shell's version.
 */
static RTN_CMD_PROC do_cmd_version(SHELL *shell)
{
    (*shell->printf)("%0.2d.%0.2d.%0.2d\r\n", 
            shell->version.major, shell->version.minor, shell->version.build);
    return RTN_CMD_OK;
}

/** 
 * Echo command.
 * It prints supplied string as variable.
 */
static RTN_CMD_PROC do_cmd_echo(SHELL *shell)
{
    (*shell->printf)("%s\r\n", shell->argv[1]);
    return RTN_CMD_OK;
}

/** 
 * Command for setting variables. 
 *  It sets variable value. 
 */
static RTN_CMD_PROC do_cmd_set(SHELL *shell)
{
    int var_id;

    if(sscanf(shell->argv[1],"V%d", &var_id) != 1)
        return RTN_CMD_FAIL;

    if(set_shell_var(shell, var_id, 
                    atoi(shell->argv[2])) != var_id)
        return RTN_CMD_FAIL;

    (*shell->printf)("%s\r\n", shell->argv[2]);
    return RTN_CMD_OK;
}

/** 
 * Command for getting variables.
 *  It gets value of requested variable.
 */
static RTN_CMD_PROC do_cmd_get(SHELL *shell)
{
    int var_id;
    SHELL_VAR_TYPE value; 

    if(sscanf(shell->argv[1],"V%d", &var_id) != 1)
        return RTN_CMD_FAIL;

    if(get_shell_var(shell, var_id, &value) != var_id)
        return RTN_CMD_FAIL;

    (*shell->printf)("%d\r\n",(int) value);
    return RTN_CMD_OK;
}

/** 
 * Reset command. 
 *  It resets all variables.
 */
static RTN_CMD_PROC do_cmd_reset(SHELL *shell)
{
    DATA_ST *d_st = (DATA_ST*) shell->data;

    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(PESO,   0));
    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(DESP,   0));
    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(NONE,   0));
    valueToMB(d_st->mb_mapping->tab_registers, SET_MB_HR(STATUS, 0));
    memset(shell->var, 0, sizeof(SHELL_VARS) * NUM_OF_SHELL_VARS);
    (*shell->printf)(CMD_OK_RESPONSE);
    return RTN_CMD_OK;
}

/** 
 * Help command function definition.
 */
static RTN_CMD_PROC do_cmd_help(SHELL *shell);

/**
 * Command's definition table.
 */
const SHELL_CMD shell_cmd_tbl[]={
    {"version", 0, "Return the application version number.", do_cmd_version },
    {"echo",    1, "Return the argument sended.", do_cmd_echo },
    {"set",     2, "Sets a variable to the supplied 16-bit signed-int value.", do_cmd_set },
    {"get",     1, "Gets the value of the specified variable.", do_cmd_get },
    {"reset",   0, "Resets ALL variable values to zero.", do_cmd_reset },
    {"debug",   2, "Set debug modes.", do_cmd_debug },
    {"turn",    1, "Turn on/off: turn [on|off]", do_cmd_turn },
    {"setsim",  2, "Set sim parameters.", do_cmd_set_sim },
    {"print",   0, "Print Variables.", do_cmd_print_mb },
    {"help",    0, "Prinf help message", do_cmd_help },
};
#define NUM_SHELL_CMDS (sizeof(shell_cmd_tbl)/sizeof(SHELL_CMD))

/** 
 * Help command. 
 *  It prints help message.
 */
static RTN_CMD_PROC do_cmd_help(SHELL *shell)
{
    const SHELL_CMD *shell_cmd;
    (*shell->printf)("Shell Help:\r\n");
    for(shell_cmd = shell_cmd_tbl;
            (shell_cmd - shell_cmd_tbl < NUM_SHELL_CMDS) ;shell_cmd++)
        (*shell->printf)("\t%s : %s\r\n", shell_cmd->cmd, shell_cmd->help);
    return RTN_CMD_OK;
}

/**
 * Command failure routine 
 * TODO: Every type of failure could have custom response.
 */
static RTN_CMD_PROC fail_cmd(RTN_CMD_PROC proc, SHELL *shell)
{
    (*shell->printf)("%s", CMD_FAIL_RESPONSE);
    return proc;
}

/**
 * This routine lookups into the command's definition table and them run it.
 */
RTN_CMD_PROC do_shell_cmd(SHELL *shell)
{
    RTN_CMD_PROC rtn;
    const SHELL_CMD *shell_cmd;
    for(shell_cmd = shell_cmd_tbl;
            (shell_cmd - shell_cmd_tbl < NUM_SHELL_CMDS) && 
            (strcmp(shell_cmd->cmd, shell->argv[0]) != 0)
            ; shell_cmd++)
        /* Lookup Table */;

    if((shell_cmd - shell_cmd_tbl >= NUM_SHELL_CMDS))
        return fail_cmd(RTN_CMD_NOTFOUND, shell);

    if(shell_cmd->argc != shell->argc - 1)
        return fail_cmd(RTN_CMD_NUM_ARGS_INCORRECT, shell);

    if((rtn = (*shell_cmd->cmd_func)(shell)) != RTN_CMD_OK) 
        return fail_cmd(rtn, shell);

    return rtn;
}

