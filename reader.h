#ifndef READER_H
#define READER_H

// STANDARD INCLUDES
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

// LOCAL INCLUDES
#include "structs.h"

// STATUS CODES
#define READ_SUCCESS 0
#define READ_FAIL 1

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
*/
static Word* pack_word(char* buffer, int byte_count, int char_count, bool REACHED_EOL, bool REACHED_EOF);

/*
    Reads the file byte-by-byte until a full
    Word has been created (until a '\n',
    a ' ' or a '\t' gets read or until
    MAX_CHARS have been read).

    Sets status to READ_SUCCESS or READ_FAIL.
*/
Word* read_word(FILE* fin, int MAX_CHARS, short* status);

#endif