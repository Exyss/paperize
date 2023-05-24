#ifndef NODE_H
#define NODE_H

// STANDARD INCLUDES
#include <stdlib.h>

// STRUCTS
typedef struct Node {
    /*
        void* accepts every kind of pointer
        (needed for polymorphic data structures)
        => ATTENTION: DEREFERENCES MUST BE HANDLED CAREFULLY
    */
    void* data_ptr;
    struct Node* next;
} Node;

// METHODS

/*
    Creates a new Node and returns it's pointer
    Returns NULL if heap allocation has failed
*/
Node* init_node(void* data_ptr);

#endif