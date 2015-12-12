/*
 * Copyright (C) 2015 Cenk Gündoğan <cnkgndgn@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @author  Cenk Gündoğan <cnkgndgn@gmail.com>
 */

#ifndef UDP_COMMON_H
#define UDP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

extern bool led_status;
void *udp_server(void *);
int udp_cmd(int argc, char **argv);

#ifdef __cplusplus
}
#endif
#endif /* UDP_COMMON_H */
/** @} */
