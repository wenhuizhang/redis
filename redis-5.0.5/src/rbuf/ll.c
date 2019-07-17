/***********************************************************/
/*********************Linkedlist : FIFO*********************/
/************FIFO for Consumer and Producer ****************/
/***********************************************************/

/* A linked list for holding consumers 
* and producers for a specific channel
*/



# include <stdlib.h>
# include <stdio.h>
# include <assert.h>
# include "ringbuf.h"


struct list_consumers*
con_init(void)
{
        struct list_consumers *list = malloc(sizeof(struct list_consumers));

        if (!list) return NULL;

        list->head = NULL;
        list->tail = NULL;
        list->size = 0;

        return list;
}

struct consumer_args*
con_create_node(struct ring* ringbuf, int location, int size)
{
        struct consumer_args *node = malloc(sizeof(struct consumer_args));

        if (!node) return NULL;

	node->ring_buffer = ringbuf;
	node->start_location = location;
	node->get_size = size; 
        node->next = NULL;
        node->last = NULL;

        return node;
}

int
con_get_size(struct list_consumers *list)
{
        return list->size;
}

void
con_append(struct list_consumers *list, struct consumer_args *node)
{
        node->last = list->tail;
        if (node->last) node->last->next = node;

        node->next = NULL;
        list->tail = node;

        if (!list->head) list->head = node;

        list->size++;
}

void
con_prepend(struct list_consumers *list, struct consumer_args *node)
{
        list->head->last = node;
        node->next = list->head;
        list->head = node;
        list->size++;
}

struct consumer_args*
con_first(struct list_consumers *list)
{
        struct consumer_args *node;
	
	assert(list != NULL);

	node = list->head;

        if (node){
                list->head = node->next;
		printf("list->head=%p\n", list->head);
		if(list->head){
                	list->head->last = NULL;
		}	
                list->size--;
        }

        return node;
}

struct consumer_args*
con_first_peek(struct list_consumers *list)
{
        return list->head;
}

struct consumer_args*
con_last(struct list_consumers *list)
{
        struct consumer_args *node = list->tail;

        if (node){
                list->tail = node->last;
                list->tail->next = NULL;
                list->size--;
        }

        return node;
}

struct consumer_args*
con_last_peek(struct list_consumers *list)
{
        return list->tail;
}

void
con_print(struct list_consumers *list, void(*print_node)(struct consumer_args*))
{
        printf("List has %d elements\n", list->size);

        struct consumer_args *node = list->head;

        while(node) {
                (*print_node)(node);
                printf("->");
                node = node->next;
        }

        printf("\n");
}

void
con_remove(struct list_consumers *list, struct consumer_args *node)
{
        if (node->last) node->last->next = node->next;
        else list->head = node->next; // this node is the head of the list

        if (node->next) node->next->last = node->last;
        else list->tail = node->last; // this node is the tail

        list->size--;

        node->next = NULL;
        node->last = NULL;
}

void
con_clear(struct list_consumers *list, void(*free_node)(struct consumer_args*))
{
        struct consumer_args *temp;
        struct consumer_args *node = list->head;

        while (node) {
                temp = node->next;
                if (free_node) (*free_node)(node->next);
                free(node);
                node = temp;
        }

        list->size = 0;
        list->head = NULL;
        list->tail = NULL;
}

void
con_free(struct list_consumers *list, void(*free_node)(struct consumer_args*))
{
        con_clear(list, free_node);
        free(list);
}

/*
int main(void) {
    struct list_consumers *list;
    struct consumer_args *node, *node2, *node3;
    struct consumer_args *head, *tail;


    int buffer_size = 100000;
    
    struct producer_args args_p;
    args_p.ring_buffer = init_ring( buffer_size );

    printf("Testing linkedlist...\n");

    list = con_init();
    node = con_create_node(args_p.ring_buffer, 10, buffer_size);
    node2 = con_create_node(args_p.ring_buffer, 1, buffer_size);
    node3 = con_create_node(args_p.ring_buffer, 100, buffer_size);
    con_append(list, node);
    con_append(list, node2);
    con_prepend(list, node3);
    head = con_first(list);
    tail = con_last(list);
    con_free(list, NULL);
  return 0;
}
*/
