/**
 * @file shell_conf.h
 * @author Jorge Courett
 * @date 11/23/2016
 * @brief Command Shell configuration.
 *
 * This file is where it's possible to be done some parameterizations over 
 * the behavior of the command shell
 *
 */

#ifndef __SHELL_CONF_H__
#define __SHELL_CONF_H__

/**
 * Version definition
 */
#define V_MAJOR                 0       /*< Major version [0..99] */
#define V_MINOR                 1       /*< minor version [0..99] */
#define V_BUILD                 1       /*< build version [0..99] */

/**
 * Max buffer size
 * Its used for input and output buffers
 */
#define MAX_IN_BUFFER_SHELL_SIZE       10 
#define MAX_OUT_BUFFER_SHELL_SIZE      512 

/**
 * Max lenght of each argument. 
 * Including command as the first one.
 */
#define MAX_ARGS_LEN                64

/**
 * Max number of arguments. 
 * Including command as the first one.
 */
#define MAX_ARGS_CNT                10

/**
 * Definition of line terminator.
 */
#define SHELL_LINE_TERMINATOR       "\n"
#define SHELL_LINE_TERMINATOR_LEN   1 

/**
 * Definition of line separator. 
 */
#define SHELL_LINE_SEPARATOR        ' '

/**
 * Group character. It's used to group arguments 
 * containing line terminators.
 */
#define SHELL_LINE_ARGS_GROUP       '"' 

/**
 * Number of shell variables
 */
#define NUM_OF_SHELL_VARS           64 



#endif
