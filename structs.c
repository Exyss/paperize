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
    }
    return col;
}

/*
    Creates a new Row and returns it's pointer
    Returns NULL if heap allocation has failed
*/
Row* init_row(int MAX_CHARS){
    Row* row;
    row = (Row*) malloc(sizeof(Row));

    //if malloc hasn't failed
    if(row != NULL){
        row->words = init_queue(INT_MAX);
        row->char_count = 0;
        row->MAX_CHARS = MAX_CHARS;
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
    Completely de-allocates the given word
*/
void destroy_word(Word* word){
    free(word->str);
    free(word);
}

/*
    Completely de-allocates the given row
*/
void destroy_row(Row* row){
    Word* word;
    short q_status;

    while(!is_queue_empty(row->words)){
        word = (Word*) dequeue(row->words, &q_status);
        destroy_word(word);
    }

    free(row->words);
    free(row);
}

/*
    Completely de-allocates the given column
*/
void destroy_column(Column* column){
    Row* row;
    short q_status;

    while(!is_queue_empty(column->rows)){
        row = (Row*) dequeue(column->rows, &q_status);
        destroy_row(row);
    }
    
    free(column->rows);
    free(column);
}

/*
    Completely de-allocates the given page
*/
void destroy_page(Page* page){
    Column* column;

    LinkedList* columns = page->columns;
    Node* node = (Node*) columns->head;
    Node* tmp;

    while(node != NULL){
        column = (Column*) node->data_ptr;
        destroy_column(column);

        tmp = node->next;
        free(node);
        node = tmp; 
    }
    
    free(page->columns);
    free(page);
}
