#ifndef FORMATTER_H
#define FORMATTER_H

// STANDARD INCLUDES
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// LOCAL INCLUDES
#include "../core/status_codes.h"
#include "../data-structs/linked_list.h"
#include "../data-structs/queue.h"
#include "../data-structs/node.h"
#include "../utils/string_utils.h"
#include "../core/structs.h"

// METHODS

/*
    Returns a copy of the given row_str concatenated to the next word of the given row.

    The used word gets DEQUEUED and DESTROYED.
    The given row_str string is left unchanged.

    Returns NULL if the given row has no words left or if an error occurred

    In case of error, sets status to WORD_DEQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static char* append_next_row_word(Row* row, char* row_str, int pad_amount, bool apply_right_padding, short* status);

/*
    Formats the given column row into a new fixed size string.
    Justified alignment is used if apply_justification is set to True.

    The given row gets DESTROYED.

    Returns NULL if an error occurred.

    In case of error, sets status to WORD_DEQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static char* format_column_string(Row* row, bool apply_justification, short* status);

/*
    Formats the next page string using the rows of the columns of the page.
    Each column row string is separated by inter_col_space spaces.

    The used rows get DEQUEUED and DESTROYED

    Returns NULL if an error occurred

    In case of success, sets status to FORMATTING_SUCCESS.
    In case of error, sets status to ROW_DEQUEUE_FAIL, WORD_DEQUEUE_FAIL or STRING_ALLOC_FAIL.
*/
char* format_page_string(Page* page, int inter_col_space, short* status);

#endif