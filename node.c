#include <stdlib.h>

typedef struct Node {
    //void* accepts every kind of pointer (needed for polymorphic queue)
    //  => ATTENTION: DEREFERENCES MUST BE HANDLED CAREFULLY
    void* data_ptr;
    struct Node* next;
} Node;

Node* init_node(void* data_ptr){
    Node* node = (Node*) malloc(sizeof(Node));
    node->data_ptr = data_ptr;
    node->next = NULL;

    return node;
}