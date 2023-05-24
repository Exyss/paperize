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

// STATUS CODES
#define WRITE_SUCCESS 0
#define WRITE_FAIL 1

// METHODS

/*
    Writes the given string on the given file.
    Sets status to WRITE_SUCCESS or WRITE_FAIL
*/
void write_to_file(FILE* fout, char* str, short* status);

/*
    Write the given row on the given file
    without justified alignment
*/
void write_justified_col_row(FILE* fout, Row* row);

/*
    Write the given row on the given file
    with justified alignment
*/
void write_justified_col_row(FILE* fout, Row* row);

/*
    Writes the given column row on the given file
    using the given INTER_COL_SPACE (space between each column)
*/
void write_column_row(FILE* fout, Row* row, int INTER_COL_SPACE, bool DO_NOT_JUSTIFY);

/*
    Writes a row from each column of the given column list
    using the given INTER_COL_SPACE (space between each column)
*/
bool write_page_row(FILE* fout, LinkedList* columns, int INTER_COL_SPACE, bool IS_LAST_PAGE);

/*
    Writes all the given pages on the given file
    using the given INTER_COL_SPACE (space between each column)
*/
int write_pages(char* out_filename, Queue* pages, int INTER_COL_SPACE);

#endif