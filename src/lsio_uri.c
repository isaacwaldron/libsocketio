/*
 * libsocketio URI implementation
 * Copyright (c) 2013 Isaac Waldron
 */

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#include "uriparser/Uri.h"
#include "libsocketio.h"

char *lsio_extract_text_range(UriTextRangeA *);

lsio_uri_t *lsio_uri_parse(const char *uri_string)
{
	UriParserStateA state;
	UriUriA uri;
	UriPathSegmentA *segment;
	lsio_uri_t *result;
	char *scratch;
	unsigned long int port, protocol_version;
	
	lsio_debug("lsio_uri_parse: entering");
	
	state.uri = &uri;
	if (URI_SUCCESS != uriParseUriA(&state, uri_string)) {
		lsio_debug("lsio_uri_parse: uriParseUriA returned failure");
		uriFreeUriMembersA(&uri);
		return NULL;
	}
	
	if (NULL == (result = (lsio_uri_t *) malloc(sizeof(lsio_uri_t)))) {
		lsio_debug("lsio_uri_parse: out of memory");
		uriFreeUriMembersA(&uri);
		return NULL;
	}
	
	result->scheme = lsio_extract_text_range(&uri.scheme);
	if (NULL == result->scheme) {
		lsio_debug("lsio_uri_parse: scheme extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	
	result->host = lsio_extract_text_range(&uri.hostText);
	if (NULL == result->host) {
		lsio_debug("lsio_uri_parse: host extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	
	scratch = lsio_extract_text_range(&uri.portText);
	errno = 0;
	port = strtoul(scratch, (char **) NULL, 10);
	free(scratch);
	if (0 != errno || UINT_MAX < port) {
		lsio_debug("lsio_uri_parse: port extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	result->port = (int) port;
	
	segment = uri.pathHead;
	if (NULL == segment) {
		lsio_debug("lsio_uri_parse: missing namespace path segment");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	result->namespace = lsio_extract_text_range(&segment->text);
	if (NULL == result->namespace) {
		lsio_debug("lsio_uri_parse: namespace extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	
	segment = segment->next;
	if (NULL == segment) {
		lsio_debug("lsio_uri_parse: missing version path segment");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	scratch = lsio_extract_text_range(&segment->text);
	errno = 0;
	protocol_version = strtoul(scratch, (char **) NULL, 10);
	free(scratch);
	if (0 != errno || UINT_MAX < protocol_version) {
		lsio_debug("lsio_uri_parse: version extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	result->protocol_version = (int) protocol_version;
	
	if (NULL != segment->next && 
		segment->next->text.afterLast != segment->next->text.first) {
		lsio_debug("lsio_uri_parse: unexpected path segment after "
				   "version");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	
	result->transport = NULL;
	result->session_id = NULL;
	result->query = NULL;
	
	uriFreeUriMembersA(&uri);
	
	lsio_debug("lsio_uri_parse: success");
	return result;
}

void lsio_uri_free(lsio_uri_t *uri)
{
	if (NULL == uri)
		return;

	if (uri->scheme)
		free(uri->scheme);
		
	if (uri->host)
		free(uri->host);
		
	if (uri->namespace)
		free(uri->namespace);
	
	if (uri->transport)
		free(uri->transport);
	
	if (uri->session_id)
		free(uri->session_id);
		
	if (uri->query)
		free(uri->query);
		
	free(uri);
	uri = NULL;
}

char *lsio_extract_text_range(UriTextRangeA *range)
{
	char *result;
	
	lsio_debug("lsio_extract_text_range: %lu %lu", 
		(unsigned long) range->first, 
		(unsigned long) range->afterLast);
	
	if (!range->first || !range->afterLast)
		return NULL;
	
	if (NULL == (result = calloc(range->afterLast - range->first + 1,
		sizeof(char))))
		return NULL;
		
	strncpy(result, range->first, range->afterLast - range->first);
	result[range->afterLast - range->first] = '\0';
	
	return result;
}
