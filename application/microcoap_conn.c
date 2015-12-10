/*
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "net/af.h"
#include "net/conn/udp.h"

#ifdef MICROCOAP_DEBUG
#define ENABLE_DEBUG (1)
#else
#define ENABLE_DEBUG (0)
#endif
#include "debug.h"

#include <coap.h>
#include "coap_common.h"

static uint8_t _udp_buf[512];   /* udp read buffer (max udp payload size) */
uint8_t scratch_raw[1024];      /* microcoap scratch buffer */

coap_rw_buffer_t scratch_buf = { scratch_raw, sizeof(scratch_raw) };

#define COAP_SERVER_PORT    (5683)

#define MCOAP_Q_SZ  (8)
static msg_t msg_q[MCOAP_Q_SZ];

/*
 * Starts a blocking and never-returning loop dispatching CoAP requests.
 *
 * When using gnrc, make sure the calling thread has an initialized msg queue.
 */
void microcoap_server_loop(void)
{
    uint8_t laddr[16] = { 0 };
    uint8_t raddr[16] = { 0 };
    size_t raddr_len;
    uint16_t rport;

    conn_udp_t conn;

    int rc = conn_udp_create(&conn, laddr, sizeof(laddr), AF_INET6, COAP_SERVER_PORT);

    while (1) {
        DEBUG("Waiting for incoming UDP packet...\n");
        rc = conn_udp_recvfrom(&conn, (char *)_udp_buf, sizeof(_udp_buf), raddr, &raddr_len, &rport);
        if (rc < 0) {
            DEBUG("Error in conn_udp_recvfrom(). rc=%u\n", rc);
            continue;
        }

        size_t n = rc;

        coap_packet_t pkt;
        DEBUG("Received packet: ");
        coap_dump(_udp_buf, n, true);
        DEBUG("\n");

        /* parse UDP packet to CoAP */
        if (0 != (rc = coap_parse(&pkt, _udp_buf, n))) {
            DEBUG("Bad packet rc=%d\n", rc);
        }
        else {
            coap_packet_t rsppkt;
            DEBUG("content:\n");
            coap_dumpPacket(&pkt);

            /* handle CoAP request */
            coap_handle_req(&scratch_buf, &pkt, &rsppkt);

            /* build reply */
            size_t rsplen = sizeof(_udp_buf);
            if ((rc = coap_build(_udp_buf, &rsplen, &rsppkt)) != 0) {
                DEBUG("coap_build failed rc=%d\n", rc);
            }
            else {
                DEBUG("Sending packet: ");
                coap_dump(_udp_buf, rsplen, true);
                DEBUG("\n");
                DEBUG("content:\n");
                coap_dumpPacket(&rsppkt);

                /* send reply via UDP */
                rc = conn_udp_sendto(_udp_buf, rsplen, NULL, 0, raddr, raddr_len, AF_INET6, COAP_SERVER_PORT, rport);
                if (rc < 0) {
                    DEBUG("Error sending CoAP reply via udp; %u\n", rc);
                }
            }
        }
    }
}

/*
 * trampoline for microcoap_server_loop()
 */
void *microcoap_server(void *arg)
{
    (void) arg;
    msg_init_queue(msg_q, MCOAP_Q_SZ);

    microcoap_server_loop();

    /* never reached */
    return NULL;
}

int handle_get_well_known_core(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt,
                               coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char *rsp = (char *)response;
    int len = sizeof(response);
    const coap_endpoint_t *ep = endpoints;
    int i;

    len--; // Null-terminated string

    while (NULL != ep->handler) {
        if (NULL == ep->core_attr) {
            ep++;
            continue;
        }

        if (0 < strlen(rsp)) {
            strncat(rsp, ",", len);
            len--;
        }

        strncat(rsp, "<", len);
        len--;

        for (i = 0; i < ep->path->count; i++) {
            strncat(rsp, "/", len);
            len--;

            strncat(rsp, ep->path->elems[i], len);
            len -= strlen(ep->path->elems[i]);
        }

        strncat(rsp, ">;", len);
        len -= 2;

        strncat(rsp, ep->core_attr, len);
        len -= strlen(ep->core_attr);

        ep++;
    }

    return coap_make_response(scratch, outpkt, (const uint8_t *)rsp, strlen(rsp), id_hi, id_lo,
                              &inpkt->tok, COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_APPLICATION_LINKFORMAT);
}
