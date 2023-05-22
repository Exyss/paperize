#include "pager.h"

Word* last_word = NULL;
Row* last_row = NULL;

bool USE_LAST_WORD = false;
bool USE_LAST_ROW = false;
bool REACHED_EOF = false;
bool REACHED_EOL = false;

static Row* read_row(FILE* fin, int MAX_CHARS){
    
    Row* row;
    Word* next_word;

    int char_count;
    bool q_status;

    row = init_row(MAX_CHARS);

    while(!REACHED_EOF && !REACHED_EOL){

        //use previously stored word
        if(USE_LAST_WORD){
            next_word = last_word;
            USE_LAST_WORD = false;
        }
        else{
            next_word = read_word(fin, MAX_CHARS);

            //avoid reading \n followed by \n
            if(last_word != NULL && next_word->char_count == 0
                && next_word->REACHED_EOL && last_word->REACHED_EOL){
                    
                continue;
            }
            
            //after all checks, store new word
            last_word = next_word;
        }

        REACHED_EOF = next_word->REACHED_EOF;
        REACHED_EOL = next_word->REACHED_EOL;

        //row chars + word chars + one space per already enqueued word
        char_count = row->char_count + next_word->char_count + queue_size(row->words);

        //if the word cant be appended
        if(char_count > MAX_CHARS){
            //globally set last read word in order to insert it into next row

            USE_LAST_WORD = true;
            REACHED_EOL = false;
            REACHED_EOF = false;
            break;
        }

        row->char_count += next_word->char_count;
        enqueue(row->words, next_word, &q_status);
    }

    return row;
}

static Column* read_column(FILE* fin, int MAX_ROWS, int MAX_CHARS){
    
    Column* column = init_column(MAX_ROWS);
    Row* next_row;
    bool q_status;
    
    while(!REACHED_EOF){
        
        //use previously stored row
        if(USE_LAST_ROW){
            next_row = last_row;
            USE_LAST_ROW = false;
        }
        else{
            next_row = read_row(fin, MAX_CHARS);
        }
        
        last_row = next_row;

        if(is_queue_full(column->rows)){
            USE_LAST_ROW = true;
            break;
        }

        //needed in case the file starts with \n
        if(next_row->char_count == 0 && is_queue_empty(column->rows)){
            REACHED_EOL = false;
            continue;
        }
        
        enqueue(column->rows, next_row, &q_status);

        //add a newline only if the row isn't empty
        if(REACHED_EOL){
            REACHED_EOL = false;

            //avoid adding empty row if the current row is empty
            //or if the column is full/empty
            if(next_row->char_count != 0 && !is_queue_empty(column->rows) && !is_queue_full(column->rows)){
                next_row = init_row(MAX_CHARS);
                last_row = next_row;
                enqueue(column->rows, next_row, &q_status);
            }
        }
    }
    return column;
}

static Page* read_page(FILE* fin, int MAX_COLS, int MAX_ROWS, int MAX_CHARS){
    
    Page* page = init_page(MAX_COLS);
    Column* col;
    bool q_status;
    int col_count = 0;

    while(!REACHED_EOF && col_count < MAX_COLS){
        col = read_column(fin, MAX_ROWS, MAX_CHARS);
        insert_in_list(page->columns, col);
        col_count++;
    }

    return page;
}

Queue* read_pages(const char* in_filename, int COLUMN_AMOUNT, int COLUMN_HEIGHT, int COLUMN_WIDTH){

    FILE* fin;
    Queue* pages;
    Page* page; 
    bool q_status;

    pages = init_queue(INT_MAX);

    //open output file
    if ((fin = fopen(in_filename, "r")) == NULL) {
        fprintf(stderr, "Cannot read from input.txt");
        // return 1;
    }
    
    //read pages until file EOF is reached
    while(!REACHED_EOF){

        page = read_page(fin, COLUMN_AMOUNT, COLUMN_HEIGHT, COLUMN_WIDTH);
        enqueue(pages, page, &q_status);
    }

    //close output file
    fclose(fin);

    return pages;
}