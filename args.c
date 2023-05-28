#include "args.h"

/*
    Parses the given program arguments

    Sets status to INTEGER_PARSE_SUCCESS or INTEGER_PARSE_FAIL
*/
Args* parse_args(int argc, char* argv[]){
    Args* args = (Args*) malloc(sizeof(Args));
    int parsed_int;

    args->PROGRAM_NAME = argv[0];
    args->INPUT_FILE = argv[1];

    //set default values
    args->OUTPUT_FILE = "output.txt";
    args->COLUMN_AMOUNT = 3;
    args->COLUMN_HEIGHT = 20;
    args->COLUMN_WIDTH = 40;
    args->INTER_COL_SPACE = 10;
    args->USE_PARALLEL_MODE = false;
    args->PRINT_HELP = false;

    for(int i = 1; i < argc; i++){

        if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0){
            args->OUTPUT_FILE = argv[i+1];
        }
        else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            args->PRINT_HELP = true;
        }
        else if(strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--parallel") == 0){
            args->USE_PARALLEL_MODE = true;
        }
        else if(i+1 < argc){
            parsed_int = safe_pos_atoi(argv[i+1]);

            // (sadly) switch cases can't be used with strings
            if(strcmp(argv[i], "-C") == 0 || strcmp(argv[i], "--columns") == 0){

                if(parsed_int == -1){
                    free(args);
                    return NULL;
                }

                args->COLUMN_AMOUNT = parsed_int;
            }
            else if(strcmp(argv[i], "-H") == 0 || strcmp(argv[i], "--height") == 0){

                if(parsed_int == -1){
                    free(args);
                    return NULL;
                }

                args->COLUMN_HEIGHT = parsed_int;
            }
            else if(strcmp(argv[i], "-W") == 0 || strcmp(argv[i], "--width") == 0){

                if(parsed_int == -1){
                    free(args);
                    return NULL;
                }

                args->COLUMN_WIDTH = parsed_int;
            }
            else if(strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--space") == 0){

                if(parsed_int == -1){
                    free(args);
                    return NULL;
                }

                args->INTER_COL_SPACE = parsed_int;
            }
        }
    }

    return args;
}