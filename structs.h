#ifndef STRUCTS_H
#define STRUCTS_H

// STANDARD INCLUDES
#include <limits.h>

// LOCAL INCLUDES
#include "queue.h"
#include "linked_list.h"

// STRUCTS
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

    //gets set to true if it's the last word of a paragraph
    bool REACHED_EOL;

    //gets set to true if it's the last word of the file
    bool REACHED_EOF;
} Word;

// METHODS

/*
    Creates a new Page and returns it's pointer
    Returns NULL if heap allocation has failed
*/
Page* init_page();

/*
    Creates a new Column and returns it's pointer
    Returns NULL if heap allocation has failed
*/
Column* init_column(int MAX_ROWS);

/*
    Creates a new Row and returns it's pointer
    Returns NULL if heap allocation has failed
*/
Row* init_row(int MAX_CHARS);

/*
    Creates a new Word and returns it's pointer
    Returns NULL if heap allocation has failed
*/
Word* init_word(char* str, int char_count, bool REACHED_EOL, bool REACHED_EOF);

/*
    Completely de-allocates the given word
*/
void destroy_word(Word* word);

/*
    Completely de-allocates the given row
*/
void destroy_row(Row* row);

/*
    Completely de-allocates the given column
*/
void destroy_column(Column* column);

/*
    Completely de-allocates the given page
*/
void destroy_page(Page* page);

#endif