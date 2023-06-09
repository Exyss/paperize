#ifndef WRITER_H
#define WRITER_H

// STANDARD INCLUDES
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// LOCAL INCLUDES
#include "../data_structs/linked_list.h"
#include "../data_structs/queue.h"
#include "../data_structs/node.h"
#include "../core/page_structs.h"
#include "../core/formatter.h"
#include "../core/status_codes.h"
#include "../utils/string_utils.h"

// METHODS

/*
    Writes the given string on the given file.
    Sets status to FILE_WRITE_SUCCESS or FILE_WRITE_FAIL
*/
void write_string_to_file(FILE* fout, char* str, short* status);

/*
    Writes the given string on the given pipe file descriptor.
    Sets status to PIPE_WRITE_SUCCESS or PIPE_WRITE_FAIL
*/
void write_string_to_pipe(int fd_out, char* str, bool close_pipe, short* status);

/*
    Writes the given word on the given pipe file descriptor.

    Sets status to PIPE_WRITE_SUCCESS or PIPE_WRITE_FAIL
*/
void write_word_to_pipe(int fd_out, Word* word, short* status);

/*
    Writes the given page to the given file using the given
    inter_col_space as the space between each column of the page

    The given page gets DESTROYED

    Sets status to FILE_WRITE_SUCCESS, FILE_WRITE_FAIL or FORMATTING_ERROR
*/
void write_page_to_file(FILE* fout, Page* page, int inter_col_space, short* status);

/*
    Writes the given page to the given file using the given
    inter_col_space as the space between each column of the page

    The given page gets DESTROYED

    Sets status to FILE_WRITE_SUCCESS, FILE_WRITE_FAIL or FORMATTING_ERROR
*/
void write_page_to_pipe(int fd_out, Page* page, int inter_col_space, short* status);

#endif