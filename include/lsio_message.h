/*
 * libsocketio message header
 * Copyright (c) 2013 Isaac Waldron
 */
 
#ifndef LSIO_MESSAGE_H_INCLUDED
#define LSIO_MESSAGE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef enum lsio_message_type_e {
	LSIO_MESSAGE_TYPE_UNDEFINED = -1,
	LSIO_MESSAGE_TYPE_DISCONNECT = 0,
	LSIO_MESSAGE_TYPE_CONNECT = 1,
	LSIO_MESSAGE_TYPE_HEARTBEAT = 2,
	LSIO_MESSAGE_TYPE_MESSAGE = 3,
	LSIO_MESSAGE_TYPE_JSON_MESSAGE = 4,
	LSIO_MESSAGE_TYPE_EVENT = 5,
	LSIO_MESSAGE_TYPE_ACK = 6,
	LSIO_MESSAGE_TYPE_ERROR = 7,
	LSIO_MESSAGE_TYPE_NOOP = 8
} lsio_message_type_t;

typedef struct lsio_message_s {
	lsio_message_type_t type;
	unsigned int id;
	char *endpoint;
	char *data;
} lsio_message_t;

void lsio_message_init(lsio_message_t *);
int lsio_message_parse(lsio_message_t *, char *);
void lsio_message_free(lsio_message_t *);

#ifdef __cplusplus
}
#endif

#endif
