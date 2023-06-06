#include "structs.h"

/*
    Creates a new Page and returns it's pointer

    Returns NULL if heap allocation has failed
*/
Page* init_page(){
    Page* page;
    
    page = (Page*) malloc(sizeof(Page));

    //if malloc hasn't failed
    if(page != NULL){
        page->columns = init_linked_list();

        // if initialization has failed
        if(page->columns == NULL){
            free(page);
            return NULL;
        }

        page->is_last_page = false;
    }
    return page;
}

/*
    Creates a new Column and returns it's pointer

    Returns NULL if heap allocation has failed
*/
Column* init_column(int MAX_ROWS){
    Column* col;

    col = (Column*) malloc(sizeof(Column));

    //if malloc hasn't failed
    if(col != NULL){
        col->rows = init_queue(MAX_ROWS);

        // if initialization has failed
        if(col->rows == NULL){
            free(col);
            return NULL;
        }
    }
    return col;
}

/*
    Creates a new Row and returns it's pointer

    Returns NULL if heap allocation has failed
*/
Row* init_row(int max_chars){
    Row* row;

    row = (Row*) malloc(sizeof(Row));

    //if malloc hasn't failed
    if(row != NULL){
        row->words = init_queue(INT_MAX);

        // if initialization has failed
        if(row->words == NULL){
            free(row);
            return NULL;
        }

        row->char_count = 0;
        row->max_chars = max_chars;
    }
    return row;
}

/*
    Creates a new Word and returns it's pointer

    Returns NULL if heap allocation has failed
*/
Word* init_word(char* str, int char_count, bool REACHED_EOL, bool REACHED_EOF){

    Word* word;

    word = (Word*) malloc(sizeof(Word));

    //if malloc hasn't failed
    if(word != NULL){
        word->str = str;
        word->char_count = char_count;
        word->REACHED_EOL = REACHED_EOL;
        word->REACHED_EOF = REACHED_EOF;
    }
    return word;
}

/*
    Completely de-allocates the given word and it's inner components
*/
void destroy_word(Word* word){
    free(word->str);
    free(word);
}

/*
    Completely de-allocates the given row and it's inner components

    Returns True if no error occurred, False de-allocation partially
    or completely failed (inner objects may still be allocated)
*/
bool destroy_row(Row* row){

    Word* word;
    short q_status;

    while(!is_queue_empty(row->words)){
        word = (Word*) dequeue(row->words, &q_status);

        if(q_status != DEQUEUE_SUCCESS){
            return false;
        }
        destroy_word(word);
    }

    free(row->words);
    free(row);
    return true;
}

/*
    Completely de-allocates the given column and it's inner components

    Returns True if no error occurred, False de-allocation partially
    or completely failed (inner objects may still be allocated)
*/
bool destroy_column(Column* column){

    Row* row;
    short q_status;

    while(!is_queue_empty(column->rows)){
        row = (Row*) dequeue(column->rows, &q_status);

        if(q_status != DEQUEUE_SUCCESS || !destroy_row(row)){
            return false;
        }
    }
    
    free(column->rows);
    free(column);
    return true;
}

/*
    Completely de-allocates the given page and it's inner components

    Returns True if no error occurred, False de-allocation partially
    or completely failed (inner objects may still be allocated)
*/
bool destroy_page(Page* page){
    LinkedList* columns;
    Column* column;
    Node* node;
    Node* tmp;

    columns = page->columns;
    node = columns->head;

    while(node != NULL){
        column = (Column*) node->data_ptr;

        if(!destroy_column(column)){
            return false;
        }

        tmp = node->next;
        free(node);
        node = tmp; 
    }
    
    free(page->columns);
    free(page);
    return true;
}

/*
    Returns true if all the columns of the given page have no rows left
*/
bool are_all_cols_empty(Page* page){

    Column* col;
    Node* node;
    
    node = page->columns->head;

    while(node != NULL){
        col = node->data_ptr;
        if(!is_queue_empty(col->rows)){
            return false;
        }

        node = node->next;
    }

    return true;
}