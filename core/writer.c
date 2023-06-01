#include "writer.h"

/*
    Writes the given string on the given file.
    Sets status to FILE_WRITE_SUCCESS or FILE_WRITE_FAIL
*/
void write_string_to_file(FILE* fout, char* str, short* status){
    fprintf(fout, "%s", str);

    //check write error
    if(ferror(fout) != 0){
        *status = FILE_WRITE_FAIL;
    }
    else{
        *status = FILE_WRITE_SUCCESS;
    }
}

/*
    Writes the given string on the given pipe file descriptor.
    Sets status to PIPE_WRITE_SUCCESS or PIPE_WRITE_FAIL
*/
void write_string_to_pipe(int fd_out, char* str, bool close_pipe, short* status){

    int bytes_written = strlen(str) + 1;

    // PIPELINE PROTOCOL:
    // - write the amount of bytes to be read on the other side of the pipeline
    // - write the bytes to be read on the other side
    // - write termination character if the pipeline must be closed on the other side

    if(write(fd_out, &bytes_written, sizeof(int)) == -1 ||
       write(fd_out, str, bytes_written) == -1 ||
       write(fd_out, &close_pipe, sizeof(bool)) == -1){

        //write() returns -1 if an error occurred
        *status = PIPE_WRITE_FAIL;
    }
    else{
        *status = PIPE_WRITE_SUCCESS;
    }
}

/*
    Writes the given word on the given pipe file descriptor.

    Sets status to PIPE_WRITE_SUCCESS or PIPE_WRITE_FAIL
*/
void write_word_to_pipe(int fd_out, Word* word, short* status){

    int str_len = strlen(word->str) + 1;
    
    if(write(fd_out, &str_len, sizeof(int)) == -1 ||
       write(fd_out, word->str, str_len) == -1 ||
       write(fd_out, &(word->char_count), sizeof(int)) == -1 ||
       write(fd_out, &(word->REACHED_EOL), sizeof(bool)) == -1 ||
       write(fd_out, &(word->REACHED_EOF), sizeof(bool)) == -1){
        
        //write() returns -1 if an error occurred
        *status = PIPE_WRITE_FAIL;
    }
    else{
        *status = PIPE_WRITE_SUCCESS;
    }
}

/*
    Writes the given page to the given file using the given
    inter_col_space as the space between each column of the page

    The given page gets DESTROYED

    Sets status to FILE_WRITE_SUCCESS, FILE_WRITE_FAIL or FORMATTING_ERROR
*/
void write_page_to_file(FILE* fout, Page* page, int inter_col_space, short* status){

    char* write_str;

    bool all_columns_empty;

    while((all_columns_empty = are_all_cols_empty(page)) == false){

        write_str = format_page_row(page, inter_col_space);

        //TODO manage errors for format_page_row

        write_string_to_file(fout, write_str, status);
        free(write_str);
    }
    
    if(!page->is_last_page){
        write_string_to_file(fout, "\n%%%\n\n", status);
    }

    destroy_page(page);
}

/*
    Writes the given page to the given file using the given
    inter_col_space as the space between each column of the page

    The given page gets DESTROYED

    Sets status to FILE_WRITE_SUCCESS, FILE_WRITE_FAIL or FORMATTING_ERROR
*/
void write_page_to_pipe(int fd_out, Page* page, int inter_col_space, short* status){

    char* write_str;

    bool all_columns_empty;

    while((all_columns_empty = are_all_cols_empty(page)) == false){

        write_str = format_page_row(page, inter_col_space);

        //TODO manage errors for format_page_row

        write_string_to_pipe(fd_out, write_str, false, status);
        free(write_str);
    }
    
    if(!page->is_last_page){
        write_string_to_pipe(fd_out, "\n%%%\n\n", false, status);
    }
    else{
        write_string_to_pipe(fd_out, "", true, status);
    }

    destroy_page(page);
}