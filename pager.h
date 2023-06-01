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

static Row* get_row(FILE* fin, int fd_in, bool get_from_pipe, int MAX_CHARS);

static Column* get_column(FILE* fin, int fd_in, bool get_from_pipe, int MAX_ROWS, int MAX_CHARS);

static Page* get_page(FILE* fin, int fd_in, bool get_from_pipe, int MAX_COLS, int MAX_ROWS, int MAX_CHARS);

Page* get_page_from_file(FILE* fin, int MAX_COLS, int MAX_ROWS, int MAX_CHARS);

Page* get_page_from_pipe(int fd_in, int MAX_COLS, int MAX_ROWS, int MAX_CHARS);

#endif