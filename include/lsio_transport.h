/*
 * libsocketio transport header
 * Copyright (c) 2013 Isaac Waldron
 */

#ifndef __lsio_transport_h__
#define __lsio_transport_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "lsio_socket.h"

typedef struct lsio_socket_s lsio_socket_t;

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
	lsio_socket_t *socket;
} lsio_transport_t;

void lsio_transport_init(lsio_transport_t *);

#ifdef __cplusplus
}
#endif

#endif
