/*
 * libsocketio packet implementation
 * Copyright (c) 2013 Isaac Waldron
 */

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "lsio_common.h"
#include "lsio_logging.h"
#include "lsio_packet.h"

const char LSIOPacketDelimiter = ':';

typedef struct lsio_packet_piece_s {
	char *text;
	struct lsio_packet_piece_s *next;
} lsio_packet_piece_t;

lsio_packet_piece_t *lsio_packet_split(char *frame);
void lsio_packet_piece_free(lsio_packet_piece_t *);

int lsio_packet_parse_disconnect(lsio_packet_t *, 
	lsio_packet_piece_t *);
int lsio_packet_parse_connect(lsio_packet_t *, lsio_packet_piece_t *);
int lsio_packet_parse_heartbeat(lsio_packet_t *, lsio_packet_piece_t *);
int lsio_packet_parse_message(lsio_packet_t *, lsio_packet_piece_t *);
int lsio_packet_parse_json_message(lsio_packet_t *, 
	lsio_packet_piece_t *);
int lsio_packet_parse_event(lsio_packet_t *, lsio_packet_piece_t *);
int lsio_packet_parse_ack(lsio_packet_t *, lsio_packet_piece_t *);
int lsio_packet_parse_error(lsio_packet_t *, lsio_packet_piece_t *);
int lsio_packet_parse_noop(lsio_packet_t *, lsio_packet_piece_t *);

void lsio_packet_init(lsio_packet_t *packet)
{
	packet->type = LSIO_PACKET_TYPE_UNDEFINED;
	packet->id = 0;
	packet->user_ack = 0;
	packet->endpoint = NULL;
	packet->data = NULL;
}

