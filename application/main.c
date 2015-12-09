/*
 * Copyright (C) 2015 Cenk Gündoğan <cnkgndgn@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @brief       Introductory example for the IEEE WF-IOT 2015 Tutorial
 * @author      Cenk Gündoğan <cnkgndgn@gmail.com>
 */

#include <stdio.h>
#include "shell.h"
#include "msg.h"

#define Q_SZ    (8)
static msg_t msg_q[Q_SZ];

/* Add the shell command function here ###################################### */

/* ########################################################################## */

static const shell_command_t shell_commands[] = {

/* Add a new shell command here ############################################# */

/* ########################################################################## */

    { NULL, NULL, NULL }
};

int main(void)
{
    msg_init_queue(msg_q, Q_SZ);

    puts("Type 'help' for a list of available commands");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* never reached */
    return 0;
}
