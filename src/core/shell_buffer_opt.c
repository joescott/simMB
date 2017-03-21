/**
 * @file shell_buffer_opt.c
 *  Routines to perform action over shell buffer
 */

#include "shell.h"
#include "hist.h"
#include "shell_buffer_opt.h"

#include <stdlib.h>

static RTN_SEQ_ACT seq_clear_line(SHELL *shell, int c)
{
    (*shell->printf)("%c[2K\r", 27);
    return RTN_SEQ_ACT_END;
}

static RTN_SEQ_ACT seq_backspace(SHELL *shell, int c)
{
    if(shell->pwrite > shell->in_buffer)
    {
        *shell->pwrite-- = '\0';
        (*shell->printf)("\b \b");
    }
    return RTN_SEQ_ACT_END;
}

static RTN_SEQ_ACT seq_tab(SHELL *shell, int c)
{
    return RTN_SEQ_ACT_END;
}

static void rewrite_buffer(SHELL *shell, char *buff)
{
    char *pc;
    while(shell->pwrite > shell->in_buffer)
        *shell->pwrite-- = '\0';

    for(pc=buff;*pc!='\0';pc++)
        *shell->pwrite++ = *pc;
}

static RTN_SEQ_ACT seq_ext_up(SHELL *shell, int c)
{
    char *buff = NULL;

    buff = upfromhist();

    if(!buff)
        return RTN_SEQ_ACT_END;

    rewrite_buffer(shell, buff);
    seq_clear_line(shell, c);
    print_shell_prompt(shell);
    (*shell->printf)(buff);

    return RTN_SEQ_ACT_END;
}

static RTN_SEQ_ACT seq_ext_down(SHELL *shell, int c)
{
    char *buff = NULL;

    buff = downfromhist();

    if(!buff)
        return RTN_SEQ_ACT_END;

    rewrite_buffer(shell, buff);
    seq_clear_line(shell, c);
    print_shell_prompt(shell);
    (*shell->printf)(buff);

    return RTN_SEQ_ACT_END;
}

static RTN_SEQ_ACT seq_ext_esp(SHELL *shell, int c)
{
    rewrite_buffer(shell, "");
    seq_clear_line(shell, c);

    return RTN_SEQ_ACT_END;
}

const SHELL_SEQ_IT seq_tbl[]={
    {'\b', 0,    RTN_SEQ_CONTINUE, seq_backspace },
    {'\t', 0,    RTN_SEQ_CONTINUE, seq_tab       },
    {0x5B, 0x41, RTN_SEQ_CONTINUE, seq_ext_up    },
    {0x5B, 0x42, RTN_SEQ_CONTINUE, seq_ext_down  },
    {0x15, 0,    RTN_SEQ_CONTINUE, seq_ext_esp   },
};
#define NUM_SEQ_IT_ELM (sizeof(seq_tbl)/sizeof(SHELL_SEQ_IT))

RTN_SEQ shell_special_seq(SHELL *shell, int c)
{
    char *ce;
    const SHELL_SEQ_IT *sit;
    for(sit=seq_tbl; sit - seq_tbl < NUM_SEQ_IT_ELM && 
            sit->sc != c; sit++);
    if(sit - seq_tbl < NUM_SEQ_IT_ELM)
    {
        if(sit->esc)
        {
            ce = (*shell->get_char_func)();
            if(sit->esc == ce)
            {
                (*sit->action)(shell, c);
                return RTN_SEQ_CONTINUE;
            }
        }else{
            (*sit->action)(shell, c);
            return RTN_SEQ_CONTINUE;
        }
    }
    return RTN_SEQ_NONE;
}
