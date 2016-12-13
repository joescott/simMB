/**
 * @file shell.h
 * @author Jorge Courett
 * @date 11/23/2016
 * @brief Command  interface implementation.
 *
 * Implementation of a simple command interface for command processing.
 *
 */

#ifndef __SHELL_CMDS_H__
#define __SHELL_CMDS_H__

typedef RTN_CMD_PROC (*CMD_FUNC)(SHELL *);

/**
 * Commands data model
 */
typedef struct {
    char *cmd;          /**< Command string */
    unsigned int argc;  /**< Number of arguments expected */
    char *help;         /**< Help message */
    CMD_FUNC cmd_func;  /**< Pointer to function who implements command */
}SHELL_CMD;

/**
 * @brief loockup table function for commands
 * @param shell receive Shell data structure
 * @return Command output status
 */
RTN_CMD_PROC do_shell_cmd(SHELL *shell);


#endif


