#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "node.h"

typedef struct Queue {
    Node* head;
    Node* tail;
    int size;
    int MAX_SIZE;
} Queue;

Queue* init_queue(int MAX_SIZE);

bool is_queue_empty(Queue* queue);

int queue_size(Queue* queue);

bool is_queue_full(Queue* queue);

//MUST BE HANDLED CAREFULLY
void* peek_queue(Queue* queue);

void enqueue(Queue* queue, void* data_ptr, bool* status);

void* dequeue(Queue* queue, bool* status);

#endif