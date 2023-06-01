#ifndef MULTIPROCESS_H
#define MULTIPROCESS_H

// LOCAL INCLUDES
#include "args.h"
#include "structs.h"
#include "stdio.h"
#include "reader.h"
#include "core/writer.h"
#include "pager.h"
#include <sys/wait.h>

// STATUS CODES
#define PROCESS_SUCCESS 0
#define PROCESS_FORK_FAIL 1
#define FILE_OPEN_FAIL 2
#define PIPE_OPEN_FAIL 3

// METHODS 
static int run_reader_process(Args* args, int pipe_rp[]);

static int run_pager_process(Args* args, int pipe_rp[], int pipe_pw[]);

static int run_writer_process(Args* args, int pipe_pw[]);

int run_multi_process(Args* args);

bool kill_process(pid_t pid);

short wait_processes(pid_t reader_pid, pid_t pager_pid, pid_t writer_pid);

#endif