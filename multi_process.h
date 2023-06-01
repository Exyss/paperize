#ifndef MULTIPROCESS_H
#define MULTIPROCESS_H

// LOCAL INCLUDES
#include "args.h"
#include "structs.h"
#include "stdio.h"
#include "reader.h"
#include "writer.h"
#include "pager.h"
#include <sys/wait.h>

// STATUS CODES
#define PROCESS_SUCCESS 0
#define PROCESS_FORK_FAIL 1
#define FILE_OPEN_FAIL 2
#define PIPE_OPEN_FAIL 3

// METHODS 
int run_multi_process(Args* args);

#endif