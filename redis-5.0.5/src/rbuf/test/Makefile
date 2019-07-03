CC = gcc
DEPS1 = rbuf_test.c 
DEPS2 = rbuf_pthread_test.c

all: ringbuf_test thread_test

rebuild: clean all

ringbuf_test:
	$(CC) $(DEPS1) -o test ../rbuf.o

thread_test:
	$(CC) $(DEPS2) -o thread ../rbuf.o -lpthread

clean:
	rm -f test thread
