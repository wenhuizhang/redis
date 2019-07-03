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


#define	MAXBUF	10000000
#define	NUM_THREADS (2*get_nprocs())



struct consumer_args{
	struct ring* ring_buffer;
	int start_location;
	int get_size;
};


struct producer_args{
	struct ring* ring_buffer;
};




void *producer(void *arg)
{
	struct producer_args* args = (struct producer_args*) arg;
	int data_size = 8;
	void* data[data_size];
	for(int j = 0; j < data_size; j++){
		data[j] = malloc(sizeof(int));
	}
	int i = 0;
	while(1){

		int count = ring_push(args->ring_buffer, &data[i%data_size]);
		printf("i = %d, push data %d = %p \n", i, count, &data[i%data_size]);
		i++;
	}
	return NULL;	
}

void *consumer(void *arg)
{
	struct consumer_args* args = (struct consumer_args*)arg;
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

	const int buffer_size = 100000;
	const int start_location = 0;

	int get_size_1 = 2;
	int get_size_2 = 12;
	
	struct producer_args args_p;
	args_p.ring_buffer = init_ring( buffer_size );

	// printf("Configured processor %d\n", get_nprocs_conf());
	// printf("Available processor %d\n", get_nprocs());
		
	int err = pthread_create(&(tid[NUM_THREADS-1]), NULL, &producer, (void*)&args_p);

	struct consumer_args args_c;
	args_c.ring_buffer = args_p.ring_buffer;
	args_c.start_location = start_location;
	args_c.get_size = get_size_1;

	for(int i = 0; i < NUM_THREADS-1; i++){
		err = pthread_create(&(tid[i]), NULL, &consumer, (void*)&args_c);
	}


	for(int i = 0; i<NUM_THREADS; i++){
		pthread_join(tid[i], NULL);
	}

	destroy_ring(args_p.ring_buffer);
	return 0;

}

