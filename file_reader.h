#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "structs.h"
#include "queue.h"
#include "linked_list.h"

extern Word* last_read_word;
extern Row* last_read_row;
extern Column last_read_col;

typedef enum status{
    REACHED_EOF,    //End Of File
    REACHED_EOL,    //End Of Line
    OK
} status_t;

static Word* read_word(FILE* fin, int MAX_CHARS, status_t* status);

static Row* read_row(FILE* fin, int MAX_CHARS, status_t* status);

static Column* read_column(FILE* fin, int MAX_ROWS, int MAX_CHARS, status_t* status);

static Page* read_page(FILE* fin, int MAX_COLS, int MAX_ROWS, int MAX_CHARS, status_t* status);

Queue* read_pages(const char* filename, int COLUMN_AMOUNT, int COLUMN_HEIGHT, int COLUMN_WIDTH);

#endif