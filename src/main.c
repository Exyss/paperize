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

void print_help_msg();

int manage_exit(short status);

int main(int argc, char* argv[]) {

    Args* args;
    short status;

    args = (Args*) parse_args(argc, argv);

    if(args == NULL){
        status = ARGS_ERROR;
    }
    else if(args->PRINT_HELP){
        print_help_msg();
        return PROCESS_SUCCESS;
    }
    else if(args->USE_PARALLEL_MODE){
        status = run_multi_process(args);
    }
    else{
        status = run_single_process(args);
    }

    return manage_exit(status);
}

void print_help_msg(){
    char* msg =

        "CORRECT USAGE\n"
        "    ./paperize input_filename.txt [FLAGS]\n"
        "\n"
        "FLAGS\n"
        "    -h  | --help            Prints this message without executing program\n"
        "\n"
        "    -c  | --columns         Specifies the number of columns to be used for each page\n"
        "                            The given value must be a positive integer.\n"
        "                            If not used, the default value is set to 3.\n"
        "\n"
        "    -r  | --rows            Specifies the number of rows to be used for each column\n"
        "                            The given value must be a positive integer.\n"
        "                            If not used, the default value is set to 20.\n"
        "\n"
        "    -w  | --width           Specifies the number of rows to be used for each column\n"
        "                            The given value must be a positive integer.\n"
        "                            If not used, the default value is set to 40.\n"
        "\n"
        "    -s  | --spacing         Specifies the number of rows to be used for each column\n"
        "                            The given value must be a positive integer.\n"
        "                            If not used, the default value is set to 10.\n"
        "\n"
        "    -o  | --output          Specifies the name of the output file to be written.\n"
        "                            If not used, the default file \"output.txt\" will be written.\n"
        "\n"
        "    -p  | --parallel        If used, the program will run as 3 parallel sub-processes,\n"
        "                            speeding up computation.\n"
        "\n"
        "EXAMPLE USAGE\n"
        "    ./paperize test.txt -c 3 -r 20 -w 40 -s 10\n"
        "\n"
        "    Paperizes the given \"test.txt\" file using 3 columns for each page,\n"
        "    20 rows for each column, 40 characters for each column row,\n"
        "    10 characters as spacing between each column (same default\n"
        "    behaviour as if none of the flags where specified)\n";

    printf("%s", msg);
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
            msg = "Wrong usage! Try \'./paperize -h\' or \'./paperize --help\' for more informations";
            exit_code = ARGS_ERROR;
            break;

        case PROCESS_FORK_FAIL:
            msg = "An error occurred while forking child processes";
            break;

        case FILE_OPEN_FAIL:
        case FILE_CLOSE_FAIL:
            msg =   "An error occurred while opening or closing a file.\n"
                    "The file may be missing or permissions may not be granted";
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