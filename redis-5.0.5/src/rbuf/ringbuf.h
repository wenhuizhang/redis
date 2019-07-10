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

#include <pthread.h>
#include <sys/sysinfo.h>

#define MAXBUF  10000000
#define MAX_THREADS (2*get_nprocs())



struct consumer_args{
        struct ring* ring_buffer;
        int start_location;
        int get_size;
        int consumer_id;
        int channel_id;
	struct consumer_args *next;
	struct consumer_args *last;
};

struct list_consumers{
	struct consumer_args *head;
	struct consumer_args *tail;
	int size;
};

struct producer_args{
        struct ring* ring_buffer;
        int producer_id;
        int channel_id;
	struct producer_args *next;
	struct producer_args *last;
};

struct list_producer{
	struct producer_args *head;
	struct producer_args *tail;
	int size;
};



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

void *producer(void *arg);
void *consumer(void *arg);







static volatile int threads_keepalive;
static volatile int threads_on_hold;


/* Binary semaphore */
typedef struct bsem {
	pthread_mutex_t mutex;
	pthread_cond_t   cond;
	int v;
} bsem;


/* Job */
typedef struct job{
	struct job*  prev;                   /* pointer to previous job   */
	void   (*function)(void* arg);       /* function pointer          */
	void*  arg;                          /* function's argument       */
} job;


/* Job queue */
typedef struct jobqueue{
	pthread_mutex_t rwmutex;             /* used for queue r/w access */
	job  *front;                         /* pointer to front of queue */
	job  *rear;                          /* pointer to rear  of queue */
	bsem *has_jobs;                      /* flag as binary semaphore  */
	int   len;                           /* number of jobs in queue   */
} jobqueue;


/* Thread */
typedef struct thread{
	int       id;                        /* friendly id               */
	pthread_t pthread;                   /* pointer to actual thread  */
	struct thpool_* thpool_p;            /* access to thpool          */
} thread;


typedef struct thpool_{
	thread**   threads;                  /* pointer to threads        */
	volatile int num_threads_alive;      /* threads currently alive   */
	volatile int num_threads_working;    /* threads currently working */
	pthread_mutex_t  thcount_lock;       /* used for thread count etc */
	pthread_cond_t  threads_all_idle;    /* signal to thpool_wait     */
	jobqueue  jobqueue;                  /* job queue                 */
} thpool_;



static int  thread_init(thpool_* thpool_p, struct thread** thread_p, int id);
static void* thread_do(struct thread* thread_p);
static void  thread_hold(int sig_id);
static void  thread_destroy(struct thread* thread_p);

static int   jobqueue_init(jobqueue* jobqueue_p);
static void  jobqueue_clear(jobqueue* jobqueue_p);
static void  jobqueue_push(jobqueue* jobqueue_p, struct job* newjob_p);
static struct job* jobqueue_pull(jobqueue* jobqueue_p);
static void  jobqueue_destroy(jobqueue* jobqueue_p);

static void  bsem_init(struct bsem *bsem_p, int value);
static void  bsem_reset(struct bsem *bsem_p);
static void  bsem_post(struct bsem *bsem_p);
static void  bsem_post_all(struct bsem *bsem_p);
static void  bsem_wait(struct bsem *bsem_p);


/**
 * Initialize threadpool
 *
 * Initializes a threadpool. This function will not return untill all
 * threads have initialized successfully.
 */
typedef struct thpool_* threadpool;
threadpool thpool_init(int num_threads);


/**
 * Add work to the job queue
 *
 * Takes an action and its argument and adds it to the threadpool's job queue.
 * If you want to add to work a function with more than one arguments then
 * a way to implement this is by passing a pointer to a structure.
 *
 */
int thpool_add_work(threadpool, void (*function_p)(void*), void* arg_p);


/**
 * Wait for all queued jobs to finish
 *
 * Will wait for all jobs - both queued and currently running to finish.
 * Once the queue is empty and all work has completed, the calling thread
 * (probably the main program) will continue.
 *
 * Smart polling is used in wait. The polling is initially 0 - meaning that
 * there is virtually no polling at all. If after 1 seconds the threads
 * haven't finished, the polling interval starts growing exponentially
 * untill it reaches max_secs seconds. Then it jumps down to a maximum polling
 */


void thpool_wait(threadpool);


/**
 * Pauses all threads immediately
 *
 * The threads will be paused no matter if they are idle or working.
 * The threads return to their previous states once thpool_resume
 * is called. While the thread is being paused, new work can be added.
 */
void thpool_pause(threadpool);


/**
 * Unpauses all threads if they are paused
 */
void thpool_resume(threadpool);


/**
 * Destroy the threadpool
 *
 * This will wait for the currently active threads to finish and then 'kill'
 * the whole threadpool to free up memory.
 */
void thpool_destroy(threadpool);


/**
 * Show currently working threads
 *
 * Working threads are the threads that are performing work (not idle).
 */
int thpool_num_threads_working(threadpool);



/***********************************************************/
/*********************Linkedlist : FIFO*********************/
/************FIFO for Consumer and Producer ****************/
/***********************************************************/

/* A linked list for holding consumers 
* and producers for a specific channel
*/



struct list_consumers* con_init(void);

struct consumer_args* con_create_node(struct ring* ringbuf, int location, int size);

int con_get_size(struct list_consumers *list);

void con_append(struct list_consumers *list, struct consumer_args *node);

void con_prepend(struct list_consumers *list, struct consumer_args *node);

struct consumer_args* con_first(struct list_consumers *list);

struct consumer_args* con_first_peek(struct list_consumers *list);

struct consumer_args* con_last(struct list_consumers *list);

struct consumer_args* con_last_peek(struct list_consumers *list);

void con_print(struct list_consumers *list, void(*print_node)(struct consumer_args*));

void con_remove(struct list_consumers *list, struct consumer_args *node);

void con_clear(struct list_consumers *list, void(*free_node)(struct consumer_args*));

void con_free(struct list_consumers *list, void(*free_node)(struct consumer_args*));


#endif 
