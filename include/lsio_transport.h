/*
 * libsocketio transport header
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

#ifndef LSIO_TRANSPORT_H_INCLUDED
#define LSIO_TRANSPORT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "lsio_socket.h"

//typedef struct lsio_socket_s lsio_socket_t;

typedef enum lsio_transport_type_e {
	LSIO_TRANSPORT_TYPE_NONE = 0,
	LSIO_TRANSPORT_TYPE_XHR_POLLING = 1,
	LSIO_TRANSPORT_TYPE_XHR_MULTIPART = 2,
	LSIO_TRANSPORT_TYPE_HTMLFILE = 4,
	LSIO_TRANSPORT_TYPE_WEBSOCKET = 8,
	LSIO_TRANSPORT_TYPE_FLASHSOCKET = 16,
	LSIO_TRANSPORT_TYPE_JSONP_POLLING = 32
} lsio_transport_type_t;

typedef enum lsio_transport_state_e {
	LSIO_TRANSPORT_STATE_CLOSED = 1,
	LSIO_TRANSPORT_STATE_CLOSING = 2,
	LSIO_TRANSPORT_STATE_OPEN = 4,
	LSIO_TRANSPORT_STATE_OPENING = 8
} lsio_transport_state_t;

typedef struct lsio_transport_s {
	lsio_transport_type_t type;
	lsio_transport_state_t state;
	struct lsio_socket_s *socket;
} lsio_transport_t;

void lsio_transport_init(lsio_transport_t *);

#ifdef __cplusplus
}
#endif

#endif
