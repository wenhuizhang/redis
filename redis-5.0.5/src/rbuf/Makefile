CC = gcc
CFLAGS =
DEPS = ringbuf.c
OBJ = ringbuf

all: ringbuf tests

rebuild: clean all

ringbuf:
	$(CC) -c $(DEPS) -o rbuf.o
tests:
	cd test && make
clean:
	rm -f rbuf.o
	cd test && make clean
