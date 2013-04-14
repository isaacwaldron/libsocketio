/*
 * libsocketio transport implementation
 * Copyright (c) 2013 Isaac Waldron
 */
 
#include <stdlib.h>

#include "lsio_transport.h"

void lsio_transport_init(lsio_transport_t *transport)
{
	if (NULL == transport)
		return;
		
	transport->type = LSIO_TRANSPORT_TYPE_NONE;
	transport->state = LSIO_TRANSPORT_STATE_CLOSED;
	transport->socket = NULL;
}

