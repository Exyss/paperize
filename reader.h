#ifndef READER_H
#define READER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "structs.h"
#include "queue.h"
#include "linked_list.h"

typedef unsigned short int u16;

static u16 get_bytes_to_read(int next_byte);

static Word* format_word(char* buffer, int byte_count, int char_count, bool REACHED_EOL, bool REACHED_EOF);

Word* read_word(FILE* fin, int MAX_CHARS);

#endif