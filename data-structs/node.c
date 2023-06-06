#include "node.h"

/*
    Creates a new Node and returns it's pointer
    
    Returns NULL if heap allocation has failed
*/
Node* init_node(void* data_ptr){
    Node* node;

    node = (Node*) malloc(sizeof(Node));

    //if malloc hasn't failed
    if(node != NULL){
        node->data_ptr = data_ptr;
        node->next = NULL;
    }

    return node;
}