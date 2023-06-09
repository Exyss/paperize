#include "args.h"

/*
    Parses the given program arguments

    Returns NULL if allocation or parsing has failed
*/
Args* parse_args(int argc, char* argv[]){
    Args* args = (Args*) malloc(sizeof(Args));

    // if malloc has failed
    if(args == NULL){
        return NULL;
    }

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

            // Check if the current argument is a valid flag
            // and then check parsed_int value (this control MUST be
            // done afterwards)
            if(strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--columns") == 0){
                
                if(parsed_int == -1){
                    free(args);
                    return NULL;
                }

                args->COLUMN_AMOUNT = parsed_int;
            }
            else if(strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--rows") == 0){

                if(parsed_int == -1){
                    free(args);
                    return NULL;
                }

                args->COLUMN_HEIGHT = parsed_int;
            }
            else if(strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0){

                if(parsed_int == -1){
                    free(args);
                    return NULL;
                }

                args->COLUMN_WIDTH = parsed_int;
            }
            else if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--space") == 0){

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