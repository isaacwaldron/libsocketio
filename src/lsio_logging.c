/*
 * libsocketio logging implementation
 * Copyright (c) 2013 Isaac Waldron
 */

#include <stdarg.h>
#include <stdlib.h>

#include "lsio_logging.h"

void (*lsio_debug_func)(const char *, va_list) = NULL;

void lsio_logging_init(void (*lsio_debug_ptr)(const char *, va_list))
{
	lsio_debug_func = lsio_debug_ptr;
}

void lsio_logging_destroy()
{
	lsio_debug_func = NULL;
}

void lsio_debug(const char *fmt, ...)
{
	va_list arg;
	if (lsio_debug_func) {
		va_start(arg, fmt);
		lsio_debug_func(fmt, arg);
		va_end(arg);
	}
}
