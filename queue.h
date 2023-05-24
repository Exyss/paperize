#ifndef QUEUE_H
#define QUEUE_H

// STANDARD INCLUDES
#include <stdlib.h>
#include <stdbool.h>

// LOCAL INCLUDES
#include "node.h"

// STATUS CODES
#define ENQUEUE_SUCCESS 1
#define DEQUEUE_SUCCESS 2
#define ENQUEUE_FAIL -1
#define DEQUEUE_FAIL -2

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
    Checks if the given queue is empty
*/
bool is_queue_empty(Queue* queue);

/*
    Checks if the given queue is full
*/
bool is_queue_full(Queue* queue);

/*
    Returns the size of the given queue
*/
int queue_size(Queue* queue);

/*
    Returns the data_ptr of the head of the given 
    queue without removing it from the queue
    (returns NULL if the queue is empty)
*/
void* peek_queue(Queue* queue);

/*
    Creates a node containing the given data_ptr
    and appends it to the given queue.
    Sets status to ENQUEUE_SUCCESS or ENQUEUE_FAIL
*/
void enqueue(Queue* queue, void* data_ptr, short* status);

/*
    Returns the data_ptr of the head of the given 
    queue and removes it from the queue
    (returns NULL if the queue is empty).
    Sets status to DEQUEUE_SUCCESS or DEQUEUE_FAIL
*/
void* dequeue(Queue* queue, short* status);

#endif