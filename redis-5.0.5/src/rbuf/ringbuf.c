/***********************************************************/
/*********************RingBuf : FIFO*************************/
/************Lock Free Data Structure***********************/
/***********************************************************/

/* A ring buffer for multi-threaded programs. A single producer pushes
 * objects into the ring, while a single consumer pops them off for
 * processing. If the ring is full (i.e., the consumer should be 
*  slower than the producer), then the push operation fails.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <sys/sysinfo.h>

#include "ringbuf.h"


/* Create a ring buffer with the specified size. Return the ring or NULL
if there is not enough memory to create. */
struct ring* init_ring(int size)
{
	struct ring *rb = (struct ring*)malloc(sizeof(struct ring ));

	if (rb == NULL) {
		printf("RINGBUF: OUT OF MEM\n");
		return NULL;
	}

	rb->size = size;
	rb->pop = 0;
	rb->push = 0;
	rb->count = 0;

	rb->items = (void**)malloc(size * sizeof(void*) );

	if (rb->items == NULL) {
		printf("ITEM: OUT OF MEM\n");
		return NULL;
	}

	return rb;
};

struct consumer *init_consumer(int loc)
{
	struct consumer *con = (struct consumer*)malloc(sizeof(struct consumer ));
	con->location = loc;
	return con;
}

/* Add an entry to the ring.
 * Return producer pointer location
 * We make this always return success
 * Producer continuously write messages 
 * and rewrite old buckets 
 */
int ring_push(struct ring *rb, void* data)
{
	rb->count = rb->count +1;
	rb->items[ rb->push ] = data;
	rb->push = (rb->push + 1) % rb->size;

	return rb->push;
}


/* Gets an entry from the ring, from an entry location
 * Return a pointer to the data, or NULL if empty
*/
void* ring_get(struct ring *rb, struct consumer *con)
{

//	printf("%s: con=%p, rb=%p\n", __func__, con, rb);
	
	if ( rb->count < 0 ){
		return NULL;
	}
	
	if ( con->location < 0 ){
		return NULL;
	}
	

	if((rb->count - con->location) > rb->size){
		printf("buffer size %d, too small\n", rb->size);
		con->location = rb->count - rb->size + (rb->size / 2);
	}
	
	if( con->location >= rb->count){
		// printf("overread\n");
		con->location = rb->count;
		usleep(1); // sleep in microsecond
		// return NULL;
		// con->location = con->location/2;
		return ring_get(rb, con); 
			
	}
	
	void* temp;
	int get_location = con->location % rb->size;
	printf("%s: LINE=%d, get_location=%d\n", __func__, __LINE__, get_location);
	temp = rb->items[get_location];
	con->location = con->location + 1;
	return  temp;
}



/* Remove an entry from the ring.
Return a pointer to the data, or NULL if empty
*/
void* ring_pop(struct ring *rb)
{
	if ( rb->count <= 0 ){
		return NULL;
	}
	else{   
		void* temp;
		temp = rb->items[ rb->pop ];
		rb->pop = (rb->pop + 1) % rb->size;
		rb->count = rb->count - 1; 

		return  temp;
	}
}






void destroy_ring(struct ring *rb)
{
    void *tmp = rb->items;
    free(tmp);
    free(rb->items);
    free(rb);
}


void destroy_consumer(struct consumer *con)
{
    free(con);
}


void *producer(void *arg)
{
        struct producer_args* args = (struct producer_args*) arg;

	/* struct fake data */
        int data_size = 8;
        void* data[data_size];
        for(int j = 0; j < data_size; j++){
                data[j] = malloc(sizeof(int));
        }
	
	/* before writing, send signal as producer is active*/ 	
	args->status = 0;
	/* feed in data */
	
	int i = 0;
        while(1){
                int count = ring_push(args->ring_buffer, &data[i%data_size]);
		printf("i = %d, push data %d = %p \n", i, count, &data[i%data_size]);
                i++;
        }


	/* feed in data test */
//	int i = 0;
//      while(i<20){
//                int count = ring_push(args->ring_buffer, &data[i%data_size]);
//		  printf("i = %d, push data %d = %p \n", i, count, &data[i%data_size]);
//                i++;
//        }

	/* after finish writing, send signal as not active*/ 
	args->status = 1;

        return NULL;
}

void *consumer(void *arg)
{
        struct consumer_args* args = (struct consumer_args*)arg;
        struct consumer *cons = init_consumer( args->start_location );
        void* get_data;
	
	/* before reading, send signal as producer is active*/ 	
	args->status = 0;
        
	get_data = ring_get(args->ring_buffer, cons);
        printf("get data %d = %p\n", cons->location, get_data);
        while(get_data != NULL){
                get_data = ring_get(args->ring_buffer, cons);
                printf("get data %d = %p\n", cons->location, get_data);
        }


	/* after finish reading, send signal as not active*/ 
	args->status = 1;

        destroy_consumer(cons);
        return NULL;
}

