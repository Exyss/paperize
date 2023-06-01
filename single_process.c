#include "single_process.h"

int run_single_process(Args* args){
    FILE* fin;
    FILE* fout;
    Queue* pages;
    Page* page; 

    short q_status;
    short write_status;
    bool stop_reading;
    bool all_columns_empty;

    pages = init_queue(INT_MAX);

    //open input file
    if((fin = fopen(args->INPUT_FILE, "r")) == NULL) {
        fprintf(stderr, "Cannot read from \"%s\". The file may be missing or corrupted", args->INPUT_FILE);
        return 1;
    }
    
    //read pages until file EOF is reached
    stop_reading = false;

    while(!stop_reading){

        page = get_page_from_file(fin, args->COLUMN_AMOUNT, args->COLUMN_HEIGHT, args->COLUMN_WIDTH);
        enqueue(pages, page, &q_status);

        stop_reading = page->is_last_page;
    }

    //close input file
    fclose(fin);

    //open output file
    if((fout = fopen(args->OUTPUT_FILE, "w")) == NULL) {
        fprintf(stderr, "Cannot write to \"%s\". Permission may not be granted.", args->OUTPUT_FILE);
        return 1;
    }

    while(!is_queue_empty(pages)){
        page = (Page*) dequeue(pages, &q_status);
        write_page_to_file(fout, page, args->INTER_COL_SPACE, &write_status);
    }
    
    //close output file
    fclose(fout);

    return 0;
}