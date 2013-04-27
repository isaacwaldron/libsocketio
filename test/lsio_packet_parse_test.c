/*
 * libsocketio packet parse test
 * 
 * Copyright (c) 2013, Isaac Waldron
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in 
 *    the documentation and/or other materials provided with the 
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
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
