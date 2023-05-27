#ifndef WRITER_H
#define WRITER_H

// STANDARD INCLUDES
#include <stdio.h>
#include <string.h>

// LOCAL INCLUDES
#include "linked_list.h"
#include "string_utils.h"
#include "queue.h"
#include "node.h"
#include "structs.h"

// STATUS FLAGS
#define FILE_WRITE_SUCCESS 1
#define PIPE_WRITE_SUCCESS 2 
#define FILE_WRITE_FAIL -1
#define PIPE_WRITE_FAIL -2

// METHODS

/*
    Writes the given string on the given file.

    Sets status to WRITE_SUCCESS or WRITE_FAIL
*/
void write_to_file(FILE* fout, char* str, short* status);

/*
    Writes the given string on the given pipe file descriptor.

    Sets status to PIPE_WRITE_SUCCESS or PIPE_WRITE_FAIL
*/
void write_to_pipe(int fd_out, char* str, short* status);


/*
    Returns a copy of the given row_str concatenated to the next word of the given row.

    The used word gets DEQUEUED and DESTROYED.
    The given row_str string is left unchanged.

    Returns NULL if the given row has no words left
    or if a dequeueing error happened
*/
static char* append_next_row_word(Row* row, char* row_str, int pad_amount, bool use_right_pad);
/*
    Formats the given column row into a new fixed size string.
    Justified alignment is used if to_justify is set to True.

    The given row gets DESTROYED.

    Returns NULL if an error occurred.
*/
static char* format_column_row(Row* row, bool to_justify);

/*
    Formats a page row made up of a formatted row from each column
    of the given column list using the given inter_col_space 
    as the space between each column

    The used rows get DEQUEUED and DESTROYED

    Returns NULL if an error occurred
*/
static char* format_page_row(Page* page, int inter_col_space);

/*
    Returns true if all the columns of the given page have no rows left
*/
static bool are_all_cols_empty(Page* page);

/*
    Writes the given page to the given file using the given
    inter_col_space as the space between each column of the page

    The given page gets DESTROYED

    Sets status to FILE_WRITE_SUCCESS, FILE_WRITE_FAIL or FORMATTING_ERROR
*/
void write_page_to_file(FILE* fout, Page* page, int inter_col_space, short* status);

/*
    Writes all the given pages on the given file
    using the given inter_col_space (space between each column)

    The given pages get DEQUEUED and DESTROYED
*/
int write_pages_to_file(char* out_filename, Queue* pages, int inter_col_space);

#endif