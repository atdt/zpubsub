prefix		= /usr/local
exec_prefix	= $(prefix)
bindir		= $(exec_prefix)/bin
CFLAGS		= -Wall -O3 --std=c99 -pedantic -I. -I/opt/local/include
LIBS		= -lczmq
PREFIX		?= /usr/local
INSTALL_BIN 	= $(PREFIX)/bin
INSTALL		= cp -pf



all: zpubsub

zpubsub:
	$(CC) $(LDFLAGS) $(CFLAGS) zpub.c -o zpub $(LIBS)
	$(CC) $(LDFLAGS) $(CFLAGS) zsub.c -o zsub $(LIBS)

install: all
	mkdir -p $(INSTALL_BIN)
	$(INSTALL) zpub $(INSTALL_BIN)
	$(INSTALL) zsub $(INSTALL_BIN)

clean:
	rm -f zpub zsub
