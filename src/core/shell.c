/**
 * @file shell.c
 * @author Jorge Courett
 * @date 11/23/2016
 * @brief Command Shell interface implementation.
 *
 * Implementation of a simple command line interface.
 *
 */

#include "shell.h"
#include "shell_cmds.h"
#include "shell_buffer_opt.h"
#include "hist.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>

#define  SHELL_DEBUG
#ifdef SHELL_DEBUG
    #define SHELL_DEBUG_PRINT(fmt, ...) printf(fmt,__VA_ARGS__)

    static inline void shell_debug_print_buffer(SHELL *shell)
    {
        char *p;
        if(shell->debug)
        {
            printf("\nBUFFER[%d]\n", shell->pwrite - shell->in_buffer);
            for(p=shell->in_buffer; 
                    p - shell->in_buffer <  MAX_IN_BUFFER_SHELL_SIZE; p++)
                printf("|%02X", *p);
            printf("\n");
            for(p=shell->in_buffer; 
                    p - shell->in_buffer <  MAX_IN_BUFFER_SHELL_SIZE; p++)
            {
                if(*p)
                    printf("|%2c", *p);
                else
                    printf("|--");
            }
            printf("\n");
        }
    }
#else
    #define SHELL_DEBUG_PRINT(fmt, ...)
    #define shell_debug_print_buffer(x)
#endif

static struct termios old_tio, new_tio;

/** 
 * @brief Static data structure
 *  This data structure hold all methods and storages 
 *  needed to support shell functionality.  
 * @note Initialization compliant with C99 
 */
static SHELL shell = {
        .version = {V_MAJOR, V_MINOR, V_BUILD}
};

/**
 * @brief Default shell printf function 
 *  This function implement printf functionality using 
 *  defined method put_char_func to put chars on defined output channel.
 */
static void default_shell_printf(const char *fmt, ...)
{
    char *pc;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(shell.out_buffer,MAX_OUT_BUFFER_SHELL_SIZE, fmt, ap);
    va_end(ap);

    for(pc=shell.out_buffer; *pc != '\0'; pc++)
        (*shell.put_char_func)((int)*pc);
}

/**
 * Test if line terminator was reached.
 */
static int is_terminator_found(SHELL *shell)
{
   return (strcmp((shell->pwrite - SHELL_LINE_TERMINATOR_LEN), 
               SHELL_LINE_TERMINATOR) == 0);
}

/**
 * Test if line terminator was reached.
 */
static void add_cmd_elm(SHELL *shell, char *pr)
{
    strcpy(shell->argv[shell->argc-1], pr);
}

/**
 * Shell initialization routine. 
 */
SHELL *init_shell(GET_CHAR_SHELL_FUNC get_char_func, 
	PUT_CHAR_SHELL_FUNC put_char_func,
	void *data)
{
    shell.get_char_func = get_char_func;
    shell.put_char_func = put_char_func;
    shell.pwrite = shell.in_buffer;
    shell.printf = default_shell_printf;
	shell.data = data;
    memset(shell.var, 0, sizeof(SHELL_VARS) * NUM_OF_SHELL_VARS);

	/* get the terminal settings for stdin */
	tcgetattr(STDIN_FILENO,&old_tio);
	/* we want to keep the old setting to restore them a the end */
	new_tio=old_tio;
	/* disable canonical mode (buffered i/o) and local echo */
	new_tio.c_lflag &=(~ICANON & ~ECHO);

	/* set the new settings immediately */
	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

    inithist();

    return &shell;
}

/**
 * Shell close routine
 */
void close_shell(SHELL *shell)
{
	/* restore the former settings */
	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
}

/**
 * Shell read line routine. This function perform readings using 
 * get_char_func methold of shell model.
 */
