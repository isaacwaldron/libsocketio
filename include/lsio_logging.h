/*
 * libsocketio logging header
 * Copyright (c) 2013 Isaac Waldron
 */

#ifndef __lsio_logging_h__
#define __lsio_logging_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

void lsio_logging_init(void (*lsio_debug_ptr)(const char *, va_list));
void lsio_logging_destroy();

void lsio_debug(const char *, ...)
	__attribute__((format(printf, 1, 2)));

#ifdef __cplusplus
}
#endif

#endif
