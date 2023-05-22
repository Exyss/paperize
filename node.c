#include "node.h"

Node* init_node(void* data_ptr){
    Node* node = (Node*) malloc(sizeof(Node));
    node->data_ptr = data_ptr;
    node->next = NULL;

    return node;
}