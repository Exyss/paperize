#ifndef PAGER_H
#define PAGER_H

// STANDARD INCLUDES
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

// LOCAL INCLUDES
#include "../data_structs/linked_list.h"
#include "../data_structs/queue.h"
#include "../core/status_codes.h"
#include "../core/page_structs.h"
#include "../core/reader.h"

// METHOD

/*
    Retrieves the next row from a file or a pipe

    Returns NULL if an error occurred

    In case of error, sets status to FILE_READ_FAIL, PIPE_READ_FAIL,
    ROW_INIT_FAIL, WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static Row* get_row(FILE* fin, int fd_in, bool get_from_pipe, int max_chars, short* status);

/*
    Retrieves the next column from a file or a pipe

    In case of error, sets status to FILE_READ_FAIL, PIPE_READ_FAIL, COLUMN_INIT_FAIL,
    ROW_INIT_FAIL, ROW_ENQUEUE_FAIL, WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static Column* get_column(FILE* fin, int fd_in, bool get_from_pipe, int MAX_ROWS, int max_chars, short* status);

/*
    Retrieves the next page from a file or a pipe

    Returns NULL if an error occurred

    Sets status to INIT_PAGE_FAIL, INIT_COLUMN_FAIL, INIT_ROW_FAIL,
    ROW_ENQUEUE_FAIL, WORD_READ_FAIL or WORD_ENQUEUE_FAIL 

    In case of error, sets status to FILE_READ_FAIL, PIPE_READ_FAIL, PAGE_INIT_FAIL,
    COLUMN_INIT_FAIL, COLUMN_APPEND_FAIL, ROW_INIT_FAIL, ROW_ENQUEUE_FAIL,
    WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static Page* get_page(FILE* fin, int fd_in, bool get_from_pipe, int MAX_COLS, int MAX_ROWS, int max_chars, short* status);

/*
    Retrieves the next page from a file

    Returns NULL if an error occurred

    In case of success, sets status to FILE_READ_SUCCESS
    In case of error, sets status to FILE_READ_FAIL, PAGE_INIT_FAIL,
    COLUMN_INIT_FAIL, COLUMN_APPEND_FAIL, ROW_INIT_FAIL, ROW_ENQUEUE_FAIL,
    WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
Page* get_page_from_file(FILE* fin, int MAX_COLS, int MAX_ROWS, int max_chars, short* status);

/*
    Retrieves the next page from a pipe

    Returns NULL if an error occurred

    In case of success, sets status to PIPE_READ_SUCCESS
    In case of error, sets status to PIPE_READ_FAIL, PAGE_INIT_FAIL,
    COLUMN_INIT_FAIL, COLUMN_APPEND_FAIL, ROW_INIT_FAIL, ROW_ENQUEUE_FAIL,
    WORD_INIT_FAIL, WORD_ENQUEUE_FAIL or STRING_ALLOC_FAIL
*/
Page* get_page_from_pipe(int fd_in, int MAX_COLS, int MAX_ROWS, int max_chars, short* status);

#endif