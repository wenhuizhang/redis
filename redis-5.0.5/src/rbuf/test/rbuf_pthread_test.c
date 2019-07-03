/***********************************************************/
/*************RingBuf : Multithread Test********************/
/************Lock Free Data Structure***********************/
/***********************************************************/

/* A ring buffer for multi-threaded programs. A single producer pushes
 * objects into the ring, while a single consumer pops them off for
 * processing. If the ring is full (i.e., the consumer should be 
*  slower than the producer), then the push operation fails.
*/



/* 
 * a test consol to see if ringbuffer's exceptions are working fine
 * with exceeding push and pop options
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/sysinfo.h>

#include "../ringbuf.h"

#define NUM_CONSUMER 1


int main() {
/*****************TODO************************/
// args_c.consumer_id%numThreads
// one thread for write
// other threads are for read
// one thread for one read
// if there are more consumer than threads left
// use mode value for thread assign

	int numThreads;

	if(NUM_CONSUMER <= (MAX_THREADS-1)){
		numThreads = NUM_CONSUMER;
	}else{
		numThreads = MAX_THREADS-1 ;
	}

	pthread_t tid[numThreads+1];

	const int buffer_size = 1000000;
	const int start_location = 0;

	int get_size = 12000;
	int err;
	
	struct producer_args args_p;
	args_p.ring_buffer = init_ring( buffer_size );

	struct consumer_args args_c;
	args_c.ring_buffer = args_p.ring_buffer;
	args_c.start_location = start_location;
	args_c.get_size = get_size;

	err = pthread_create(&(tid[numThreads]), NULL, &producer, (void*)&args_p);

	for(int i = 0; i < numThreads; i++){
		err = pthread_create(&(tid[i]), NULL, &consumer, (void*)&args_c);
	}

	for(int i = 0; i<MAX_THREADS; i++){
		pthread_join(tid[i], NULL);
	}

	destroy_ring(args_p.ring_buffer);
	return 0;

}

