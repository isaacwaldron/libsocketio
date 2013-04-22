# libsocketio Makefile
# Copyright (c) 2013 Isaac Waldron

CC=gcc
CFLAGS=-I /usr/local/include
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
