/*
 * libsocketio socket header
 * Copyright (c) 2013 Isaac Waldron
 */

#ifndef __lsio_socket_h__
#define __lsio_socket_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "lsio_transport.h"

typedef struct lsio_transport_s lsio_transport_t;

typedef enum lsio_socket_state_e {
	LSIO_SOCKET_STATE_DISCONNECTED,
	LSIO_SOCKET_STATE_DISCONNECTING,
	LSIO_SOCKET_STATE_CONNECTED,
	LSIO_SOCKET_STATE_CONNECTING
} lsio_socket_state_t;

typedef struct lsio_socket_s {
	lsio_socket_state_t state;
	char *session_id;
	int heartbeat_timeout;
	int close_timeout;
	lsio_transport_t *transport;
} lsio_socket_t;

void lsio_socket_init(lsio_socket_t *);

#ifdef __cplusplus
}
#endif

#endif
