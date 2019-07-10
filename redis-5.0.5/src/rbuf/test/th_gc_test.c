/*****************Garbage Collection Test************************/
// Create threads pool for a certain channel
// One thread for producer
// One thread for one consumers
// After all joined, conduct garbage collection for this channel





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/sysinfo.h>

#include "../ringbuf.h"

#define NUM_THREADS 512
#define NUM_CHANNEL 1


int main(void) {
	/* create thread pool*/
	pthread_t tid[NUM_THREADS];

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
        head = con_first(list);
        tail = con_last(list);

	/* assign producer to a thread*/
	int err = pthread_create(&(tid[0]), NULL, &producer, (void*)&args_p);

	/* assign consumers to threads */
        struct consumer_args *args_c;
        args_c = con_first(list);
	int i = 1;
	while(args_c != NULL){
		if(i >= (NUM_THREADS - 1)){
			printf("Consumer Number %d More than Thread Pool %d, please increase threads\n", i, NUM_THREADS);
			return 1;
		}
		err = pthread_create(&(tid[i]), NULL, &consumer, (void*)args_c);
        	args_c = con_first(list);
		i++;
	}
	
	int threads_join = i-1;
	for(i = 0; i< threads_join; i++){
		pthread_join(tid[i], NULL);
	}

	destroy_ring(ring_buffer);
	con_free(list, NULL);
	//free(tid);

	return 0;

}

