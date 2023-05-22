#include "queue.h"

Queue* init_queue(int MAX_SIZE){

    Queue* queue = (Queue*) malloc(sizeof(Queue));

    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    queue->MAX_SIZE = MAX_SIZE;

    return queue;
}

bool is_queue_empty(Queue* queue){
    return (queue->size == 0);
}

int queue_size(Queue* queue){
    return queue->size;
}

bool is_queue_full(Queue* queue){
    return (queue->size == queue->MAX_SIZE);
}

//MUST BE HANDLED CAREFULLY
void* peek_queue(Queue* queue){
    if(!is_queue_empty(queue)){
        return queue->head->data_ptr;
    }
    else{
        return NULL;
    }
}

//Sets status to false in case max limit has been reached
void enqueue(Queue* queue, void* data_ptr, bool* status){
    if(is_queue_full(queue)){
        *status = false;
        return;
    }

    //create new node
    Node* node = init_node(data_ptr);

    if(is_queue_empty(queue)){
        //set new node both as head and tail
        queue->head = node;
        queue->tail = node;
    }
    else{
        //make old tail point to the new tail
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
}

//Sets status to false and returns 0 in case the queue is empty
void* dequeue(Queue* queue, bool* status){

    //use a status flag in order to avoid accessing NULL value from caller
    if(is_queue_empty(queue)){
        *status = false;
        return 0;
    }

    *status = true;

    
    //update queue head and free the old head
    Node* old_head = queue->head;
    void* old_head_data_ptr = old_head->data_ptr;

    if(queue->size == 1){
        queue->head = NULL;
        queue->tail = NULL;
    }
    else{
        queue->head = queue->head->next;
    }
    
    free(old_head);
    queue->size--;

    return old_head_data_ptr;
}