/*
 * libsocketio socket implementation
 * Copyright (c) 2013 Isaac Waldron
 */
 
#include <stdlib.h>

#include "libsocketio.h"

void lsio_socket_init(lsio_socket_t *socket)
{
	if (NULL == socket)
		return;
		
	socket->state = LSIO_SOCKET_STATE_DISCONNECTED;
	socket->session_id = NULL;
	socket->heartbeat_timeout = 0;
	socket->close_timeout = 0;
	socket->transport = NULL;
}

