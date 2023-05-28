#ifndef ARGS_H
#define ARGS_H

// STANDARD INCLUDES
#include <stdlib.h>
#include <stdbool.h>

// LOCAL INCLUDES
#include "string_utils.h"

// STRUCTS
typedef struct Args{
    char* PROGRAM_NAME;
    char* INPUT_FILE;
    char* OUTPUT_FILE;
    int COLUMN_AMOUNT;
    int COLUMN_HEIGHT;
    int COLUMN_WIDTH;
    int INTER_COL_SPACE;
    bool USE_PARALLEL_MODE;
    bool PRINT_HELP;
} Args;

// STATUS CODES
#define INTEGER_PARSE_SUCCESS 0
#define INTEGER_PARSE_FAIL 1

// METHODS

/*
    Parses the given program arguments

    Returns NULL if parsing failed
*/
Args* parse_args(int argc, char* argv[]);

#endif