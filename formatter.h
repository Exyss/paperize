#ifndef FORMATTER_H
#define FORMATTER_H

// STANDARD INCLUDES
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// LOCAL INCLUDES
#include "linked_list.h"
#include "string_utils.h"
#include "queue.h"
#include "node.h"
#include "structs.h"

// METHODS

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
char* format_page_row(Page* page, int inter_col_space);

#endif