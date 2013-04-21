/*
 * libsocketio message parse test
 * Copyright (c) 2013 Isaac Waldron
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "libsocketio.h"

void vlogger(const char *, va_list);

int main(int argc, char **argv)
{
	lsio_message_t *message;
	
	lsio_logging_init(vlogger);
	
	if (2 != argc) {
		fprintf(stderr, "usage: lsio_message_parse_test MESSAGE\n");
		return -1;
	}
	
	if (NULL == (message = malloc(sizeof(lsio_message_t)))) {
		fprintf(stderr, "failed to allocate memory for message\n");
		return -1;
	}

	lsio_message_init(message);
	
	if (0 != lsio_message_parse(message, argv[1])) {
		fprintf(stderr, "lsio_message_parse returned error\n");
		lsio_message_free(message);
		return -1;
	}
	
	fprintf(stderr, "type: %d id: %d\n"
		"endpoint: %s\n"
		"data: %s\n", 
		message->type, message->id, message->endpoint, message->data);
	
	lsio_message_free(message);
	
	lsio_logging_destroy();
	
	return 0;
}

void vlogger(const char *fmt, va_list arg)
{
	vfprintf(stderr, fmt, arg);
	fprintf(stderr, "\n");
}
