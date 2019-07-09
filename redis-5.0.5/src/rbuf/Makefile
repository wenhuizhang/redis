CC = gcc
CFLAGS = -lpthread
DEPS_BUF = ringbuf.c
DEPS_TH = thpool.c
DEPS_CONLS = ll.c
OBJ_BUF = ringbuf.o 
OBJ_TH = thpool.o 
OBJ_CONLS = ll.o
OBJ = rbuf.o

all: ringbuf tests

rebuild: clean all

ringbuf:
	$(CC) -o $(OBJ_BUF) -c $(DEPS_BUF) $(CFLAGS)
	$(CC) -o $(OBJ_CONLS) -c $(DEPS_CONLS) $(CFLAGS)
	$(CC) -o $(OBJ_TH) -c $(DEPS_TH) $(CFLAGS) -D THPOOL_DEBUG
tests:
	cd test && make
clean:
	rm -f *.o
	cd test && make clean