int lsio_packet_parse(lsio_packet_t *packet, char *frame)
{
	char *c1, *c2;
	unsigned long int ltype;
	int result, type;
	lsio_packet_piece_t *piece, *current_piece;
	
	piece = lsio_packet_split(frame);
	if (NULL == piece) {
		LSIO_DEBUG("lsio_packet_split returned NULL");
		lsio_packet_piece_free(piece);
		return LSIO_ERROR;
	}
	current_piece = piece;
	
	c1 = current_piece->text;
	c2 = c1;
	errno = 0;
	ltype = strtoul(c1, &c2, 10);
	if (0 != errno || 1 != (c2 - c1)) {
		LSIO_DEBUG("failed to parse packet type");
		lsio_packet_piece_free(piece);
		return LSIO_ERROR;
	}
	if (INT_MAX < ltype) {
		LSIO_DEBUG("packet type too large");
		lsio_packet_piece_free(piece);
		return LSIO_ERROR;
	}
	type = (int) ltype;
	LSIO_DEBUG("found packet type %d", type);
	
	switch (type) {
		case LSIO_PACKET_TYPE_DISCONNECT:
			result = lsio_packet_parse_disconnect(packet, 
				current_piece);
			break;
			
		case LSIO_PACKET_TYPE_CONNECT:
			result = lsio_packet_parse_connect(packet, current_piece);
			break;
			
		case LSIO_PACKET_TYPE_HEARTBEAT:
			result = lsio_packet_parse_heartbeat(packet, current_piece);
			break;
			
		case LSIO_PACKET_TYPE_MESSAGE:
			result = lsio_packet_parse_message(packet, current_piece);
			break;
			
		case LSIO_PACKET_TYPE_JSON_MESSAGE:
			result = lsio_packet_parse_json_message(packet, 
				current_piece);
			break;
			
		case LSIO_PACKET_TYPE_EVENT:
			result = lsio_packet_parse_event(packet, current_piece);
			break;
			
		case LSIO_PACKET_TYPE_ACK:
			result = lsio_packet_parse_ack(packet, current_piece);
			break;
			
		case LSIO_PACKET_TYPE_ERROR:
			result = lsio_packet_parse_error(packet, current_piece);
			break;
			
		case LSIO_PACKET_TYPE_NOOP:
			result = lsio_packet_parse_noop(packet, current_piece);
			break;
			
		default:
			LSIO_DEBUG("invalid packet type %d", 
				type);
			result = -1;
			break;
	}
	
	lsio_packet_piece_free(piece);

	return result;
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

lsio_packet_piece_t *lsio_packet_split(char *frame)
{
	lsio_packet_piece_t *result, *current_piece;
	int n;
	char *c1, *c2;
	
	if (NULL == frame || '\0' == *frame) {
		LSIO_DEBUG("NULL or zero-length frame");
		return NULL;
	}
	
	if (NULL == (result = malloc(sizeof(lsio_packet_piece_t)))) {
		LSIO_DEBUG("failed to allocate memory for packet piece");
		return NULL;
	}

	n = 0;	
	current_piece = result;
	current_piece->text = NULL;
	current_piece->next = NULL;
	c1 = frame;
	while (n < 3 && '\0' != *c1) {
		if (LSIOPacketDelimiter == *c1)
			c1++;
		c2 = c1;
		while (LSIOPacketDelimiter != *c2 && '\0' != *c2)
			c2++;
		
		n++;
		
		if (NULL == (current_piece->text = 
				calloc(c2 - c1 + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for packet piece "
				"text");
			lsio_packet_piece_free(result);
			return NULL;
		}
		strncpy(current_piece->text, c1, c2 - c1);
		current_piece->text[c2 - c1] = '\0';
		
		if (LSIOPacketDelimiter == *c2) {
			LSIO_DEBUG("appending new packet piece");
			
			if (NULL == (current_piece->next = 
					malloc(sizeof(lsio_packet_piece_t)))) {
				LSIO_DEBUG("failed to allocate memory for packet "
					"piece");
				lsio_packet_piece_free(result);
				return NULL;
			}
			
			current_piece = current_piece->next;
			current_piece->text = NULL;
			current_piece->next = NULL;
		}

		c1 = c2;
	}
	
	if (LSIOPacketDelimiter == *c1) {
		c1++;
		c2 = c1;
		while ('\0' != *c2)
			c2++;
			
		n++;
			
		if (NULL == (current_piece->text = 
				calloc(c2 - c1 + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for packet piece "
				"text");
			lsio_packet_piece_free(result);
			return NULL;
		}
		strncpy(current_piece->text, c1, c2 - c1);
		current_piece->text[c2 - c1] = '\0';
	}
	
	LSIO_DEBUG("successfully parsed %d packet pieces", n);

	return result;
}

void lsio_packet_piece_free(lsio_packet_piece_t *piece)
{
	if (NULL == piece)
		return;
		
	if (NULL != piece->text)
		free(piece->text);
		
	lsio_packet_piece_free(piece->next);
	free(piece);
}

int lsio_packet_parse_disconnect(lsio_packet_t *packet, 
	lsio_packet_piece_t *piece)
{
	int n, type;
	char *endpoint;
	
	type = LSIO_PACKET_TYPE_DISCONNECT;
	
	if (NULL == piece->next) {
		// disconnect whole socket
		packet->type = type;
		return LSIO_SUCCESS;
	}
	
	piece = piece->next;
	
	if (0 < strlen(piece->text)) {
		LSIO_DEBUG("non-zero-length message id in disconnect packet");
		return LSIO_ERROR;
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing endpoint");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	if (NULL != piece->next) {
		LSIO_DEBUG("unexpected data piece");
		return LSIO_ERROR;
	}		
	
	n = strlen(piece->text);
	if (0 == n) {
		LSIO_DEBUG("missing endpoint text");
		return LSIO_ERROR;
	}
	
	if (NULL == (endpoint = calloc(n + 1, sizeof(char)))) {
		LSIO_DEBUG("out of memory");
		return LSIO_ERROR;
	}
	
	strncpy(endpoint, piece->text, n);
	endpoint[n] = '\0';
	
	packet->type = type;
	packet->endpoint = endpoint;
	return LSIO_SUCCESS;
}

int lsio_packet_parse_connect(lsio_packet_t *packet, 
	lsio_packet_piece_t *piece)
{
	int n, type;
	char *endpoint;
	
	type = LSIO_PACKET_TYPE_CONNECT;
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing message id piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	if (0 < strlen(piece->text)) {
		LSIO_DEBUG("non-zero-length message id");
		return LSIO_ERROR;
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing endpoint piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	if (NULL != piece->next) {
		LSIO_DEBUG("unexpected data piece");
		return LSIO_ERROR;
	}
	
	n = strlen(piece->text);
	if (0 == n) {
		LSIO_DEBUG("missing endpoint text");
		return LSIO_ERROR;
	}
	
	if (NULL == (endpoint = calloc(n + 1, sizeof(char)))) {
		LSIO_DEBUG("out of memory");
		return LSIO_ERROR;
	}
	
	strncpy(endpoint, piece->text, n);
	endpoint[n] = '\0';
	
	packet->type = type;
	packet->endpoint = endpoint;
	return LSIO_SUCCESS;
}

int lsio_packet_parse_heartbeat(lsio_packet_t *packet, 
	lsio_packet_piece_t *piece)
{
	lsio_packet_type_t type;
	
	type = LSIO_PACKET_TYPE_HEARTBEAT;
	
	while (NULL != piece->next) {
		piece = piece->next;
	
		if (0 < strlen(piece->text)) {
			LSIO_DEBUG("non-zero-length piece text");
			return LSIO_ERROR;
		}
	}
	
	packet->type = type;
	return LSIO_SUCCESS;
}

int lsio_packet_parse_message(lsio_packet_t *packet, 
	lsio_packet_piece_t *piece)
{
	int id, n, type, user_ack;
	char *c1, *c2, *data, *endpoint;
	unsigned long int lid;
	
	type = LSIO_PACKET_TYPE_MESSAGE;
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing message id piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	c1 = piece->text;
	c2 = c1;
	errno = 0;
	lid = strtoul(c1, &c2, 10);
	if (0 != errno || c1 == c2) {
		LSIO_DEBUG("failed to parse message id");
		return LSIO_ERROR;
	}
	if (INT_MAX < lid) {
		LSIO_DEBUG("message id too large");
		return LSIO_ERROR;
	}
	id = (int) lid;
	
	user_ack = 0;
	if ('\0' != *c2) {
		if ('+' != *c2) {
			LSIO_DEBUG("invalid character %c in message id", *c2);
			return LSIO_ERROR;
		}
		user_ack = 1;
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing endpoint piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	endpoint = NULL;
	n = strlen(piece->text);
	if (0 < n) {
		if (NULL == (endpoint = calloc(n + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for endpoint");
			return LSIO_ERROR;
		}
		
		strncpy(endpoint, piece->text, n);
		endpoint[n] = '\0';
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing data piece");
		if (NULL != endpoint)
			free(endpoint);
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	data = NULL;
	n = strlen(piece->text);
	if (0 < n) {
		if (NULL == (data = calloc(n + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for data");
			if (NULL != endpoint)
				free(endpoint);
			return LSIO_ERROR;
		}
		
		strncpy(data, piece->text, n);
		data[n] = '\0';
	}
	
	if (NULL != piece->next) {
		LSIO_DEBUG("unexpected packet piece after data");
		if (NULL != endpoint)
			free(endpoint);
		if (NULL != data)
			free(data);
		return LSIO_ERROR;
	}
	
	packet->type = type;
	packet->id = id;
	packet->user_ack = user_ack;
	packet->endpoint = endpoint;
	packet->data = data;
	
	return LSIO_SUCCESS;
}

int lsio_packet_parse_json_message(lsio_packet_t *packet, 
	lsio_packet_piece_t *piece)
{
	int id, n, type, user_ack;
	char *c1, *c2, *data, *endpoint;
	unsigned long int lid;
	
	type = LSIO_PACKET_TYPE_JSON_MESSAGE;
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing message id piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;

	c1 = piece->text;
	c2 = c1;
	errno = 0;
	lid = strtoul(c1, &c2, 10);
	if (0 != errno || c1 == c2) {
		LSIO_DEBUG("failed to parse message id");
		return LSIO_ERROR;
	}
	if (INT_MAX < lid) {
		LSIO_DEBUG("message id too large");
		return LSIO_ERROR;
	}
	id = (int) lid;
	
	user_ack = 0;
	if ('\0' != *c2) {
		if ('+' != *c2) {
			LSIO_DEBUG("invalid character %c in message id", *c2);
			return LSIO_ERROR;
		}
		user_ack = 1;
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing endpoint piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	endpoint = NULL;
	n = strlen(piece->text);
	if (0 < n) {
		if (NULL == (endpoint = calloc(n + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for endpoint");
			return LSIO_ERROR;
		}
		
		strncpy(endpoint, piece->text, n);
		endpoint[n] = '\0';
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing data piece");
		if (NULL != endpoint)
			free(endpoint);
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	data = NULL;
	n = strlen(piece->text);
	if (0 < n) {
		if (NULL == (data = calloc(n + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for data");
			if (NULL != endpoint)
				free(endpoint);
			return LSIO_ERROR;
		}
		
		strncpy(data, piece->text, n);
		data[n] = '\0';
	}
	
	if (NULL != piece->next) {
		LSIO_DEBUG("unexpected packet piece after data");
		if (NULL != endpoint)
			free(endpoint);
		if (NULL != data)
			free(data);
		return LSIO_ERROR;
	}
	
	packet->type = type;
	packet->id = id;
	packet->user_ack = user_ack;
	packet->endpoint = endpoint;
	packet->data = data;
	
	return LSIO_SUCCESS;
}

int lsio_packet_parse_event(lsio_packet_t *packet, 
	lsio_packet_piece_t *piece)
{
	lsio_packet_type_t type;
	char *c1, *c2, *endpoint, *data;
	unsigned long int lid;
	int id, n, user_ack;
	
	type = LSIO_PACKET_TYPE_EVENT;
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing message id piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	c1 = piece->text;
	c2 = c1;
	errno = 0;
	lid = strtoul(c1, &c2, 10);
	if (0 != errno || c1 == c2) {
		LSIO_DEBUG("failed to parse message id");
		return LSIO_ERROR;
	}
	if (INT_MAX < lid) {
		LSIO_DEBUG("message id too large");
		return LSIO_ERROR;
	}
	id = (int) lid;
	
	user_ack = 0;
	if ('\0' != *c2) {
		if ('+' != *c2) {
			LSIO_DEBUG("invalid character %c in message id", *c2);
			return LSIO_ERROR;
		}
		user_ack = 1;
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing endpoint piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	endpoint = NULL;
	n = strlen(piece->text);
	if (0 < n) {
		if (NULL == (endpoint = calloc(n + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for endpoint");
			return LSIO_ERROR;
		}
		
		strncpy(endpoint, piece->text, n);
		endpoint[n] = '\0';
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing data piece");
		if (NULL != endpoint)
			free(endpoint);
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	data = NULL;
	n = strlen(piece->text);
	if (0 == n) {
		LSIO_DEBUG("zero length data");
		if (NULL != endpoint)
			free(endpoint);
		return LSIO_ERROR;
	}
	else {
		if (NULL == (data = calloc(n + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for data");
			if (NULL != endpoint)
				free(endpoint);
			return LSIO_ERROR;
		}
		
		strncpy(data, piece->text, n);
		data[n] = '\0';
	}
	
	if (NULL != piece->next) {
		LSIO_DEBUG("unexpected packet piece after data");
		if (NULL != endpoint)
			free(endpoint);
		if (NULL != data)
			free(data);
		return LSIO_ERROR;
	}
	
	packet->type = type;
	packet->id = id;
	packet->user_ack = user_ack;
	packet->endpoint = endpoint;
	packet->data = data;
	
	return LSIO_SUCCESS;
}

int lsio_packet_parse_ack(lsio_packet_t *packet, 
	lsio_packet_piece_t *piece)
{
	lsio_packet_type_t type;
	char *c1, *c2, *data;
	unsigned long int lid;
	int id, n;
	
	type = LSIO_PACKET_TYPE_ACK;
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing message id piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	if (0 != strlen(piece->text)) {
		LSIO_DEBUG("non-zero length message id piece");
		return LSIO_ERROR;
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing endpoint piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	if (0 != strlen(piece->text)) {
		LSIO_DEBUG("non-zero length endpoint piece");
		return LSIO_ERROR;
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing data piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	c1 = piece->text;
	c2 = c1;
	errno = 0;
	lid = strtoul(c1, &c2, 10);
	if (0 != errno || c1 == c2) {
		LSIO_DEBUG("failed to parse message id from ACK data");
		return LSIO_ERROR;
	}
	if (INT_MAX < lid) {
		LSIO_DEBUG("message id too large");
		return LSIO_ERROR;
	}
	id = (int) lid;
	
	data = NULL;
	if ('\0' != *c2) {
		if ('+' != *c2) {
			LSIO_DEBUG("invalid character %c in ACK data", *c2);
			return LSIO_ERROR;
		}
		
		c2++;
		n = strlen(c2);
		if (0 < n) {
			if (NULL == (data = calloc(n + 1, sizeof(char)))) {
				LSIO_DEBUG("failed to allocate memory for data");
				return LSIO_ERROR;
			}
			
			strncpy(data, c2, n);
			data[n] = '\0';
		}
	}
	
	if (NULL != piece->next) {
		LSIO_DEBUG("unexpected packet piece");
		if (NULL != data)
			free(data);
		return LSIO_ERROR;
	}
	
	packet->type = type;
	packet->id = id;
	packet->data = data;
	
	return LSIO_SUCCESS;
}

int lsio_packet_parse_error(lsio_packet_t *packet, 
	lsio_packet_piece_t *piece)
{
	lsio_packet_type_t type;
	char *endpoint, *data;
	int n;
	
	type = LSIO_PACKET_TYPE_ERROR;
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing message id piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	if (0 < strlen(piece->text)) {
		LSIO_DEBUG("non-zero length message id");
		return LSIO_ERROR;
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing endpoint piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	endpoint = NULL;
	n = strlen(piece->text);
	if (0 < n) {
		if (NULL == (endpoint = calloc(n + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for endpoint");
			return LSIO_ERROR;
		}
		
		strncpy(endpoint, piece->text, n);
		endpoint[n] = '\0';
	}
	
	if (NULL == piece->next) {
		LSIO_DEBUG("missing data piece");
		return LSIO_ERROR;
	}
	
	piece = piece->next;
	
	data = NULL;
	n = strlen(piece->text);
	if (0 == n) {
		LSIO_DEBUG("zero length data");
		if (NULL != endpoint)
			free(endpoint);
		return LSIO_ERROR;
	}
	else {
		if (NULL == (data = calloc(n + 1, sizeof(char)))) {
			LSIO_DEBUG("failed to allocate memory for data");
			if (NULL != endpoint)
				free(endpoint);
			return LSIO_ERROR;
		}
		
		strncpy(data, piece->text, n);
		data[n] = '\0';
	}
	
	if (NULL != piece->next) {
		LSIO_DEBUG("unexpected packet piece after data");
		if (NULL != endpoint)
			free(endpoint);
		if (NULL != data)
			free(data);
		return LSIO_ERROR;
	}
	
	packet->type = type;
	packet->endpoint = endpoint;
	packet->data = data;
	
	return LSIO_SUCCESS;
}

int lsio_packet_parse_noop(lsio_packet_t *packet, 
	lsio_packet_piece_t *piece)
{
	if (NULL != piece->next) {
		LSIO_DEBUG("unexpected packet piece after type");
		return LSIO_ERROR;
	}
	
	packet->type = LSIO_PACKET_TYPE_NOOP;
	
	return LSIO_SUCCESS;
}

