/*
 * libsocketio packet header
 * 
 * Copyright (c) 2013, Isaac Waldron
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in 
 *    the documentation and/or other materials provided with the 
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
 
#ifndef LSIO_PACKET_H_INCLUDED
#define LSIO_PACKET_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef enum lsio_packet_type_e {
	LSIO_PACKET_TYPE_UNDEFINED = -1,
	LSIO_PACKET_TYPE_DISCONNECT = 0,
	LSIO_PACKET_TYPE_CONNECT = 1,
	LSIO_PACKET_TYPE_HEARTBEAT = 2,
	LSIO_PACKET_TYPE_MESSAGE = 3,
	LSIO_PACKET_TYPE_JSON_MESSAGE = 4,
	LSIO_PACKET_TYPE_EVENT = 5,
	LSIO_PACKET_TYPE_ACK = 6,
	LSIO_PACKET_TYPE_ERROR = 7,
	LSIO_PACKET_TYPE_NOOP = 8
} lsio_packet_type_t;

typedef struct lsio_packet_s {
	lsio_packet_type_t type;
	unsigned int id;
	unsigned int user_ack;
	char *endpoint;
	char *data;
} lsio_packet_t;

void lsio_packet_init(lsio_packet_t *);
int lsio_packet_parse(lsio_packet_t *, char *);
void lsio_packet_free(lsio_packet_t *);

#ifdef __cplusplus
}
#endif

#endif
