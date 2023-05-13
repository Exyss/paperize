#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "structs.h"
#include "queue.h"
#include "linked_list.h"

Word* last_read_word = NULL;
Row* last_read_row = NULL;
bool is_last_word_EOL = false;

typedef enum status{
    REACHED_EOF,    //End Of File
    REACHED_EOL,    //End Of Line
    OK
} status_t;

static Word* read_word(FILE* fin, int MAX_CHARS, status_t* status){
    
    //worst case: all row chars are UTF-8 (max 4 bytes per UTF-8 char), +1 for \0
    //   => using a "big" buffer is preferable to continuously using malloc()
    //      in order to append characters since it would impact on performance
    //      due to too much malloc() and free() calls (also, the stack gets
    //      cleared once a word has been read, meaning the same bytes would be reused)
    const int MAX_BUFFER = MAX_CHARS * 4 + 1;

    char buffer[MAX_BUFFER];

    //DO NOT use char type for next_char (conflict with UTF-8 chars)
    short next_char;
    int byte_count = 0;
    int char_count = 0;

    *status = OK;

    //read one byte at a time (slower but simpler to manage)
    while((next_char = fgetc(fin)) != EOF){

        //stop reading on newline
        if(next_char == '\n'){
            *status = REACHED_EOL;
            break;
        }
        //stop reading on spaces
        else if(next_char == '\t' || next_char == ' '){
            break;
        }
        //ignore carriage return
        else if(next_char == '\r'){
            continue;
        }
        else{
            //append char
            buffer[byte_count] = next_char;
            byte_count++;
        }

        //count UTF-8 chars only when they end
        //  => the first octet of a UTF-8 char starts with 0b11xxxxxx
        //     while all trailing UTF-8 octets start with 0b10xxxxxx)
        if((next_char >> 6) != 0b10){
            char_count++;

            //this check must be made here in order to avoid
            //infinite loops with UTF-8 chars
            if(char_count == MAX_CHARS){
                break;
            }
        }
    }

    //check if EOF has been reached
    if(next_char == EOF){
        *status = REACHED_EOF;
    }

    //signal word end for strcpy (see below)
    buffer[byte_count] = '\0';
    byte_count++;

    //strcpy stops copying after reading the first \0
    //    => we get a perfect sized string in the heap
    //       while the bloated string in stack gets reused
    char* tmp = (char*) malloc(byte_count);
    strcpy(tmp, buffer);

    Word* word = init_word(tmp, char_count);
    return word;
}

static Row* read_row(FILE* fin, int MAX_CHARS, status_t* status){
    
    Row* row;
    Word* word;

    int char_count;
    bool q_status;

    row = init_row(MAX_CHARS);

    //read until EOF or EOL is reached
    while(*status == OK){

        //use previously stored word in case it exists
        if(last_read_word != NULL){
            word = last_read_word;
            last_read_word = NULL;
            *status = (is_last_word_EOL) ? REACHED_EOL : OK;
        }
        else{
            word = read_word(fin, MAX_CHARS, status);
            
            //ignore empty words (e.g. when two spaces are read consecutively)
            if(word->char_count == 0){
                *status = (*status == REACHED_EOL) ? OK : *status;
                continue;
            }
        }

        //row chars + word chars + one space per already enqueued word
        char_count = row->char_count + word->char_count + queue_size(row->words);

        //if row is full/has ended
        if(char_count > MAX_CHARS){
            //globally set last read word in order to
            //insert it into next row
            last_read_word = word;
            is_last_word_EOL = (*status == REACHED_EOL);
            *status = OK;
            break;
        }

        row->char_count += word->char_count;
        enqueue(row->words, word, &q_status);
    }

    return row;
}

static Column* read_column(FILE* fin, int MAX_ROWS, int MAX_CHARS, status_t* status){
    
    Column* column = init_column(MAX_ROWS);
    Row* row;
    bool q_status;
    
    while(*status != REACHED_EOF){

        //use previously stored row in case it exists
        if(last_read_row != NULL){
            row = last_read_row;
            last_read_row = NULL;
        }
        else{
            row = read_row(fin, MAX_CHARS, status);
        }

        if(is_queue_full(column->rows)){
            //globally set last read row in order to
            //insert it into next column
            last_read_row = row;
            break;
        }

        enqueue(column->rows, row, &q_status);

        //add an empty row to separate paragraphs
        if(*status == REACHED_EOL){
            *status = OK;
            
            //avoid adding empty row if the column is full or empty
            if(!is_queue_empty(column->rows) && !is_queue_full(column->rows)){
                row = init_row(MAX_CHARS);
                enqueue(column->rows, row, &q_status);
            }
        }
    }
    return column;
}

static Page* read_page(FILE* fin, int MAX_COLS, int MAX_ROWS, int MAX_CHARS, status_t* status){
    
    Page* page = init_page(MAX_COLS);
    Column* col;
    bool q_status;
    int col_count = 0;

    while(*status != REACHED_EOF && col_count < MAX_COLS){
        col = read_column(fin, MAX_ROWS, MAX_CHARS, status);
        insert_in_list(page->columns, col);
        col_count++;
    }

    return page;
}


//TODO: this should become a stand-alone process

Queue* read_pages(const char* in_filename, int COLUMN_AMOUNT, int COLUMN_HEIGHT, int COLUMN_WIDTH){

    FILE* fin;
    Queue* pages;
    Page* page; 
    status_t read_status;
    bool q_status;

    pages = init_queue(INT_MAX);
    read_status = OK;

    //open output file
    if ((fin = fopen(in_filename, "r")) == NULL) {
        fprintf(stderr, "Cannot read from input.txt");
        // return 1;
    }
    
    //read pages until file EOF is reached
    while(read_status != REACHED_EOF){
        page = read_page(fin, COLUMN_AMOUNT, COLUMN_HEIGHT, COLUMN_WIDTH, &read_status);
        enqueue(pages, page, &q_status);
    }

    //close output file
    fclose(fin);

    return pages;
}