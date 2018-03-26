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
/**/

#include "shell.h"
#include "shell_cmds.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
        case 's':
            shell->debug = on_off;
            rtn = RTN_CMD_OK;
            break;
        case 'm':
            d_st->mb->debug = on_off;
            rtn = RTN_CMD_OK;
            break;
    }
    if(rtn==RTN_CMD_OK)
        (*shell->printf)(CMD_OK_RESPONSE);
    return rtn;
}

/** 
 * Command to get version.
 *  It prints the shell's version.
 */
static RTN_CMD_PROC do_cmd_version(SHELL *shell)
{
    (*shell->printf)("%0.2d.%0.2d.%0.2d\r\n", 
            shell->version.major, shell->version.minor, 
            shell->version.build);
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
                    shell->argv[2]) != var_id)
                    //atoi(shell->argv[2])) != var_id)
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

    (*shell->printf)("%s\r\n",(char *) value);
    return RTN_CMD_OK;
}

/** 
 * Reset command. 
 *  It resets all variables.
 */
static RTN_CMD_PROC do_cmd_reset(SHELL *shell)
{
    memset(shell->var, 0, sizeof(SHELL_VARS) * NUM_OF_SHELL_VARS);
    (*shell->printf)(CMD_OK_RESPONSE);
    return RTN_CMD_OK;
}

/** 
 * Command quit.
 *  It quit from cmd shell and exit.
 */
static RTN_CMD_PROC do_cmd_quit(SHELL *shell)
{
    DATA_ST *d_st = (DATA_ST*) shell->data;
    d_st->quit = true;
    (*shell->printf)("Bye\n");
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
    {"help",    0, "Print help message", do_cmd_help },
    {"quit",    0, "Quit", do_cmd_quit },
    {NULL,0,NULL,NULL}
};
#define NUM_SHELL_CMDS (sizeof(shell_cmd_tbl)/sizeof(SHELL_CMD))

/** 
 * Help command. 
 *  It prints help message.
 */
static RTN_CMD_PROC do_cmd_help(SHELL *shell)
{
    const SHELL_CMD *shell_cmd;
    (*shell->printf)("Help menu:\r\n");
    (*shell->printf)("Shell:\r\n");
    for(shell_cmd = shell_cmd_tbl;
            (shell_cmd->help != NULL) ;shell_cmd++)
        (*shell->printf)("\t%s : %s\r\n", shell_cmd->cmd, shell_cmd->help);
    (*shell->printf)("App:\r\n");
    for(shell_cmd = shell_cmd_app_tbl;
            (shell_cmd->help != NULL) ;shell_cmd++)
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

static RTN_CMD_PROC do_shell_lookup_cmd(SHELL *shell, const SHELL_CMD tbl[])
{
    RTN_CMD_PROC rtn = RTN_CMD_NOTFOUND;
    const SHELL_CMD *shell_cmd;
    for(shell_cmd = tbl;
            (shell_cmd->help != NULL) && 
            (strcmp(shell_cmd->cmd, shell->argv[0]) != 0)
            ; shell_cmd++)
        /* Lookup Table */;

    if(shell_cmd->help != NULL)
    {
        if(shell_cmd->argc != shell->argc - 1)
            return fail_cmd(RTN_CMD_NUM_ARGS_INCORRECT, shell);

        if((rtn = (*shell_cmd->cmd_func)(shell)) != RTN_CMD_OK) 
            return fail_cmd(rtn, shell);
    }

    return rtn;
}

/**
 * This routine lookups into the command's definition table and them run it.
 */
RTN_CMD_PROC do_shell_cmd(SHELL *shell)
{
    RTN_CMD_PROC rtn;
    rtn = do_shell_lookup_cmd(shell, shell_cmd_tbl);
    if(rtn == RTN_CMD_NOTFOUND)
        rtn = do_shell_lookup_cmd(shell, shell_cmd_app_tbl);

    if(rtn == RTN_CMD_NOTFOUND)
        return fail_cmd(RTN_CMD_NOTFOUND, shell);
    return rtn;
}

