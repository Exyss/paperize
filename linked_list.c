#include "linked_list.h"

/*
    Creates a new LinkedList and returns it's pointer
    Returns NULL if heap allocation has failed
*/
LinkedList* init_linked_list(){
    LinkedList* list;
    list = (LinkedList*) malloc(sizeof(LinkedList));

    //if malloc hasn't failed
    if (list != NULL){
        list->head = NULL;
    }

    return list;
}

/*  
    Creates a node containing the given data_ptr
    and appends it to the given list.
*/
void insert_in_list(LinkedList* list, void* data_ptr){

    //create new node
    Node* node = init_node(data_ptr);

    //if head is NULL, this node becomes the head
    if(list->head == NULL){
        list->head = node;
    }
    else{
        //append this node to the end of the list
        Node* next = list->head;
        while(next->next != NULL){
            next = next->next;
        }

        next->next = node;
    }
}