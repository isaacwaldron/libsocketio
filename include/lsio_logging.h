/*
 * libsocketio logging header
 * Copyright (c) 2013 Isaac Waldron
 */

#ifndef LSIO_LOGGING_H_INCLUDED
#define LSIO_LOGGING_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define LSIO_DEBUG(format, ...) lsio_debug("%s: " format, __func__, ## \
	__VA_ARGS__)

#include <stdarg.h>

void lsio_logging_init(void (*lsio_debug_ptr)(const char *, va_list));
void lsio_logging_destroy();

void lsio_debug(const char *, ...)
	__attribute__((format(printf, 1, 2)));

#ifdef __cplusplus
}
#endif

#endif
