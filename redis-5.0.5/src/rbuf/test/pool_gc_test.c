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
#include <unistd.h>

#include <pthread.h>
#include <sys/sysinfo.h>

#include "../ringbuf.h"

#define NUM_THREAD 4

int main(void) {

	/* init thread pool for this channel*/
	threadpool thpool = thpool_init(NUM_THREAD);

	int buffer_size = 100000000;

	int get_size = buffer_size;

	/* init the ring buffer for this channel*/
	struct ring* ring_buffer = init_ring( buffer_size );

	/* one producer for this channel*/
	struct producer_args args_p;
	/* init the ring buffer for this channel*/
	args_p.ring_buffer = ring_buffer;

	/* list of consumers for this channel*/
	struct list_consumers *list;
    	struct consumer_args *node, *node2, *node3;
    	struct consumer_args *head, *tail;


	list = con_init();
    	node = con_create_node(ring_buffer, 1, buffer_size);
    	node2 = con_create_node(ring_buffer, 10, buffer_size);
    	node3 = con_create_node(ring_buffer, 100, buffer_size);


    	con_append(list, node);
    	con_append(list, node2);
    	con_append(list, node3);
//    	head = con_first(list);
//    	tail = con_last(list);


	/* add producer to job tank */
	thpool_add_work(thpool, (void*)producer, &args_p);


	/* add consumers to job tank */
	struct consumer_args *args_c;
	args_c = con_first(list);


	while(args_c != NULL) {
		thpool_add_work(thpool, (void*)consumer, args_c);
		args_c = con_first(list);
		printf("in while %s: LINE=%d\n", __func__, __LINE__);
	}

	for (int j=0; j < NUM_THREAD; j++){
		pthread_join(thpool->threads[j]->pthread, NULL);
	}
	

	thpool_destroy(thpool);
	con_free(list, NULL);
	destroy_ring(ring_buffer);

	return 0;

}

