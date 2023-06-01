#include "multi_process.h"

#define READ 0
#define WRITE 1

static int run_reader_process(Args* args, int pipe_rp[]){
    FILE* fin;
    Word* word;

    short read_file_status;
    short write_pipe_status;
    bool end_process;

    // Close unneeded pipe file descriptors
    close(pipe_rp[READ]);

    //open input file
    if((fin = fopen(args->INPUT_FILE, "r")) == NULL) {
        fprintf(stderr, "Cannot read from \"%s\". The file may be missing or corrupted", args->INPUT_FILE);
        return FILE_OPEN_FAIL;
    }

    end_process = false;

    while(!end_process){
        word = read_word_from_file(fin, args->COLUMN_WIDTH, &read_file_status);
        end_process = word->REACHED_EOF;

        write_word_to_pipe(pipe_rp[WRITE], word, &write_pipe_status);
        destroy_word(word);
    }

    fclose(fin);
    close(pipe_rp[WRITE]);

    return PROCESS_SUCCESS;
}

static int run_pager_process(Args* args, int pipe_rp[], int pipe_pw[]){

    Page* page;

    short read_pipe_status;
    short write_pipe_status;
    bool end_process;

    // Close unneeded pipe file descriptors
    close(pipe_rp[WRITE]);
    close(pipe_pw[READ]);

    end_process = false;

    while(!end_process){

        page = get_page_from_pipe(pipe_rp[READ], args->COLUMN_AMOUNT, args->COLUMN_HEIGHT, args->COLUMN_WIDTH);
        end_process = page->is_last_page;

        write_page_to_pipe(pipe_pw[WRITE], page, args->INTER_COL_SPACE, &write_pipe_status);
        //page already gets destroyed by write_page_to_pipe()
    }

    close(pipe_rp[READ]);
    close(pipe_pw[WRITE]);
    
    return PROCESS_SUCCESS;
}

static int run_writer_process(Args* args, int pipe_pw[]){

    FILE* fout;
    char* str;

    short read_pipe_status;
    short write_file_status;
    bool end_process;

    // Close unneeded pipe file descriptors
    close(pipe_pw[WRITE]);

    //open output file
    if((fout = fopen(args->OUTPUT_FILE, "w")) == NULL) {
        fprintf(stderr, "Cannot write to \"%s\". Permission may not be granted.", args->OUTPUT_FILE);
        return FILE_OPEN_FAIL;
    }
        
    //read pages until file EOF is reached

    end_process = false;
    
    while(!end_process){
        str = read_string_from_pipe(pipe_pw[READ], &read_pipe_status);
        end_process = (read_pipe_status == PIPE_READ_TERMINATED);

        write_string_to_file(fout, str, &write_file_status);
        free(str);
    }

    fclose(fout);
    close(pipe_pw[READ]);

    return PROCESS_SUCCESS;
}

int run_multi_process(Args* args){

    int pipe_rp[2];    // pipe between reader process and pager process
    int pipe_pw[2];    // pipe between pager process and writer process

    int reader_pid;
    int pager_pid;
    int writer_pid;

    // Open pipes and check for errors
    if(pipe(pipe_rp) < 0 || pipe(pipe_pw) < 0){
        return PIPE_OPEN_FAIL;
    };

    // Spawn reader process
    reader_pid = fork();

    if(reader_pid == -1){ //if error occurred
        return PROCESS_FORK_FAIL;
    }
    else if(reader_pid == 0){ // READER PROCESS
        return run_reader_process(args, pipe_rp);
    }

    // Spawn pager process
    pager_pid = fork();

    if(pager_pid == -1){ //if error occurred
        return PROCESS_FORK_FAIL;
    }
    else if(pager_pid == 0){ // PAGER PROCESS
        return run_pager_process(args, pipe_rp, pipe_pw);
    }

    // Spawn writer process
    writer_pid = fork();

    if(writer_pid == -1){ //if error occurred
        return PROCESS_FORK_FAIL;
    }
    else if(writer_pid == 0){ // WRITER PROCESS
        return run_writer_process(args, pipe_pw);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}
