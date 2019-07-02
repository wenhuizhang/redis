CC = gcc
DEPS = rbuf_test.c


all: ringbuf_test

rebuild: clean all

ringbuf_test:
	$(CC) $(DEPS) -o test ../rbuf.o

clean:
	rm -f test
