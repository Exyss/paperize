#include "queue.h"
#include "pager.h"
#include "writer.h"
#include "string_utils.h"
#include "args.h"

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

    Queue* pages;
    
    pages = read_pages(args->INPUT_FILE, args->COLUMN_AMOUNT, args->COLUMN_HEIGHT, args->COLUMN_WIDTH);
    write_pages_to_file(args->OUTPUT_FILE, pages, args->INTER_COL_SPACE);
    return 0;
}
