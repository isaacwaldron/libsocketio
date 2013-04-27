/*
 * libsocketio URI implementation
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
	
	LSIO_DEBUG("entering");
	
	state.uri = &uri;
	if (URI_SUCCESS != uriParseUriA(&state, uri_string)) {
		LSIO_DEBUG("uriParseUriA returned failure");
		uriFreeUriMembersA(&uri);
		return NULL;
	}
	
	if (NULL == (result = (lsio_uri_t *) malloc(sizeof(lsio_uri_t)))) {
		LSIO_DEBUG("out of memory");
		uriFreeUriMembersA(&uri);
		return NULL;
	}
	
	result->scheme = lsio_extract_text_range(&uri.scheme);
	if (NULL == result->scheme) {
		LSIO_DEBUG("scheme extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	
	result->host = lsio_extract_text_range(&uri.hostText);
	if (NULL == result->host) {
		LSIO_DEBUG("host extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	
	scratch = lsio_extract_text_range(&uri.portText);
	errno = 0;
	port = strtoul(scratch, (char **) NULL, 10);
	free(scratch);
	if (0 != errno || UINT_MAX < port) {
		LSIO_DEBUG("port extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	result->port = (int) port;
	
	segment = uri.pathHead;
	if (NULL == segment) {
		LSIO_DEBUG("missing namespace path segment");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	result->namespace = lsio_extract_text_range(&segment->text);
	if (NULL == result->namespace) {
		LSIO_DEBUG("namespace extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	
	segment = segment->next;
	if (NULL == segment) {
		LSIO_DEBUG("missing version path segment");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	scratch = lsio_extract_text_range(&segment->text);
	errno = 0;
	protocol_version = strtoul(scratch, (char **) NULL, 10);
	free(scratch);
	if (0 != errno || UINT_MAX < protocol_version) {
		LSIO_DEBUG("version extraction failed");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	result->protocol_version = (int) protocol_version;
	
	if (NULL != segment->next && 
		segment->next->text.afterLast != segment->next->text.first) {
		LSIO_DEBUG("unexpected path segment after version");
		uriFreeUriMembersA(&uri);
		lsio_uri_free(result);
		return NULL;
	}
	
	result->transport = NULL;
	result->session_id = NULL;
	result->query = NULL;
	
	uriFreeUriMembersA(&uri);
	
	LSIO_DEBUG("success");
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
	
	LSIO_DEBUG("%lu %lu", (unsigned long) range->first, 
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
