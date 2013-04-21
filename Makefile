# libsocketio Makefile
# Copyright (c) 2013 Isaac Waldron

CC=gcc

OBJDIR=obj
OBJS=lsio_logging.o \
	 lsio_message.o \
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

TESTS=lsio_uri_parse_test lsio_message_parse_test

DIRS=$(BINDIR) $(LIBDIR) $(OBJDIR)

.PHONY: dirs clean test

$(LIBDIR)/$(LIBNAME): $(LIBDIR)/$(LIBNAME_FULL)
	-ln -s $(LIBNAME_FULL) $@

$(LIBDIR)/$(LIBNAME_FULL): $(addprefix $(OBJDIR)/, $(OBJS))
	$(CC) -shared -o $@ $^ -luriparser
	
dirs:
	-mkdir $(DIRS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDEDIR)/*.h dirs
	$(CC) -c -o $@ $< -I $(INCLUDEDIR) -fPIC

test: $(addprefix $(BINDIR)/, $(TESTS))

$(BINDIR)/%: $(TESTDIR)/%.c $(INCLUDEDIR)/*.h dirs
	$(CC) -o $@ $< -I $(INCLUDEDIR) -L $(LIBDIR) -lsocketio

clean:
	-rm $(BINDIR)/*
	-rm $(LIBDIR)/*
	-rm $(OBJDIR)/*
	-rmdir $(DIRS)
