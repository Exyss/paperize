#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "string_utils.h"
#include "queue.h"
#include "node.h"
#include "structs.h"

void write_justified_col_row(FILE* fout, Row* row);

void write_justified_col_row(FILE* fout, Row* row);

void write_column_row(FILE* fout, Row* row, int INTER_COL_SPACE, bool DO_NOT_JUSTIFY);

bool write_page_row(FILE* fout, LinkedList* columns, int INTER_COL_SPACE);

int write_pages(const char* out_filename, Queue* pages, int INTER_COL_SPACE);

#endif