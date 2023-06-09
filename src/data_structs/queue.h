#ifndef QUEUE_H
#define QUEUE_H

// STANDARD INCLUDES
#include <stdlib.h>
#include <stdbool.h>

// LOCAL INCLUDES
#include "node.h"

// STATUS CODES
#define ENQUEUE_SUCCESS 0
#define DEQUEUE_SUCCESS 1
#define QUEUE_IS_EMPTY 2
#define QUEUE_IS_FULL 3
#define ENQUEUE_ALLOCATION_ERROR 4

// STRUCTS

typedef struct Queue {
    Node* head;
    Node* tail;
    int size;
    int MAX_SIZE;
} Queue;

// METHODS

/*
    Creates a new Queue and returns it's pointer
    Returns NULL if heap allocation has failed
*/
Queue* init_queue(int MAX_SIZE);

/*
    Creates a new Queue and returns it's pointer.

    Returns NULL if heap allocation has failed
*/
Queue* init_queue(int MAX_SIZE);

/*
    Checks if the given queue is empty
*/
int queue_size(Queue* queue);

/*
    Checks if the given queue is full
*/
bool is_queue_empty(Queue* queue);

/*
    Returns the size of the given queue
*/
bool is_queue_full(Queue* queue);

/*
    Returns the data_ptr of the head of the given 
    queue without removing it from the queue or
    NULL if the queue is empty
*/
void* peek_queue(Queue* queue);
/*
    Creates a node containing the given data_ptr
    and appends it to the given queue.

    Sets status to ENQUEUE_SUCCESS, QUEUE_IS_FULL
    or ENQUEUE_ALLOCATION_ERROR
*/
void enqueue(Queue* queue, void* data_ptr, short* status);

/*
    Returns the data_ptr of the head of the given 
    queue and removes it from the queue or NULL
    if the queue is empty.

    Sets status to DEQUEUE_SUCCESS or QUEUE_IS_EMPTY
*/
void* dequeue(Queue* queue, short* status);

#endif