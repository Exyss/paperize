#include "structs.h"

Page* init_page(){
    Page* page = (Page*) malloc(sizeof(Page));
    page->columns = init_linked_list();
    return page;
}

Column* init_column(int MAX_ROWS){
    Column* col = (Column*) malloc(sizeof(Column));
    col->rows = init_queue(MAX_ROWS);
    return col;
}

Row* init_row(int MAX_CHARS){
    Row* row = (Row*) malloc(sizeof(Row));
    row->words = init_queue(INT_MAX);
    row->char_count = 0;
    row->MAX_CHARS = MAX_CHARS;
    return row;
}

Word* init_word(char* str, int char_count, bool REACHED_EOL, bool REACHED_EOF){
    Word* word = (Word*) malloc(sizeof(Word));
    word->str = str;
    word->char_count = char_count;
    word->REACHED_EOL = REACHED_EOL;
    word->REACHED_EOF = REACHED_EOF;
    return word;
}

void destroy_word(Word* word){
    free(word->str);
    free(word);
}

void destroy_row(Row* row){
    Word* word;
    bool q_status;

    while(!is_queue_empty(row->words)){
        word = (Word*) dequeue(row->words, &q_status);
        destroy_word(word);
    }

    free(row->words);
    free(row);
}

void destroy_column(Column* column){
    Row* row;
    bool q_status;

    while(!is_queue_empty(column->rows)){
        row = (Row*) dequeue(column->rows, &q_status);
        destroy_row(row);
    }
    
    free(column->rows);
    free(column);
}

void destroy_page(Page* page){
    Column* column;

    LinkedList* columns = (LinkedList*) page->columns;
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
