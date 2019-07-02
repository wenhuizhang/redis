/***********************************************************/
/*********************RingBuf : Test*************************/
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



int main() {
    
	const int buffer_size = 5;
	const int start_location = 0;

	int push_size_1 = 8;
	int get_size_1 = 2;
	// int push_size_2 = 6;
	int get_size_2 = 12;

	void* data[push_size_1];
	void* get_data;

	struct ring* ring_buffer = init_ring( buffer_size );
	struct consumer *con_1 = init_consumer( start_location );
	struct consumer *con_2 = init_consumer( start_location );

	int i;

	for( i = 0; i < push_size_1 ; i++){

		data[i] = malloc(sizeof(int));

		int count = ring_push(ring_buffer, &data[i]);

		if ( count != 0 ){
			printf("i = %d, push data %d = %p \n", i, count, &data[i]);
		}
		else{
			printf("i = %d, push %d failed \n ", i, count);
		}
	}

	for( i = 0; i < get_size_1 ; i++){
		
		get_data = ring_get(ring_buffer, con_1);

		if(get_data == NULL){
			printf("i = %d, get %d failed \n" , i, (con_1->location+1));
		}
		else{
			printf("i = %d, get data %d = %p\n", i, con_1->location, get_data);
		}
	}

/*

	for( i = 0; i < push_size_2  ; i++){

		data[i] = malloc(sizeof(int));

		int count = ring_push(ring_buffer, &data[i]);

		if ( count != 0 ){
			printf("push data %d = %p \n", i, &data[i]);
		}
		else{
			printf("push %d failed \n ", i);
		}
	}
*/
	for( i = 0; i < get_size_2  ; i++){
		
		get_data = ring_get(ring_buffer, con_2);
		printf("location is %d\n", con_2->location);

		if(get_data == NULL){
			printf("i = %d, get  %d failed \n" , i, (con_2->location+1));
		}
		else{
			printf("i = %d, get data %d = %p\n", i, con_2->location, get_data);
		}
	}

	destroy_ring(ring_buffer);
	destroy_consumer(con_1);
	destroy_consumer(con_2);

	return 0;

}

