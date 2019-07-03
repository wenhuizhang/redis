/***********************************************************/
/*********************RingBuf : FIFO*************************/
/************Lock Free Data Structure***********************/
/***********************************************************/

/* A ring buffer for multi-threaded programs. A single producer pushes
 * objects into the ring, while a single consumer pops them off for
 * processing. If the ring is full (i.e., the consumer should be 
*  slower than the producer), then the push operation fails.
*/


#ifndef RING_H
#define RING_H



// #define     MAXLEN      100


struct ring
{
	int size;
	int pop;
	int push;
	int count;
	void** items;
};


struct consumer
{
	int location;
};

/* Create a ring buffer with the specified size. Return the ring or NULL
if there is not enough memory to create. */
struct ring* init_ring(int size);

struct consumer* init_consumer(int location);

/* Add an entry to the ring.
 * Return producer pointer location
 * We make this always return success
 * Producer continuously write messages 
 * and rewrite old buckets 
 */
int ring_push(struct ring *rb, void* data);


/* Gets an entry from the ring, from an entry location
Return a pointer to the data, or NULL if empty
*/
void* ring_get(struct ring *rb, struct consumer *con);



/* Remove an entry from the ring.
Return a pointer to the data, or NULL if empty
*/
void* ring_pop(struct ring *rb);


void destroy_ring(struct ring *rb);
void destroy_consumer(struct consumer *con);


#endif 
