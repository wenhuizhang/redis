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


#define	MAXLEN	100
#define	NUM_THREADS (2*get_nprocs())



struct pthread_args{
	struct ring* ring_buffer;
	int start_location;
	int get_size;
};




int producer(struct ring* ring_buffer, int push_size)
{
	void* data[push_size];
	for( int i = 0; i < push_size ; i++){

		data[i] = malloc(sizeof(int));

		int count = ring_push(ring_buffer, &data[i]);

		if ( count != 0 ){
			printf("i = %d, push data %d = %p \n", i, count, &data[i]);
		}
		else{
			printf("i = %d, push %d failed \n ", i, count);
		}
	}

	return 0;	
}

void *consumer(void *arg)
{
	struct pthread_args* args = (struct pthread_args*)arg;
	struct consumer *cons = init_consumer( args->start_location );
	void* get_data;
	get_data = ring_get(args->ring_buffer, cons);
	printf("get data %d = %p\n", cons->location, get_data);
	while(get_data != NULL){
		get_data = ring_get(args->ring_buffer, cons);
		printf("get data %d = %p\n", cons->location, get_data);
	}
	destroy_consumer(cons);
	return NULL;
}




int main() {
    
	pthread_t tid[NUM_THREADS];

	const int buffer_size = 5;
	const int start_location = 0;

	int push_size = 8;
	int get_size_1 = 2;
	// int push_size_2 = 6;
	int get_size_2 = 12;
	
	struct ring* ring_buffer = init_ring( buffer_size );

	// printf("Configured processor %d\n", get_nprocs_conf());
	// printf("Available processor %d\n", get_nprocs());
		
	int err = producer(ring_buffer, push_size);

	struct pthread_args args_1;
	args_1.ring_buffer = ring_buffer;
	args_1.start_location = start_location;
	args_1.get_size = get_size_1;

	for(int i = 0; i < NUM_THREADS; i++){
		err = pthread_create(&(tid[i]), NULL, &consumer, (void*)&args_1);
	}


	for(int i = 0; i<NUM_THREADS; i++){
		pthread_join(tid[i], NULL);
	}

	destroy_ring(ring_buffer);
	return 0;

}

