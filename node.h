#ifndef NODE_H
#define NODE_H

#include <stdlib.h>

typedef struct Node {
    //void* accepts every kind of pointer (needed for polymorphic queue)
    //  => ATTENTION: DEREFERENCES MUST BE HANDLED CAREFULLY
    void* data_ptr;
    struct Node* next;
} Node;

Node* init_node(void* data_ptr);

#endif