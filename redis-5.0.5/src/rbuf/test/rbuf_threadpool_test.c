/***********************************************************/
/*************RingBuf : Thread Pool Test********************/
/***********************************************************/

/* A ring buffer for multi-threaded programs. A single producer pushes
 * objects into the ring, while a single consumer pops them off for
 * processing. If the ring is full (i.e., the consumer should be 
*  slower than the producer), then the push operation fails.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/sysinfo.h>

#include "../ringbuf.h"

#define NUM_CONSUMER 3
#define NUM_THREAD 4

int main() {

	/* Thread Pool handling*/

	// threadpool thpool = thpool_init(MAX_THREADS);
	threadpool thpool = thpool_init(NUM_THREAD);

	const int buffer_size = 1000000;
	const int start_location = 0;

	int get_size = 1000000;
	
	struct producer_args args_p;
	args_p.ring_buffer = init_ring( buffer_size );

	struct consumer_args args_c;
	args_c.ring_buffer = args_p.ring_buffer;
	args_c.start_location = start_location;
	args_c.get_size = get_size;

	thpool_add_work(thpool, (void*)producer, &args_p);
	
	int i;
	for(i = 0; i < NUM_CONSUMER; i++){
		thpool_add_work(thpool, (void*)consumer, &args_c);
	}

	int j;
	for (j=0; j < NUM_THREAD; j++){
		pthread_join(thpool->threads[j]->pthread, NULL);
	}
	
	thpool_destroy(thpool);

	return 0;

}

