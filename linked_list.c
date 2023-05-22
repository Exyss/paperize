#include "linked_list.h"

LinkedList* init_linked_list(){
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    list->head = NULL;
}

void insert_in_list(LinkedList* list, void* data_ptr){

    //create new node
    Node* node = init_node(data_ptr);

    if(list->head == NULL){
        list->head = node;
    }
    else{
        //reach list end
        Node* next = list->head;
        while(next->next != NULL){
            next = next->next;
        }

        //append node to list
        next->next = node;
    }
}