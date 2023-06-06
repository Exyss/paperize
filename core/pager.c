#include "pager.h"

Word* last_word = NULL;
Row* last_row = NULL;

bool USE_LAST_WORD = false;
bool USE_LAST_ROW = false;
bool REACHED_EOF = false;
bool REACHED_EOL = false;

/*
    Retrieves the next row from a file or a pipe

    Returns NULL if an error occurred

    In case of error, sets status to FILE_READ_FAIL, PIPE_READ_FAIL,
    ROW_INIT_FAIL, WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static Row* get_row(FILE* fin, int fd_in, bool get_from_pipe, int max_chars, short* status){
    
    Row* row;
    Word* next_word;

    int char_count;
    short q_status;
    short read_status;

    row = init_row(max_chars);

    //if initialization has failed
    if(row == NULL){
        *status = ROW_INIT_FAIL;
        return NULL;
    }

    while(!REACHED_EOF && !REACHED_EOL){

        //use previously stored word
        if(USE_LAST_WORD){
            next_word = last_word;
            USE_LAST_WORD = false;
        }
        else{
            if(!get_from_pipe){
                next_word = read_word_from_file(fin, max_chars, &read_status);

                if(read_status != FILE_READ_SUCCESS){
                    //status already gets set by read_word_from_file()
                    return NULL;
                }
            }
            else if(!REACHED_EOF){
                next_word = read_word_from_pipe(fd_in, &read_status);

                if(read_status != PIPE_READ_SUCCESS){
                    //status already gets set by read_word_from_pipe()
                    return NULL;
                }
            }

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
        if(char_count > max_chars){
            //globally set last read word in order to insert it into next row

            USE_LAST_WORD = true;
            REACHED_EOL = false;
            REACHED_EOF = false;
            break;
        }

        row->char_count += next_word->char_count;
        enqueue(row->words, next_word, &q_status);

        if(q_status != ENQUEUE_SUCCESS){
            *status = WORD_ENQUEUE_FAIL;
            destroy_word(next_word);
            return NULL;
        }
    }

    return row;
}

/*
    Retrieves the next column from a file or a pipe

    In case of error, sets status to FILE_READ_FAIL, PIPE_READ_FAIL, COLUMN_INIT_FAIL,
    ROW_INIT_FAIL, ROW_ENQUEUE_FAIL, WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static Column* get_column(FILE* fin, int fd_in, bool get_from_pipe, int MAX_ROWS, int max_chars, short* status){
    
    Column* column;
    Row* next_row;
    short q_status;
    
    column =  init_column(MAX_ROWS);

    //if initialization has failed
    if(column == NULL){
        *status = COLUMN_INIT_FAIL;
        return NULL;
    }

    while(!REACHED_EOF){
        
        //use previously stored row
        if(USE_LAST_ROW){
            next_row = last_row;
            USE_LAST_ROW = false;
        }
        else{
            next_row = get_row(fin, fd_in, get_from_pipe, max_chars, status);
            
            if(next_row == NULL){
                //status already gets set by get_row()
                return NULL;
            }
        }
        
        //store row
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

        if(q_status != ENQUEUE_SUCCESS){
            *status = ROW_ENQUEUE_FAIL;
            destroy_row(next_row);
            return NULL;
        }

        //add a newline only if the row isn't empty
        if(REACHED_EOL){
            REACHED_EOL = false;

            //avoid adding empty row if the current row is empty
            //or if the column is full/empty
            if(next_row->char_count != 0 && !is_queue_empty(column->rows) && !is_queue_full(column->rows)){
                next_row = init_row(max_chars);

                if(next_row == NULL){
                    *status = ROW_INIT_FAIL;
                    return NULL;
                }

                last_row = next_row;
                enqueue(column->rows, next_row, &q_status);

                if(q_status != ENQUEUE_SUCCESS){
                    *status = ROW_ENQUEUE_FAIL;
                    destroy_row(next_row);
                    return NULL;
                }
            }
        }
    }

    return column;
}

/*
    Retrieves the next page from a file or a pipe

    Returns NULL if an error occurred

    Sets status to INIT_PAGE_FAIL, INIT_COLUMN_FAIL, INIT_ROW_FAIL,
    ROW_ENQUEUE_FAIL, WORD_READ_FAIL or WORD_ENQUEUE_FAIL 

    In case of error, sets status to FILE_READ_FAIL, PIPE_READ_FAIL, PAGE_INIT_FAIL,
    COLUMN_INIT_FAIL, COLUMN_APPEND_FAIL, ROW_INIT_FAIL, ROW_ENQUEUE_FAIL,
    WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static Page* get_page(FILE* fin, int fd_in, bool get_from_pipe, int MAX_COLS, int MAX_ROWS, int max_chars, short* status){
    
    Page* page;
    Column* col;

    short q_status;
    int col_count;

    page = init_page(MAX_COLS);

    //if initialization has failed
    if(page == NULL){
        *status = PAGE_INIT_FAIL;
        return NULL;
    }

    col_count = 0;

    while(!REACHED_EOF && col_count < MAX_COLS){

        col = get_column(fin, fd_in, get_from_pipe, MAX_ROWS, max_chars, status);

        if(col == NULL){
            //status already gets set by get_column()            
            return NULL;
        }
        
        //if insert fails
        if(!insert_in_list(page->columns, col)){
            *status = COLUMN_APPEND_FAIL;
            destroy_column(col);
            destroy_page(page);
            return NULL;
        }

        col_count++;
    }

    if(REACHED_EOF){
        page->is_last_page = true;
    }
    
    return page;
}

/*
    Retrieves the next page from a file

    Returns NULL if an error occurred

    In case of success, sets status to FILE_READ_SUCCESS
    In case of error, sets status to FILE_READ_FAIL, PAGE_INIT_FAIL,
    COLUMN_INIT_FAIL, COLUMN_APPEND_FAIL, ROW_INIT_FAIL, ROW_ENQUEUE_FAIL,
    WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
Page* get_page_from_file(FILE* fin, int MAX_COLS, int MAX_ROWS, int max_chars, short* status){
    Page* page;
    page = get_page(fin, -1, false, MAX_COLS, MAX_ROWS, max_chars, status);

    if(page == NULL){
        //status already gets set by get_page()
        return NULL;
    }

    *status = FILE_READ_SUCCESS;
    return page;
}

/*
    Retrieves the next page from a pipe

    Returns NULL if an error occurred

    In case of success, sets status to PIPE_READ_SUCCESS
    In case of error, sets status to PIPE_READ_FAIL, PAGE_INIT_FAIL,
    COLUMN_INIT_FAIL, COLUMN_APPEND_FAIL, ROW_INIT_FAIL, ROW_ENQUEUE_FAIL,
    WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
Page* get_page_from_pipe(int fd_in, int MAX_COLS, int MAX_ROWS, int max_chars, short* status){
    Page* page;
    page = get_page(NULL, fd_in, true, MAX_COLS, MAX_ROWS, max_chars, status);

    if(page == NULL){
        //status already gets set by get_page()
        return NULL;
    }

    *status = PIPE_READ_SUCCESS;
    return page;
}