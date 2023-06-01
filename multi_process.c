#include "multi_process.h"
#include <errno.h>
#include <stdbool.h>
#include <signal.h>

#define READ 0
#define WRITE 1

static int run_reader_process(Args* args, int pipe_rp[]){
    FILE* fin;
    Word* word;

    short read_file_status;
    short write_pipe_status;
    bool keep_reading;

    // Close unneeded pipe file descriptors
    close(pipe_rp[READ]);

    //open input file
    if((fin = fopen(args->INPUT_FILE, "r")) == NULL) {
        fprintf(stderr, "Cannot read from \"%s\". The file may be missing or corrupted", args->INPUT_FILE);
        return FILE_OPEN_FAIL;
    }

    keep_reading = true;

    while(keep_reading){
        word = read_word_from_file(fin, args->COLUMN_WIDTH, &read_file_status);
        keep_reading = !(word->REACHED_EOF);

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

    pid_t reader_pid;
    pid_t pager_pid;
    pid_t writer_pid;

    int pipe_rp[2];    // pipe between reader process and pager process
    int pipe_pw[2];    // pipe between pager process and writer process

    // Open pipes and check for errors
    if(pipe(pipe_rp) < 0 || pipe(pipe_pw) < 0){
        return PIPE_OPEN_FAIL;
    };

    // Spawn reader process
    reader_pid = fork();

    if(reader_pid == -1){ //if error occurred
        return PROCESS_FORK_FAIL;
    }
    else if(reader_pid == 0){
        return run_reader_process(args, pipe_rp);
    }

    // Spawn pager process
    pager_pid = fork();

    if(pager_pid == -1){
        //uccidi 1
        return PROCESS_FORK_FAIL;
    }
    else if(pager_pid == 0){ // PAGER PROCESS
        return run_pager_process(args, pipe_rp, pipe_pw);
    }

    // Spawn writer process
    writer_pid = fork();

    if(writer_pid == -1){
        //uccidi 1 e 2
        return PROCESS_FORK_FAIL;
    }
    else if(writer_pid == 0){ // WRITER PROCESS
        return run_writer_process(args, pipe_pw);
    }

    //returns exit code
    return wait_processes(reader_pid, pager_pid, writer_pid);
}

/*
    Kills the process with the given pid

    Returns -1 if the process is already dead or
    if an error occurred while sending SIGTERM signal
*/
bool kill_process(pid_t pid){
    /*
        kill(pid, 0) sends a signal to the process without 
        killing it and returns -1 if the signaling failed.
        Used to check if the process is alive or not.

        errno is a global variable used by the <errno.h> library.
        It gets set to ESRCH after using kill(pid, 0)
        if the process was not found
    */

    if(kill(pid, 0) == -1){
        if(errno != ESRCH){
            return false;
        }
    }

    /*
        kill(pid, SIGTERM) sends a signal to the process without 
        killing it and returns -1 if the signaling failed
    */
    if(kill(pid, SIGTERM) == -1){
        return false;
    }

    return true;
}

short wait_processes(pid_t reader_pid, pid_t pager_pid, pid_t writer_pid){

    short exit_code;
    short other_pid_1;
    short other_pid_2;

    pid_t pid[3] = {reader_pid, pager_pid, writer_pid};
    bool pid_closed[3] = {false, false, false};
    int pid_status[3];

    pid_t wait_status;

    exit_code = PROCESS_SUCCESS;
    
    while(!pid_closed[0] || !pid_closed[1] || !pid_closed[2]){

        for(short i = 0; i < 3; i++){
            
            if(pid_closed[i]){
                continue;
            }

            /*
                waitpid() returns he given pid (which is always > 0) if the child process was
                successfully joined to the parent process or a value < 0 if an error occurred
            */
            wait_status = waitpid(pid[i], &pid_status[i], WNOHANG);

            if(wait_status != 0){

                pid_closed[i] = true;

                /*  If an error occurred in the closed child, immediately kill all the other childs

                    - WIFEXITED returns true if the child terminated normally, while
                    - WEXITSTATUS returns the exit status of the child
                */
                if(wait_status < 0 || !WIFEXITED(pid_status[i]) || WEXITSTATUS(pid_status[i]) != PROCESS_SUCCESS){
                    other_pid_1 = (i + 1) % 3;
                    other_pid_2 = (i + 2) % 3;

                    pid_closed[other_pid_1] = kill_process(pid[other_pid_1]);  //kill the first other process
                    pid_closed[other_pid_2] = kill_process(pid[other_pid_2]);  //kill the second other process

                    if(exit_code == PROCESS_SUCCESS){
                        exit_code = WEXITSTATUS(pid_status[i]);
                    }
                }
            }
        }
    }

    return exit_code;
}