#ifndef MULTIPROCESS_H
#define MULTIPROCESS_H

// STANDARD INCLUDES
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>

// LOCAL INCLUDES
#include "../core/status_codes.h"
#include "../core/page_structs.h"
#include "../core/reader.h"
#include "../core/writer.h"
#include "../core/pager.h"
#include "../utils/args.h"

// METHODS


/*
    Runs program in parallel using 3 child processes

    If all children successfully complete execution, returns PROCESS_SUCCESS
    If one or more children fail, the exit code of the first failing child is returned  
*/
int run_multi_process(Args* args);

/*
    Runs the reader process.

    In case of success, returns PROCESS_SUCCESS
    In case of error, returns the main process exit code
*/
static int run_reader_process(Args* args, int pipe_rp[]);
/*
    Runs the pager process.

    In case of success, returns PROCESS_SUCCESS
    In case of error, returns the main process exit code
*/
static int run_pager_process(Args* args, int pipe_rp[], int pipe_pw[]);

/*
    Runs the writer process.

    In case of success, returns PROCESS_SUCCESS
    In case of error, returns the main process exit code
*/
static int run_writer_process(Args* args, int pipe_pw[]);

/*
    Kills the process with the given pid

    Returns False if the process was not found or
    if an error occurred while sending SIGTERM signal
*/
bool kill_process(pid_t pid);

/*
    Waits for the 3 parallel child processes to terminate or
    exit due to an error. 

    If a child exits due to an error, a SIGTERM signal is send
    to all other childrens in order to stop their execution.
*/
short wait_processes(pid_t reader_pid, pid_t pager_pid, pid_t writer_pid);

#endif