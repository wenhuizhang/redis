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

struct consumer *init_consumer(int location)
{
	struct consumer *con = (struct consumer*)malloc(sizeof(struct consumer ));
	con->location = location;
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

	return rb->count;
}


/* Gets an entry from the ring, from an entry location
 * Return a pointer to the data, or NULL if empty
*/
void* ring_get(struct ring *rb, struct consumer *con)
{
	while((rb->count - con->location) > rb->size){
		// printf("buffer size %d, too small\n", rb->size);
		con->location = con->location + rb->size;
	}
	
	if ( rb->count <= 0 ){
		return NULL;
	}
	
	if( con->location >= rb->count){
		printf("overread\n");
		return NULL;
	}
	
	void* temp;
	int get_location = con->location % rb->size;
	temp = rb->items[get_location];

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


/*

int main() {
    
	// set buffer size as 5, first push 3 elements
	// and then get 2 elements,
	// then push 6 elements, at last get 12 elements
	const int buffer_size = 5;

	int push_size_1 = 3;
	int get_size_1 = 2;
	int push_size_2 = 6;
	int get_size_2 = 12;

	void* data[buffer_size];
	void* get_data;

	struct ring* ring_buffer = init_ring( buffer_size );

	int i;

	for( i = 0; i < push_size_1  ; i++){

		data[i] = malloc(sizeof(int));

		int count = ring_push(ring_buffer, &data[i]);

		if ( count != 0 ){
			printf("push data %d = %d \n", i, &data[i]);
		}
		else{
			printf("push %d failed \n ", i);
		}
	}

	for( i = 0; i < get_size_1  ; i++){
		get_data = ring_get(ring_buffer, i);

		if(get_data == NULL){
			printf("get %d failed \n" , i);
		}
		else{
			printf("get data %d = %d\n", i, get_data);
		}
	}

	for( i = 0; i < push_size_2  ; i++){

		data[i] = malloc(sizeof(int));

		int count = ring_push(ring_buffer, &data[i]);

		if ( count != 0 ){
			printf("push data %d = %d \n", i, &data[i]);
		}
		else{
			printf("push %d failed \n ", i);
		}
	}

	for( i = 0; i < get_size_2  ; i++){
		get_data = ring_get(ring_buffer, i);

		if(get_data == NULL){
			printf("get  %d failed \n" , i);
		}
		else{
			printf("get data %d = %d\n", i, get_data);
		}
	}

	return 0;

}
*/
