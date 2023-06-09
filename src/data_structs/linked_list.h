#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// STANDARD INCLUDES
#include <stdbool.h>

// LOCAL INCLUDES
#include "node.h"

// STRUCTS
typedef struct LinkedList {
    Node* head;
} LinkedList;

// METHODS

/*
    Creates a new LinkedList and returns it's pointer

    Returns NULL if heap allocation has failed
*/
LinkedList* init_linked_list();

/*  
    Creates a node containing the given data_ptr
    and appends it to the given list.

    Returns True if insertion was successful,
    False if insertion failed
*/
bool insert_in_list(LinkedList* list, void* data_ptr);

#endif