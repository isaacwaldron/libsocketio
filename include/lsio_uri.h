/*
 * libsocketio URI header
 * Copyright (c) 2013 Isaac Waldron
 */

#ifndef LSIO_URI_H_INCLUDED
#define LSIO_URI_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lsio_uri_s {
	char *scheme;
	char *host;
	int port;
	char *namespace;
	int protocol_version;
	char *transport;
	char *session_id;
	char *query;
} lsio_uri_t;

lsio_uri_t *lsio_uri_parse(const char *);
void lsio_uri_free(lsio_uri_t *);

#ifdef __cplusplus
}
#endif

#endif
