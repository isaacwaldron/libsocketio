/*
 * libsocketio message implementation
 * Copyright (c) 2013 Isaac Waldron
 */

#include <stdlib.h>

#include "lsio_message.h"

void lsio_message_init(lsio_message_t *message)
{
	message->type = LSIO_MESSAGE_TYPE_UNDEFINED;
	message->id = 0;
	message->endpoint = NULL;
	message->data = NULL;
}

void lsio_message_free(lsio_message_t *message)
{
	if (NULL == message)
		return;
	
	if (NULL != message->endpoint)
		free(message->endpoint);
	
	if (NULL != message->data)
		free(message->data);
		
	free(message);
	
	message = NULL;
}
