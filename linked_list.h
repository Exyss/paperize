#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include "node.h"

typedef struct LinkedList {
    Node* head;
    Node* tail;     //used just to achieve O(1) tail-insertion
    int size;
    int MAX_SIZE;
} LinkedList;

LinkedList* init_linked_list();

bool is_list_empty(LinkedList* list);

int list_size(LinkedList* queue);

bool is_list_full(LinkedList* list);

void insert_in_list(LinkedList* list, void* data_ptr);

#endif