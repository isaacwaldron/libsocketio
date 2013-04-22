/*
 * libsocketio packet implementation
 * Copyright (c) 2013 Isaac Waldron
 */

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "lsio_logging.h"
#include "lsio_packet.h"

int lsio_packet_type_is_valid(int);

void lsio_packet_init(lsio_packet_t *packet)
{
	packet->type = LSIO_PACKET_TYPE_UNDEFINED;
	packet->id = 0;
	packet->endpoint = NULL;
	packet->data = NULL;
}

int lsio_packet_parse(lsio_packet_t *packet, char *frame)
{
	char *c1, *c2, *endpoint, *data;
	unsigned long int ltype, lid;
	int type;
	unsigned int id, user_ack;
	
	if (NULL == frame) {
		lsio_debug("lsio_packet_parse: NULL frame");
		return -1;
	}
	
	c1 = frame;
	c2 = frame;
	errno = 0;
	ltype = strtoul(c1, &c2, 10);
	if (0 != errno || 1 != (c2 - c1)) {
		lsio_debug("lsio_packet_parse: failed to parse packet type");
		return -1;
	}
	if (INT_MAX < ltype) {
		lsio_debug("lsio_packet_parse: packet type too large");
		return -1;
	}
	type = (int) ltype;
	if (0 != lsio_packet_type_is_valid(type)) {
		lsio_debug("lsio_packet_parse: invalid packet type");
		return -1;
	}
	lsio_debug("lsio_packet_parse: found packet type %d", type);
	
	c1 = c2;
	if (':' != *c2) {
		lsio_debug("lsio_packet_parse: missing ':' after packet "
			"type");
		return -1;
	}
	
	c1++;
	c2 = c1;
	errno = 0;
	lid = strtoul(c1, &c2, 10);
	if (0 != errno) {
		lsio_debug("lsio_packet_parse: failed to parse packet id");
		return -1;
	}
	if (UINT_MAX < lid) {
		lsio_debug("lsio_packet_parse: packet id too large");
		return -1;
	}
	id = (unsigned int) lid;
	user_ack = 0;
	if ('+' == *c2) {
		user_ack = 1;
		c2++;
	}
	lsio_debug("lsio_packet_parse: found packet id %d, user flag %d", 
		id, user_ack);
		
	c1 = c2;
	if (':' != *c1) {
		lsio_debug("lsio_packet_parse: missing ':' after packet id");
		return -1;
	}
	
	c1++;
	if (':' == *c1) {
		endpoint = NULL;
		c2 = c1;
	}
	else {
		c2 = c1;
		while (':' != *c2 && '\0' != *c2)
			c2++;
			
		if (NULL == (endpoint = calloc(c2 - c1 + 1, sizeof(char)))) {
			lsio_debug("lsio_packet_parse: failed to allocate memory "
				"for endpoint");
			return -1;
		}
		
		strncpy(endpoint, c1, c2 - c1);
		endpoint[c2 - c1] = '\0';
	}
	lsio_debug("lsio_packet_parse: endpoint %s", endpoint);
	
	c1 = c2;
	if (':' != *c1) {
		lsio_debug("lsio_packet_parse: missing ':' after endpoint");
		free(endpoint);
		return -1;
	}
	
	c1++;
	c2 = c1;
	while ('\0' != *c2)
		c2++;
	if (NULL == (data = calloc(c2 - c1 + 1, sizeof(char)))) {
		lsio_debug("lsio_packet_parse: failed to allocate memory for "
			"data");
		free(endpoint);
		return -1;
	}
	strncpy(data, c1, c2 - c1);
	data[c2 - c1] = '\0';
	lsio_debug("lsio_packet_parse: data %s", data);
	
	packet->type = type;
	packet->id = id;
	packet->endpoint = endpoint;
	packet->data = data;
	
	return 0;
}

void lsio_packet_free(lsio_packet_t *packet)
{
	if (NULL == packet)
		return;
	
	if (NULL != packet->endpoint)
		free(packet->endpoint);
	
	if (NULL != packet->data)
		free(packet->data);
		
	free(packet);
	
	packet = NULL;
}

int lsio_packet_type_is_valid(int id)
{
	if (LSIO_PACKET_TYPE_DISCONNECT == id ||
			LSIO_PACKET_TYPE_CONNECT == id ||
			LSIO_PACKET_TYPE_HEARTBEAT == id ||
			LSIO_PACKET_TYPE_MESSAGE == id ||
			LSIO_PACKET_TYPE_JSON_MESSAGE == id ||
			LSIO_PACKET_TYPE_EVENT == id ||
			LSIO_PACKET_TYPE_ACK == id ||
			LSIO_PACKET_TYPE_ERROR == id ||
			LSIO_PACKET_TYPE_NOOP == id)
		return 0;
		
	return -1;
}
