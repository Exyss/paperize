#include "multi_process.h"

#define READ 0
#define WRITE 1

/*
    Runs program in parallel using 3 child processes

    If all children successfully complete execution, returns PROCESS_SUCCESS
    If one or more children fail, the exit code of the first failing child is returned  
*/
int run_multi_process(Args* args){

    pid_t reader_pid;
    pid_t pager_pid;
    pid_t writer_pid;

    int pipe_rp[2];    // pipe between reader process and pager process
    int pipe_pw[2];    // pipe between pager process and writer process

    // Open pipes and check for errors
    if(pipe(pipe_rp) < 0 || pipe(pipe_pw) < 0){
        exit(PIPE_OPEN_FAIL);
    };

    // Spawn reader process
    if((reader_pid = fork()) == -1){ //if error occurred
        exit(PROCESS_FORK_FAIL);
    }
    else if(reader_pid == 0){
        exit(run_reader_process(args, pipe_rp));
    }

    // Spawn pager process
    if((pager_pid = fork()) == -1){
        kill_process(reader_pid);
        exit(PROCESS_FORK_FAIL);
    }
    else if(pager_pid == 0){ // PAGER PROCESS
        exit(run_pager_process(args, pipe_rp, pipe_pw));
    }

    // Spawn writer process
    if((writer_pid = fork()) == -1){
        kill_process(reader_pid);
        kill_process(pager_pid);
        exit(PROCESS_FORK_FAIL);
    }
    else if(writer_pid == 0){ // WRITER PROCESS
        exit(run_writer_process(args, pipe_pw));
    }

    //close all pipes in main process
    if(close(pipe_rp[READ]) == -1 || close(pipe_rp[WRITE]) == -1 ||
        close(pipe_pw[READ]) == -1 || close(pipe_pw[WRITE]) == -1){
        kill_process(reader_pid);
        kill_process(pager_pid);
        kill_process(writer_pid);
        exit(PIPE_CLOSE_FAIL);
    }

    //returns exit code
    return wait_processes(reader_pid, pager_pid, writer_pid);
}

/*
    Runs the reader process.

    In case of success, returns PROCESS_SUCCESS
    In case of error, returns the main process exit code
*/
static int run_reader_process(Args* args, int pipe_rp[]){
    FILE* fin;
    Word* word;

    short read_file_status;
    short write_pipe_status;
    bool keep_reading;

    // Close unneeded pipe file descriptors
    if(close(pipe_rp[READ]) == -1){
        return PIPE_CLOSE_FAIL;
    }

    //open input file
    if((fin = fopen(args->INPUT_FILE, "r")) == NULL) {
        //do not manage close() error in order to not update exit code
        close(pipe_rp[WRITE]);

        return FILE_OPEN_FAIL;
    }

    keep_reading = true;

    while(keep_reading){
        word = read_word_from_file(fin, args->COLUMN_WIDTH, &read_file_status);

        if(read_file_status != FILE_READ_SUCCESS){
            //do not manage close() error in order to not update exit code
            close(pipe_rp[WRITE]);

            return read_file_status;
        }

        keep_reading = !(word->REACHED_EOF);
        
        write_word_to_pipe(pipe_rp[WRITE], word, &write_pipe_status);
        destroy_word(word);

        if(write_pipe_status != PIPE_WRITE_SUCCESS){
            //do not manage close() error in order to not update exit code
            close(pipe_rp[WRITE]);
            
            return write_pipe_status;
        }
    }

    if(fclose(fin) != 0){
        //do not manage close() error in order to not update exit code
        close(pipe_rp[WRITE]);

        return FILE_CLOSE_FAIL;
    };
    
    if(close(pipe_rp[WRITE]) == -1){
        return PIPE_CLOSE_FAIL;
    }

    return PROCESS_SUCCESS;
}

/*
    Runs the pager process.

    In case of success, returns PROCESS_SUCCESS
    In case of error, returns the main process exit code
*/
static int run_pager_process(Args* args, int pipe_rp[], int pipe_pw[]){

    Page* page;

    short read_pipe_status;
    short write_pipe_status;
    bool end_process;

    // Close unneeded pipe file descriptors
    if(close(pipe_rp[WRITE]) == -1 || close(pipe_pw[READ]) == -1){
        return PIPE_CLOSE_FAIL;
    }

    end_process = false;

    while(!end_process){

        page = get_page_from_pipe(pipe_rp[READ], args->COLUMN_AMOUNT, args->COLUMN_HEIGHT, args->COLUMN_WIDTH, &read_pipe_status);

        if(read_pipe_status != PIPE_READ_SUCCESS){
            //do not manage close() error in order to not update exit code
            close(pipe_rp[READ]);
            close(pipe_pw[WRITE]);

            return read_pipe_status;
        }

        end_process = page->is_last_page;        

        write_page_to_pipe(pipe_pw[WRITE], page, args->INTER_COL_SPACE, &write_pipe_status);
        //page already gets destroyed by write_page_to_pipe()

        if(write_pipe_status != PIPE_WRITE_SUCCESS){
            //do not manage close() error in order to not update exit code
            close(pipe_rp[READ]);
            close(pipe_pw[WRITE]);

            return write_pipe_status;
        }
    }

    if(close(pipe_rp[READ]) == -1 || close(pipe_pw[WRITE]) == -1){
        return PIPE_CLOSE_FAIL;
    }
    
    return PROCESS_SUCCESS;
}

/*
    Runs the writer process.

    In case of success, returns PROCESS_SUCCESS
    In case of error, returns the main process exit code
*/
static int run_writer_process(Args* args, int pipe_pw[]){

    FILE* fout;
    char* str;

    short read_pipe_status;
    short write_file_status;
    bool end_process;

    // Close unneeded pipe file descriptors
    if(close(pipe_pw[WRITE]) == -1){
        return PIPE_CLOSE_FAIL;
    }

    //open output file
    if((fout = fopen(args->OUTPUT_FILE, "w")) == NULL) {
        //do not manage close() error in order to not update exit code
        close(pipe_pw[READ]);
        return FILE_OPEN_FAIL;
    }
        
    //read pages until file EOF is reached

    end_process = false;
    
    while(!end_process){
        str = read_string_from_pipe(pipe_pw[READ], &read_pipe_status);

        if(read_pipe_status != PIPE_READ_SUCCESS && read_pipe_status != PIPE_READ_TERMINATED){
            //do not manage close() error in order to not update exit code
            close(pipe_pw[READ]);
            
            return read_pipe_status;
        }

        end_process = (read_pipe_status == PIPE_READ_TERMINATED);

        write_string_to_file(fout, str, &write_file_status);
        free(str);

        if(write_file_status != FILE_WRITE_SUCCESS){
            //do not manage close() error in order to not update exit code
            close(pipe_pw[READ]);

            return write_file_status;
        }

    }


    if(fclose(fout) != 0){
        return FILE_CLOSE_FAIL;
    };

    if(close(pipe_pw[READ]) == -1){
        return PIPE_CLOSE_FAIL;
    }

    return PROCESS_SUCCESS;
}

/*
    Kills the process with the given pid

    Returns False if the process was not found or
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

    if(kill(pid, 0) == -1 && errno != ESRCH){
        return false;
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


/*
    Waits for the 3 parallel child processes to terminate or
    exit due to an error. 

    If a child exits due to an error, a SIGTERM signal is send
    to all other childrens in order to stop their execution.
*/
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