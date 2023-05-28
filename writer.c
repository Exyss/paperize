#include "writer.h"

/*
    Writes the given string on the given file.
    Sets status to FILE_WRITE_SUCCESS or FILE_WRITE_FAIL
*/
void write_to_file(FILE* fout, char* str, short* status){
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
void write_to_pipe(int fd_out, char* str, short* status){
    // fprintf(fout, "%s", str);

    // //check write error
    // if(ferror(fout) != 0){
    //     *status = PIPE_WRITE_FAIL;
    // }
    // else{
    //     *status = PIPE_WRITE_SUCCESS;
    // }
}

/*
    Returns a copy of the given row_str concatenated to the next word of the given row.

    The used word gets DEQUEUED and DESTROYED.
    The given row_str string is left unchanged.

    Returns NULL if the given row has no words left
    or if a dequeueing error happened
*/
static char* append_next_row_word(Row* row, char* row_str, int pad_amount, bool apply_right_padding){

    Word* next_word;

    char* tmp_str;
    char* new_str;

    short q_status;

    //add if is_queue_empty(row->words) error

    next_word = (Word*) dequeue(row->words, &q_status);

    // //if dequeueing has failed
    // if(is_queue_empty(row->words) q_status == DEQUEUE_FAIL){
    //     free(row_str);

    //     if(tmp_ptr != NULL){
    //         free(tmp_ptr);
    //     }

    //     if(to_be_appended_str != NULL){
    //         free(to_be_appended_str);
    //     }

    //     return NULL;
    // }
    
    //pad next word
    if(apply_right_padding){
        tmp_str = right_pad_string(next_word->str, pad_amount);
    }
    else{
        tmp_str = left_pad_string(next_word->str, pad_amount);
    }

    //append next word
    new_str = safe_strcat(row_str, tmp_str);
    free(tmp_str);

    destroy_word(next_word);

    return new_str;
}

/*
    Formats the given column row into a new fixed size string.
    Justified alignment is used if apply_justification is set to True.

    The given row gets DESTROYED.

    Returns NULL if an error occurred.
*/
static char* format_column_row(Row* row, bool apply_justification){

    char* out_str;
    char* tmp_str;

    int words_count;
    int next_space_amount;
    int remaining_space;
    bool apply_right_padding;

    short q_status;

    //assign empty heap string
    out_str = (char*) malloc(1);
    *out_str = '\0';

    words_count = queue_size(row->words);
    remaining_space = row->MAX_CHARS - row->char_count;

    //return an empty string
    if(words_count == 0){
        out_str = right_pad_string(out_str, remaining_space);
        return out_str;
    }

    //return a string containing only the first word
    if(words_count == 1){
        apply_right_padding = true;
        out_str = append_next_row_word(row, out_str, remaining_space, apply_right_padding);

        return out_str;
    }

    apply_right_padding = true;

    //add spacing for all rows except the last one            
    while(words_count > 1){
        
        if(apply_justification){
            //equally divide remaining space (floor rounded)
            next_space_amount = remaining_space / (words_count - 1);

            //ensure that it's at least one space
            next_space_amount = (next_space_amount > 0) ? next_space_amount : 1; 
        }
        else{
            next_space_amount = 1;
        }

        //append next word with right padding
        tmp_str = out_str;
        out_str = append_next_row_word(row, out_str, next_space_amount, true);
        free(tmp_str);

        words_count--;
        remaining_space -= next_space_amount;
    }

    //use right padding for last word only if not applying justification
    apply_right_padding = !apply_justification;

    //append last word with left padding
    tmp_str = out_str;
    out_str = append_next_row_word(row, out_str, remaining_space, apply_right_padding);
    free(tmp_str);

    destroy_row(row);

    return out_str;
}

/*
    Formats a page row made up of a formatted row from each column
    of the given column list using the given inter_col_space 
    as the space between each column

    The used rows get DEQUEUED and DESTROYED

    Returns NULL if an error occurred
*/
static char* format_page_row(Page* page, int inter_col_space){
    
    char* out_str;
    char* next_str;
    char* tmp_str;

    Node* head;
    Node* node;
    Column* col;

    Row* curr_row;
    Row* next_row;

    short q_status;
    bool apply_justification;

    //start from first column
    head = page->columns->head;
    node = head;

    //assign empty heap string
    out_str = (char*) malloc(1);
    *out_str = '\0';

    //until all nodes have been analyzed
    while(node != NULL){
        col = (Column*) node->data_ptr;

        if(!is_queue_empty(col->rows)){
            curr_row = (Row*) dequeue(col->rows, &q_status);
            next_row = (Row*) peek_queue(col->rows);    //peek doesn't dequeue

            //if there is another row
            if(next_row != NULL){
                //justify only if the next row is not empty
                apply_justification = (next_row->char_count != 0);
            }
            //if it's the last row of the last column of the last page
            else if(node->next == NULL && page->is_last_page){
                apply_justification = false;
            }
            else{
                apply_justification = true;
            }

            //add next column row
            next_str = format_column_row(curr_row, apply_justification);

            //add inter column spacing on the left only if it's not the first column
            if(node != head){
                tmp_str = next_str;
                next_str = left_pad_string(tmp_str, inter_col_space);
                free(tmp_str);
            }
            
            //append and free temporary or previous strings
            tmp_str = out_str;
            out_str = safe_strcat(out_str, next_str);
            free(tmp_str);
            free(next_str);
        }

        node = node->next;
    }

    //add \n
    tmp_str = out_str;
    out_str = safe_strcat(out_str, "\n");
    free(tmp_str);

    return out_str;
}

/*
    Returns true if all the columns of the given page have no rows left
*/
static bool are_all_cols_empty(Page* page){

    Column* col;
    Node* node = page->columns->head;

    while(node != NULL){
        col = node->data_ptr;
        if(!is_queue_empty(col->rows)){
            return false;
        }

        node = node->next;
    }

    return true;
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

        write_to_file(fout, write_str, status);
        free(write_str);
    }
    
    if(!page->is_last_page){
        write_to_file(fout, "\n%%%\n\n", status);
    }

    destroy_page(page);
}

/*
    Writes all the given pages on the given file
    using the given inter_col_space (space between each column)

    The given pages get DEQUEUED and DESTROYED
*/
void write_pages_to_file(char* out_filename, Queue* pages, int inter_col_space){

    FILE* fout;
    Page* page;

    short q_status;
    short write_status;
    bool all_columns_empty;

    //open output file
    if ((fout = fopen(out_filename, "w")) == NULL) {
        fprintf(stderr, "Cannot write to out.txt");
        // return 1;
    }

    while(!is_queue_empty(pages)){
        page = (Page*) dequeue(pages, &q_status);
        write_page_to_file(fout, page, inter_col_space, &write_status);
    }
    
    //close output file
    fclose(fout);
}
