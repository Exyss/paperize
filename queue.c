#include "queue.h"

/*
    Creates a new Queue and returns it's pointer.
    Returns NULL if heap allocation has failed
*/
Queue* init_queue(int MAX_SIZE){

    Queue* queue;
    queue = (Queue*) malloc(sizeof(Queue));

    //if malloc hasn't failed
    if(queue != NULL){
        queue->head = NULL;
        queue->tail = NULL;
        queue->size = 0;
        queue->MAX_SIZE = MAX_SIZE;
    }

    return queue;
}

/*
    Checks if the given queue is empty
*/
int queue_size(Queue* queue){
    return queue->size;
}

/*
    Checks if the given queue is full
*/
bool is_queue_empty(Queue* queue){
    return (queue->size == 0);
}

/*
    Returns the size of the given queue
*/
bool is_queue_full(Queue* queue){
    return (queue->size == queue->MAX_SIZE);
}

/*
    Returns the data_ptr of the head of the given 
    queue without removing it from the queue
    (returns NULL if the queue is empty)
*/
void* peek_queue(Queue* queue){
    if(!is_queue_empty(queue)){
        return queue->head->data_ptr;
    }
    else{
        return NULL;
    }
}

/*
    Creates a node containing the given data_ptr
    and appends it to the given queue.
    Sets status to ENQUEUE_SUCCESS or ENQUEUE_FAIL
*/
void enqueue(Queue* queue, void* data_ptr, short* status){
    if(is_queue_full(queue)){
        *status = ENQUEUE_FAIL;
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
    *status = ENQUEUE_SUCCESS;
}

/*
    Returns the data_ptr of the head of the given 
    queue and removes it from the queue
    (returns NULL if the queue is empty).
    Sets status to DEQUEUE_SUCCESS or DEQUEUE_FAIL
*/
void* dequeue(Queue* queue, short* status){

    //use a status flag in order to avoid accessing NULL value from caller
    if(is_queue_empty(queue)){
        *status = DEQUEUE_FAIL;
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

    *status = DEQUEUE_SUCCESS;
    return old_head_data_ptr;
}