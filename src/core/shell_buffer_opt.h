/**
 * @file shell_buffer_opt.h
 *  Routines to perform action over shell buffer
 */

#ifndef __SHELL_BUFFER_OPT_H__
#define __SHELL_BUFFER_OPT_H__

#include "shell.h"

typedef enum {
    RTN_SEQ_CONTINUE,
    RTN_SEQ_NONE,
    RTN_SEQ_BREAK,
}RTN_SEQ;

typedef enum {
    RTN_SEQ_ACT_END,
    RTN_SEQ_ACT_CONTINUE,
}RTN_SEQ_ACT;

typedef RTN_SEQ_ACT (*SEQ_ACT)(SHELL *, int);
typedef struct {
    const char sc;
    const char esc;
    RTN_SEQ output;
    SEQ_ACT action;
}SHELL_SEQ_IT;

typedef struct {
    SHELL_SEQ_IT *seq;
    int level;
}SHELL_SEQ_ST;

RTN_SEQ shell_special_seq(SHELL *shell, int c);


#endif
