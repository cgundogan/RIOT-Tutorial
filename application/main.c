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
#include <stdbool.h>
#include <string.h>
#include "board.h"

#define Q_SZ    (8)
static msg_t msg_q[Q_SZ];
static bool led_status = false;

/* Add the shell command function here ###################################### */

int led_control(int argc, char **argv)
{
    if (argc == 2) {
        if (strcmp(argv[1], "on") == 0) {
            led_status = true;
            LED_ON;
            return 0;
        }
        else if (strcmp(argv[1], "off") == 0) {
            led_status = false;
            LED_OFF;
            return 0;
        }
    }

    printf("Usage: %s [on|off]\n", argv[0]);

    return -1;
}

/* ########################################################################## */

static const shell_command_t shell_commands[] = {

/* Add a new shell command here ############################################# */

    { "led", "use 'led on' to turn the LED on and 'led off' to turn the LED off", led_control },

/* ########################################################################## */

    { NULL, NULL, NULL }
};

int main(void)
{
    msg_init_queue(msg_q, Q_SZ);

    puts("Type 'help' for a list of available commands");

    LED_OFF;

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* never reached */
    return 0;
}
