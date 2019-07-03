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


int main() {
    
	pthread_t tid[NUM_THREADS];

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

	err = pthread_create(&(tid[NUM_THREADS-1]), NULL, &producer, (void*)&args_p);

	for(int i = 0; i < NUM_THREADS-1; i++){
		err = pthread_create(&(tid[i]), NULL, &consumer, (void*)&args_c);
	}

	for(int i = 0; i<NUM_THREADS; i++){
		pthread_join(tid[i], NULL);
	}

	destroy_ring(args_p.ring_buffer);
	return 0;

}

