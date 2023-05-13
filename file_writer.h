#ifndef FILE_WRITER__H
#define FILE_WRITER__H

#include "queue.h"
#include "file_reader.h"
#include "structs.h"
#include "linked_list.h"
#include "node.h"

void write_column_row(FILE* fout, Row* row, int INTER_COL_SPACE);

bool write_row(FILE* fout, LinkedList* columns, int INTER_COL_SPACE);

int write_pages(const char* out_filename, Queue* pages, int INTER_COL_SPACE);

#endif