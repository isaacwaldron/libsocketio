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
	LSIO_TRASNPORT_TYPE_XHR_POLLING,
	LSIO_TRASNPORT_TYPE_XHR_MULTIPART,
	LSIO_TRASNPORT_TYPE_HTMLFILE,
	LSIO_TRASNPORT_TYPE_WEBSOCKET,
	LSIO_TRASNPORT_TYPE_FLASHSOCKET,
	LSIO_TRASNPORT_TYPE_JSONP_POLLING
} lsio_transport_type_t;

typedef enum lsio_transport_state_e {
	LSIO_TRANSPORT_STATE_CLOSED,
	LSIO_TRANSPORT_STATE_CLOSING,
	LSIO_TRANSPORT_STATE_OPEN,
	LSIO_TRANSPORT_STATE_OPENING
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
