/**
 * @file shell_buffer_opt.c
 *  Routines to perform action over shell buffer
 */

#include "shell.h"
#include "shell_buffer_opt.h"

static RTN_SEQ_ACT seq_backspace(SHELL *shell, int c)
{
    if(shell->pwrite > shell->in_buffer)
        *shell->pwrite-- = '\0';
    (*shell->printf)("\b \b");
    return RTN_SEQ_ACT_END;
}

static RTN_SEQ_ACT seq_tab(SHELL *shell, int c)
{
    return RTN_SEQ_ACT_END;
}

const SHELL_SEQ_IT seq_tbl[]={
    {'\b', RTN_SEQ_CONTINUE, seq_backspace  },
    {'\t', RTN_SEQ_CONTINUE, seq_tab        },
};
#define NUM_SEQ_IT_ELM (sizeof(seq_tbl)/sizeof(SHELL_SEQ_IT))

RTN_SEQ shell_special_seq(SHELL *shell, int c)
{
    const SHELL_SEQ_IT *sit;
    for(sit=seq_tbl; sit - seq_tbl < NUM_SEQ_IT_ELM && 
            sit->sc != c; sit++);
    if(sit - seq_tbl < NUM_SEQ_IT_ELM)
    {
        (*sit->action)(shell, c);
        return RTN_SEQ_CONTINUE;
    }
    return RTN_SEQ_NONE;
}
