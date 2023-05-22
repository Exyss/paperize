#ifndef PAGER_H
#define PAGER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "structs.h"
#include "reader.h"
#include "queue.h"
#include "linked_list.h"

static Row* read_row(FILE* fin, int MAX_CHARS);

static Column* read_column(FILE* fin, int MAX_ROWS, int MAX_CHARS);

static Page* read_page(FILE* fin, int MAX_COLS, int MAX_ROWS, int MAX_CHARS);

Queue* read_pages(const char* in_filename, int COLUMN_AMOUNT, int COLUMN_HEIGHT, int COLUMN_WIDTH);
#endif