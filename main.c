#include "core/status_codes.h"
#include "spark/single_process.h"
#include "spark/multi_process.h"
#include "utils/args.h"

// EXIT CODES

#define SUCCESS         0
#define ARGS_ERROR      1
#define FORK_ERROR      2
#define FILE_ERROR      3
#define PIPE_ERROR      4
#define PAGING_ERROR    5
#define ALLOC_ERROR     6

int manage_exit(short status);

int main(int argc, char* argv[]) {

    Args* args;
    short status;

    args = (Args*) parse_args(argc, argv);

    if(args == NULL){
        status = ARGS_ERROR;
    }
    else if(args->PRINT_HELP){
        printf("HELP MESSAGE\n");
        status = PROCESS_SUCCESS;
    }
    else if(args->USE_PARALLEL_MODE){
        status = run_multi_process(args);
    }
    else{
        status = run_single_process(args);
    }

    return manage_exit(status);
}

/*
    Prints exit message and returns exit code
*/
int manage_exit(short status){

    char* msg;
    int exit_code;

    switch(status){
        case PROCESS_SUCCESS:
            msg = "Paperization completed! Output file was written.";
            exit_code = SUCCESS;
            break;

        case WRONG_USAGE:
            msg = "Wrong usage! Run program with -h for more informations";
            exit_code = ARGS_ERROR;
            break;

        case PROCESS_FORK_FAIL:
            msg = "An error occurred while forking child processes";
            break;

        case FILE_OPEN_FAIL:
        case FILE_CLOSE_FAIL:
            msg = "An error occurred while opening or closing a file.";
            exit_code = FILE_ERROR;
            break;

        case FILE_READ_FAIL:
        case FILE_WRITE_FAIL:
            msg = "An error occurred while reading from or writing to a file.";
            exit_code = FILE_ERROR;
            break;

        case PIPE_OPEN_FAIL:
        case PIPE_CLOSE_FAIL:
            msg = "An error occurred while opening or closing a pipe.";
            exit_code = PIPE_ERROR;
            break;

        case PIPE_READ_FAIL:
        case PIPE_READ_TERMINATED:
        case PIPE_WRITE_FAIL:
            msg = "An error occurred while reading from or writing to a pipe.";
            exit_code = PIPE_ERROR;
            break;

        case PAGE_INIT_FAIL:
        case PAGE_ENQUEUE_FAIL:
        case PAGE_DEQUEUE_FAIL:
            msg = "An error occurred while managing a text page";
            exit_code = PAGING_ERROR;
            break;

        case COLUMN_INIT_FAIL:
        case COLUMN_APPEND_FAIL:
            msg = "An error occurred while managing a text column";
            exit_code = PAGING_ERROR;
            break;

        case ROW_INIT_FAIL:
        case ROW_ENQUEUE_FAIL:
        case ROW_DEQUEUE_FAIL:
            msg = "An error occurred while managing a text row";
            exit_code = PAGING_ERROR;
            break;

        case WORD_INIT_FAIL:
        case WORD_ENQUEUE_FAIL:
        case WORD_DEQUEUE_FAIL:
            msg = "An error occurred while managing a word";
            exit_code = PAGING_ERROR;
            break;

        case STRING_ALLOC_FAIL:
        case QUEUE_ALLOC_FAIL:
            msg = "An error occurred while allocating a string or a queue";
            exit_code = ALLOC_ERROR;
            break;

        case FORMATTING_SUCCESS:
            msg = "An error occurred while formatting a string";
            exit_code = PAGING_ERROR;
            break;
    }

    fprintf(stderr, "%s\n", msg);

    if(status != PROCESS_SUCCESS){
        fprintf(stderr, "Exit code: %d\n", exit_code);
    }

    return exit_code;
}