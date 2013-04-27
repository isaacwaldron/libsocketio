/*
 * libsocketio socket header
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

#ifndef LSIO_SOCKET_H_INCLUDED
#define LSIO_SOCKET_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "lsio_transport.h"

//typedef struct lsio_transport_s lsio_transport_t;

typedef enum lsio_socket_state_e {
	LSIO_SOCKET_STATE_DISCONNECTED = 1,
	LSIO_SOCKET_STATE_DISCONNECTING = 2,
	LSIO_SOCKET_STATE_CONNECTED = 4,
	LSIO_SOCKET_STATE_CONNECTING = 8
} lsio_socket_state_t;

typedef struct lsio_socket_s {
	lsio_socket_state_t state;
	char *session_id;
	int heartbeat_timeout;
	int close_timeout;
	struct lsio_transport_s *transport;
} lsio_socket_t;

void lsio_socket_init(lsio_socket_t *);
int lsio_socket_handshake(lsio_socket_t *, const char *uri);

#ifdef __cplusplus
}
#endif

#endif
