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
#include "../ringbuf.h"

#define     MAXLEN      100



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



void consumer(struct ring* ring_buffer, int start_location, int get_size)
{	
	struct consumer *cons = init_consumer( start_location );
	void* get_data;
	for( int i = 0; i < get_size ; i++){
		get_data = ring_get(ring_buffer, cons);

		if(get_data == NULL){
			printf("i = %d, get %d failed \n" , i, (cons->location+1));
			// return NULL;
		}
		else{
			printf("i = %d, get data %d = %p\n", i, cons->location, get_data);
			// return get_data;
		}
	}
	destroy_consumer(cons);
}




int main() {
    
	const int buffer_size = 5;
	const int start_location = 0;

	int push_size = 8;
	int get_size_1 = 2;
	// int push_size_2 = 6;
	int get_size_2 = 12;
	
	struct ring* ring_buffer = init_ring( buffer_size );
	
	int err = producer(ring_buffer, push_size);
	consumer(ring_buffer, start_location, get_size_1);
	consumer(ring_buffer, start_location, get_size_2);

	destroy_ring(ring_buffer);
	return 0;

}

