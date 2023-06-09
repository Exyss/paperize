#ifndef READER_H
#define READER_H

// STANDARD INCLUDES
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

// LOCAL INCLUDES
#include "../core/status_codes.h"
#include "page_structs.h"

// METHODS

/*
    Analyzes next_byte in order to calculate
    how many more bytes make up the current
    character.
    
    Returns 0 if it's an ASCII char,
    1, 2 or 3 if it's an UTF-8 char
*/
static short get_bytes_to_read(int next_byte);

/*
    Creates and fills a Word with the given data.
    The given max-size stack buffer gets converted
    to a perfect-sized heap buffer.

    Returns NULL if an error has occurred

    In case of error, sets status to WORD_INIT_FAIL or STRING_ALLOC_FAIL
*/
static Word* pack_word(char* buffer, int byte_count, int char_count, bool REACHED_EOL, bool REACHED_EOF, short* status);

/*
    Reads the file byte-by-byte until a full
    Word has been read or max_chars have been read
    
    Returns NULL if an error occurred

    In case of success, sets status to FILE_READ_SUCCESS.
    In case of error, sets status to FILE_READ_FAIL, WORD_INIT_FAIL or STRING_ALLOC_FAIL
*/
Word* read_word_from_file(FILE* fin, int max_chars, short* status);

/*
    Reads the next word from the given pipe

    Returns NULL if an error occurred

    In case of success, sets status to PIPE_READ_SUCCESS
    In case of error, sets status to PIPE_READ_FAIL, WORD_INIT_FAIL or STRING_ALLOC_FAIL
*/
Word* read_word_from_pipe(int fd_in, short* status);

/*
    Reads the next string from the given pipe

    Returns NULL if an error occurred

    In case of success, sets status to PIPE_READ_SUCCESS or PIPE_READ_TERMINATED
    In case of error, sets status to PIPE_READ_FAIL or STRING_ALLOC_FAIL
*/
char* read_string_from_pipe(int fd_in, short* status);

#endif