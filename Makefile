# libsocketio Makefile
# Copyright (c) 2013 Isaac Waldron

CC=gcc

OBJDIR=obj
OBJS=lsio_socket.o lsio_transport.o

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

TESTS=

DIRS=$(BINDIR) $(LIBDIR) $(OBJDIR)

.PHONY: dirs clean test

$(LIBDIR)/$(LIBNAME): $(LIBDIR)/$(LIBNAME_FULL)
	-ln -s $(LIBNAME_FULL) $@

$(LIBDIR)/$(LIBNAME_FULL): $(addprefix $(OBJDIR)/, $(OBJS))
	$(CC) -shared -o $@ $^
	
dirs:
	-mkdir $(DIRS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDEDIR)/*.h dirs
	$(CC) -c -o $@ $< -I $(INCLUDEDIR) -fPIC

test: $(BINDIR)/$(TESTS)

$(BINDIR)/%: $(TESTDIR)/%.c $(INCLUDEDIR)/*.h dirs
	$(CC) -o $@ $< -I $(INCLUDEDIR) -L $(LIBDIR) -lds

clean:
	-rm $(BINDIR)/*
	-rm $(LIBDIR)/*
	-rm $(OBJDIR)/*
	-rmdir $(DIRS)
