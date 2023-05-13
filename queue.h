#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "node.h"

typedef struct Queue Queue;

Queue* init_queue(int max_size);

bool is_queue_empty(Queue* queue);

int queue_size(Queue* queue);

bool is_queue_full(Queue* queue);

void enqueue(Queue* queue, void* data_ptr, bool* status);

void* dequeue(Queue* queue, bool* status);

//void destroy_queue(Queue* queue); 

#endif