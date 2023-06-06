#ifndef STRUCTS_H
#define STRUCTS_H

// STANDARD INCLUDES
#include <limits.h>

// LOCAL INCLUDES
#include "../data-structs/queue.h"
#include "../data-structs/linked_list.h"

// STRUCTS
typedef struct Page{
    LinkedList* columns;
    bool is_last_page;
} Page;

typedef struct Column{
    Queue* rows;
} Column;

typedef struct Row{
    Queue* words;
    int char_count;
    int max_chars;
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
Row* init_row(int max_chars);
/*
    Creates a new Word and returns it's pointer

    Returns NULL if heap allocation has failed
*/
Word* init_word(char* str, int char_count, bool REACHED_EOL, bool REACHED_EOF);

/*
    Completely de-allocates the given word and it's inner components
*/
void destroy_word(Word* word);
/*
    Completely de-allocates the given row and it's inner components

    Returns True if no error occurred, False de-allocation partially
    or completely failed (inner objects may still be allocated)
*/
bool destroy_row(Row* row);
/*
    Completely de-allocates the given column and it's inner components

    Returns True if no error occurred, False de-allocation partially
    or completely failed (inner objects may still be allocated)
*/
bool destroy_column(Column* column);

/*
    Completely de-allocates the given page and it's inner components

    Returns True if no error occurred, False de-allocation partially
    or completely failed (inner objects may still be allocated)
*/
bool destroy_page(Page* page);
/*
    Returns true if all the columns of the given page have no rows left
*/
bool are_all_cols_empty(Page* page);

#endif