#include <limits.h>
#include "queue.h"
#include "linked_list.h"

typedef struct Page{
    LinkedList* columns;
} Page;

typedef struct Column{
    Queue* rows;
} Column;

typedef struct Row{
    Queue* words;
    int char_count;
    int MAX_CHARS;
} Row;

typedef struct Word{
    char* str;
    int char_count;
} Word;

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

Word* init_word(char* str, int char_count){
    Word* word = (Word*) malloc(sizeof(Word));
    word->str = str;
    word->char_count = char_count;
    return word;
}