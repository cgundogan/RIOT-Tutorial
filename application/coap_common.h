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

#ifndef COAP_COMMON_H
#define COAP_COMMON_H

#include <coap.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_RESPONSE_LEN    500

extern uint8_t response[MAX_RESPONSE_LEN];
extern const coap_endpoint_t endpoints[];

int handle_get_well_known_core(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt,
                               coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo);

void *microcoap_server(void *);

#ifdef __cplusplus
}
#endif
#endif /* COAP_COMMON_H */
/** @} */
