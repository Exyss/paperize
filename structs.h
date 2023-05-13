#ifndef STRUCTS_H
#define STRUCTS_H

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

Page* init_page(int MAX_COLS);

Column* init_column(int MAX_ROWS);

Row* init_row(int MAX_CHARS);

Word* init_word(char* str, int char_count);

#endif