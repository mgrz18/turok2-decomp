CC ?= cc
CFLAGS ?= -O2 -Wall

all: n64crc

n64crc: n64crc.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f n64crc

.PHONY: all clean
