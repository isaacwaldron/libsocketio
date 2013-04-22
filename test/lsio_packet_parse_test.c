/*
 * libsocketio packet parse test
 * Copyright (c) 2013 Isaac Waldron
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "libsocketio.h"

void vlogger(const char *, va_list);

int main(int argc, char **argv)
{
	lsio_packet_t *packet;
	
	lsio_logging_init(vlogger);
	
	if (2 != argc) {
		fprintf(stderr, "usage: lsio_packet_parse_test MESSAGE\n");
		return -1;
	}
	
	if (NULL == (packet = malloc(sizeof(lsio_packet_t)))) {
		fprintf(stderr, "failed to allocate memory for packet\n");
		return -1;
	}

	lsio_packet_init(packet);
	
	if (0 != lsio_packet_parse(packet, argv[1])) {
		fprintf(stderr, "lsio_packet_parse returned error\n");
		lsio_packet_free(packet);
		return -1;
	}
	
	fprintf(stderr, "type: %d id: %d\n"
		"endpoint: %s\n"
		"data: %s\n", 
		packet->type, packet->id, packet->endpoint, packet->data);
	
	lsio_packet_free(packet);
	
	lsio_logging_destroy();
	
	return 0;
}

void vlogger(const char *fmt, va_list arg)
{
	vfprintf(stderr, fmt, arg);
	fprintf(stderr, "\n");
}