RTN_SLINE_READ read_line(SHELL *shell)
{
    int c;
    RTN_SEQ rseq;
    RTN_SLINE_READ rtn = RTN_SLINE_READ_FAIL;
    while((c = (*shell->get_char_func)()) != 0)
    {
        if(shell->debug)
            SHELL_DEBUG_PRINT("\nInput: |%c|%02X| ",isprint(c)?(char)c:'-', c);

        if(c == EOF)
            continue;

        if((rseq = shell_special_seq(shell, c)) == RTN_SEQ_CONTINUE)
            continue;
        else if(rseq == RTN_SEQ_BREAK)
            break;

        if(shell->pwrite - shell->in_buffer >= MAX_IN_BUFFER_SHELL_SIZE)
        {
            shell->pwrite = shell->in_buffer;
            rtn = RTN_SLINE_READ_EXCEEDED;
            break; 
        }

        *shell->pwrite++ = c;
        *shell->pwrite = '\0';

        shell_debug_print_buffer(shell);

        (*shell->put_char_func)((int)c);

        if(shell->pwrite - shell->in_buffer > SHELL_LINE_TERMINATOR_LEN && 
                is_terminator_found(shell))
        {
            *(shell->pwrite - SHELL_LINE_TERMINATOR_LEN) = '\0';
            shell->pwrite = shell->in_buffer;
            rtn = RTN_SLINE_READ_OK;
            break; 
        }
    }
   
    return rtn;
}

/**
 * Shell processing line routine. This function perform processing of 
 * line readed with read_line function.
 * Processing have 3 steps:
 *     -# Looking for first character non equal to SHELL_LINE_SEPARATOR.
 *        This help to skip beginning spaces. 
 *     -# Looking for separators and group characters and replace them with nulls.
 *     -# Determination of each argument.
 *
 */
RTN_CMD_PROC proc_line(SHELL *shell)
{
    char *pc;
    char *pr;
    int lidx, start, sep, group;
    addtohist(shell->in_buffer);
    for(shell->argc = lidx = 0, start=0, sep = 0, group = 0, 
            pr = pc = shell->in_buffer; *pc != '\0';pc++)
    {
        if(*pc == SHELL_LINE_ARGS_GROUP) 
        {
            *pc = '\0';
            group =  !group;

        }else if(*pc != SHELL_LINE_SEPARATOR) {
            start = 1;
            if(sep == 1) 
                pr = pc; 
            sep = 0;
        }else if(*pc == SHELL_LINE_SEPARATOR) {

            if(group)
                continue;

            sep = 1;
            *pc = '\0';

            if(shell->argc != lidx)
                add_cmd_elm(shell, pr);

            lidx = shell->argc;
        }
            
        if(start && (pc == shell->in_buffer || *(pc-1) == '\0') && *pc != '\0')
            if(shell->argc++ >= MAX_ARGS_CNT)
                return RTN_CMD_MAX_ARGS;
    }
    add_cmd_elm(shell, pr);
    return do_shell_cmd(shell);
}

/**
 * This function perform setting of a shell variable addresed by id.  
 */
int set_shell_var(SHELL *shell, int id, SHELL_VAR_TYPE value)
{
    if(id <= 0 || id > NUM_OF_SHELL_VARS)
        return SV_ERR_CODE_WRONG_ID;

    shell->var[id].value = value;
    shell->var[id].status = 1;

    return id;
}

/**
 * This function get value of a shell variable from provided id.
 */
int get_shell_var(SHELL *shell, int id, SHELL_VAR_TYPE *value)
{
    if(id <= 0 || id > NUM_OF_SHELL_VARS)
        return SV_ERR_CODE_WRONG_ID;

    if(shell->var[id].status == 0)
        return SV_ERR_CODE_VAR_NOT_SET;

    *value = shell->var[id].value;
    return id;
}

/**
 * This function reset value of a shell variable from provided id or 
 *  all if id = 0.
 */
int reset_shell_var(SHELL *shell, int id)
{
    if(id < 0 || id > NUM_OF_SHELL_VARS)
        return SV_ERR_CODE_WRONG_ID;

    if(id == 0)
        memset(shell->var, 0, sizeof(SHELL_VARS) * NUM_OF_SHELL_VARS);
    else
        memset(&shell->var[id], 0, sizeof(SHELL_VARS));

    return id;
}
