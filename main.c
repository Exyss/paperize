#include "single_process.h"
#include "multi_process.h"

int main(int argc, char* argv[]) {

    Args* args = (Args*) parse_args(argc, argv);

    if(args == NULL){
        printf("Wrong usage! Run %s -h for more informations\n", argv[0]);
        return 1;
    }
    else if(args->PRINT_HELP){
        printf("HELP MESSAGE\n");
        return 0;
    }

    if(args->USE_PARALLEL_MODE){
        return run_multi_process(args);
    }
    else{
        return run_single_process(args);
    }
}
