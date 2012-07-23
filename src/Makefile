CFLAGS		= -Wall -O3 --std=c99 
LIBS		= -lczmq
PREFIX		?= /usr/local
INSTALL_BIN 	= $(PREFIX)/bin
INSTALL		= cp -pf
MAN1DIR         ?= $(PREFIX)/man/man1
BUILDDIR        ?= ../build



all:
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) zpub.c -o $(BUILDDIR)/zpub $(LIBS)

install: all
	mkdir -p $(INSTALL_BIN)
	$(INSTALL) zpub $(INSTALL_BIN)
	$(INSTALL) ../man/zpubsub.1 $(MAN1DIR)

dist:
	mkdir -p $(BUILDDIR)
	tar cvzf $(BUILDDIR)/zpubsub_$(REV).tar.gz .. --exclude=$(BUILDDIR) --exclude=.git*

clean:
	rm -rf $(BUILDDIR)
