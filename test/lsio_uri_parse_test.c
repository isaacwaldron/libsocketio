/*
 * libsocketio URI parse test
 * Copyright (c) 2013 Isaac Waldron
 */
 
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "libsocketio.h"

void vlogger(const char *, va_list);

int main(int arcg, char **argv) {
	lsio_uri_t *uri;
	
	lsio_logging_init(vlogger);
	
	uri = lsio_uri_parse("https://socketio.mtgox.com:443/socket.io/1/");
	if (NULL == uri) {
		fprintf(stderr, "lsio_uri_parse returned NULL\n");
		return -1;
	}
	
	fprintf(stderr, "scheme: %s host: %s port: %d namespace: %s "
		"version: %d\n",
		uri->scheme, uri->host, uri->port, uri->namespace, 
		uri->protocol_version);

	lsio_uri_free(uri);

	lsio_logging_destroy();
	
	return 0;
}

void vlogger(const char *fmt, va_list arg)
{
	vfprintf(stderr, fmt, arg);
	fprintf(stderr, "\n");
}
