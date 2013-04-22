/*
 * libsocketio packet header
 * Copyright (c) 2013 Isaac Waldron
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
