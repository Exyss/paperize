#include "single_process.h"

/*
    Runs the whole program as a single process

    Returns success or error exit code
*/
int run_single_process(Args* args){
    FILE* fin;
    FILE* fout;
    Queue* pages;
    Page* page; 

    short q_status;
    short program_status;
    bool stop_reading;
    bool all_columns_empty;

    pages = init_queue(INT_MAX);

    //if initialization has failed
    if(pages == NULL){
        return QUEUE_ALLOC_FAIL;
    }

    
    //open input file
    if((fin = fopen(args->INPUT_FILE, "r")) == NULL) {
        free(pages);
        return FILE_OPEN_FAIL;
    }
    
    //read pages until file EOF is reached
    stop_reading = false;

    while(!stop_reading){

        page = get_page_from_file(fin, args->COLUMN_AMOUNT, args->COLUMN_HEIGHT, args->COLUMN_WIDTH, &program_status);

        if(page == NULL){
            return program_status;
        }

        enqueue(pages, page, &q_status);

        if(q_status != ENQUEUE_SUCCESS){
            destroy_page(page);
            free(pages);
            return PAGE_ENQUEUE_FAIL;
        }

        stop_reading = page->is_last_page;
    }

    //close input file
    if(fclose(fin) != 0){
        return FILE_CLOSE_FAIL;
    };

    //open output file
    if((fout = fopen(args->OUTPUT_FILE, "w")) == NULL) {
        return FILE_OPEN_FAIL;
    }
    
    while(!is_queue_empty(pages)){
        page = (Page*) dequeue(pages, &q_status);

        if(q_status != DEQUEUE_SUCCESS){
            return PAGE_DEQUEUE_FAIL;
        }


        write_page_to_file(fout, page, args->INTER_COL_SPACE, &program_status);
        
        if(program_status != FILE_WRITE_SUCCESS){
            
            //do not update error
            fclose(fout);
            return program_status;
        }

    }
    
    //close output file
    if(fclose(fout) != 0){
        return FILE_CLOSE_FAIL;
    };

    return PROCESS_SUCCESS;
}