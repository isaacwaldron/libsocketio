# libsocketio Makefile
# 
# Copyright (c) 2013, Isaac Waldron
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions 
# are met:
# 
# 1. Redistributions of source code must retain the above copyright 
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright 
#    notice, this list of conditions and the following disclaimer in 
#    the documentation and/or other materials provided with the 
#    distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
# POSSIBILITY OF SUCH DAMAGE.

CC=gcc
CFLAGS=-g -Wall -I /usr/local/include
LDFLAGS=-L /usr/local/lib

OBJDIR=obj
OBJS=lsio_logging.o \
	 lsio_packet.o \
	 lsio_socket.o \
	 lsio_transport.o \
	 lsio_uri.o

LIBDIR=lib
LIBVER_MAJOR=1
LIBVER_MINOR=0
LIBVER_PATCH=0
LIBNAME=libsocketio.so
LIBNAME_FULL=$(LIBNAME).$(LIBVER_MAJOR).$(LIBVER_MINOR).$(LIBVER_PATCH)

BINDIR=bin
SRCDIR=src
INCLUDEDIR=include
TESTDIR=test

TESTS=lsio_uri_parse_test lsio_packet_parse_test

DIRS=$(BINDIR) $(LIBDIR) $(OBJDIR)

.PHONY: dirs clean test

$(LIBDIR)/$(LIBNAME): $(LIBDIR)/$(LIBNAME_FULL)
	-ln -s $(LIBNAME_FULL) $@

$(LIBDIR)/$(LIBNAME_FULL): $(addprefix $(OBJDIR)/, $(OBJS))
	$(CC) -shared -o $@ $^ $(LDFLAGS) -luriparser
	
dirs:
	-mkdir $(DIRS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDEDIR)/*.h dirs
	$(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDEDIR) -fPIC

test: $(addprefix $(BINDIR)/, $(TESTS))

$(BINDIR)/%: $(TESTDIR)/%.c $(INCLUDEDIR)/*.h dirs
	$(CC) -o $@ $< -I $(INCLUDEDIR) $(LDFLAGS) -L $(LIBDIR) -lsocketio

clean:
	-rm $(BINDIR)/*
	-rm $(LIBDIR)/*
	-rm $(OBJDIR)/*
	-rmdir $(DIRS)
