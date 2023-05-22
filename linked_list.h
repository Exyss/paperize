#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include "node.h"

typedef struct LinkedList {
    Node* head;
} LinkedList;

LinkedList* init_linked_list();

void insert_in_list(LinkedList* list, void* data_ptr);

#endif