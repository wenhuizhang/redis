CC = gcc
DEPS1 = rbuf_test.c 
DEPS2 = rbuf_pthread_test.c
DEPS3 = rbuf_threadpool_test.c
DEPS4 = pool_gc_test.c

all: ringbuf_test thread_test thpool_test pool_gc_test

rebuild: clean all

ringbuf_test:
	$(CC) $(DEPS1) -o test ../ringbuf.o ../thpool.o -lpthread

thread_test:
	$(CC) $(DEPS2) -o thread ../ringbuf.o ../thpool.o -lpthread

thpool_test:
	$(CC) $(DEPS3) -o thpool ../ringbuf.o ../thpool.o -lpthread

pool_gc_test:
	$(CC) $(DEPS4) -o poolgc ../ringbuf.o ../thpool.o ../ll.o -lpthread

clean:
	rm -f test thread thpool poolgc
