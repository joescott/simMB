/**
 * @file shell.h
 * @author Jorge Courett
 * @date 11/26/2016
 * @brief Command Shell interface.
 *
 * This file implement a simple command line interface.
 *
 */

#ifndef __SHELL_H__
#define __SHELL_H__

#include "shell_conf.h"

#include <stdint.h>
#include <stdbool.h>

/** 
 * Read line return codes
 */
typedef enum{ 
    RTN_SLINE_READ_OK,          /**< Read OK*/
    RTN_SLINE_READ_EXCEEDED,    /**< Line was exceeded in input characters*/
    RTN_SLINE_READ_FAIL         /**< Failure read*/
} RTN_SLINE_READ; 

/** 
 * Commands return codes
 */
typedef enum{ 
    RTN_CMD_OK,                 /**< Command Ok*/
    RTN_CMD_NOTFOUND,           /**< Command not found*/
    RTN_CMD_MAX_ARGS,           /**< Max number of arguments reached*/
    RTN_CMD_NUM_ARGS_INCORRECT, /**< Incorrect number of arguments provided*/
    RTN_CMD_FAIL,               /**< Command fail*/
} RTN_CMD_PROC; 

/**
 * Shell variables error code definitions
 */
typedef enum {
    SV_ERR_CODE_WRONG_ID,       /**< Wrong id selection */
    SV_ERR_CODE_VAR_NOT_SET,    /**< Variable isn't set */
}SV_ERR_CODE;

/**
 *  Get char methold format definition 
 */
typedef int (*GET_CHAR_SHELL_FUNC)(void);

/**
 * Put char methold format definition 
 */
typedef int (*PUT_CHAR_SHELL_FUNC)(int);

/**
 * Printf methold format definition 
 */
typedef void (*PRINTF_SHELL_FUNC)(const char *,...);

/**
 * Shell Variable type 
 */
typedef int16_t SHELL_VAR_TYPE;

/**
 * Shell Variables model
 */
typedef struct {
    SHELL_VAR_TYPE value;
    unsigned int  status;
}SHELL_VARS;

/**
 * Defines the data-type to hold version information.
 */
typedef struct t_version
{
    uint8_t major;    /**< version major number   */
    uint8_t minor;    /**< version minor number   */
    uint8_t build;    /**< version build number   */

}SHELL_VERSION;

/** 
 * @brief Shell model
 */
typedef struct {
    char in_buffer[MAX_BUFFER_SHELL_SIZE];  /**< Input command Line buffer. */
    char out_buffer[MAX_BUFFER_SHELL_SIZE]; /**< Input command Line buffer. */
    char *pwrite;                           /**< Write pointer. */

    char argv[MAX_ARGS_CNT][MAX_ARGS_LEN];  /**< Command arguments. */
    int  argc;                              /**< Number of arguments counter. */

    GET_CHAR_SHELL_FUNC get_char_func;      /**< Get char function pointer. */
    PUT_CHAR_SHELL_FUNC put_char_func;      /**< Put char function pointer. */
    PRINTF_SHELL_FUNC printf;               /**< Printf function pointer. */

    SHELL_VARS var[NUM_OF_SHELL_VARS];      /**< Shell variables */

    SHELL_VERSION version;                  /**< Version */
    void *data;                             /**< General data pointer */
    bool debug;                             /**< Debug activation */
} SHELL;

/** 
 * @brief Shell initialization  rutine
 * @param get_char_func It's the get char function implementation to be used by shell. 
 * @param put_char_func It's the put char function implementation to be used by shell. 
 * @param data It's a pointer to data usefull into cmd actions. 
 * @return shell data structure
 */
SHELL *init_shell(GET_CHAR_SHELL_FUNC get_char_func, PUT_CHAR_SHELL_FUNC put_char_funci, void *data);

/**
 * @brief Close Shell 
 * @param shell It's Shell data structure
 */
void close_shell(SHELL *shell);
/** 
 * @brief Read line rutine
 * @param shell It's Shell data structure
 * @return Read line status
 */
RTN_SLINE_READ read_line(SHELL *shell);

/** 
 * @brief Processing line rutine
 * @param shell It's Shell data structure
 * @return Command status
 */
RTN_CMD_PROC proc_line(SHELL *shell);

/**
 * @brief Set shell variable
 * @param shell It 's Shell data structure.
 * @param id Variable id
 * @param value Variable value
 * @return Variable id or - SV_ERR_CODE
 */
int set_shell_var(SHELL *shell, int id, SHELL_VAR_TYPE value);

/**
 * @brief Get shell variable
 * @param shell It 's Shell data structure.
 * @param id Variable id
 * @param value return variable value
 * @return Variable id or - SV_ERR_CODE
 */
int get_shell_var(SHELL *shell ,int id, SHELL_VAR_TYPE *value);

/**
 * @brief Reset shell variable
 * @param shell It 's Shell data structure.
 * @param id Variable id to by reseted or 0 for all.
 * @return Variable id or - SV_ERR_CODE
 */
int reset_shell_var(SHELL *shell, int id);

#endif
