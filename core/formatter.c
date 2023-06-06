#include "formatter.h"

/*
    Returns a copy of the given row_str concatenated to the next word of the given row.

    The used word gets DEQUEUED and DESTROYED.
    The given row_str string is left unchanged.

    Returns NULL if the given row has no words left or if an error occurred

    In case of error, sets status to WORD_DEQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static char* append_next_row_word(Row* row, char* row_str, int pad_amount, bool apply_right_padding, short* status){

    Word* next_word;

    char* tmp_str;
    char* new_str;

    short q_status;

    next_word = (Word*) dequeue(row->words, &q_status);

    if(q_status != DEQUEUE_SUCCESS){
        *status = WORD_DEQUEUE_FAIL;
        return NULL;
    }
    
    //pad next word
    if(apply_right_padding){
        tmp_str = right_pad_string(next_word->str, pad_amount);
    }
    else{
        tmp_str = left_pad_string(next_word->str, pad_amount);
    }

    if(tmp_str == NULL){
        *status = STRING_ALLOC_FAIL;
        destroy_word(next_word);
        return NULL;
    }

    //append next word
    new_str = safe_strcat(row_str, tmp_str);

    free(tmp_str);
    destroy_word(next_word);

    if(new_str == NULL){
        *status = STRING_ALLOC_FAIL;
        return NULL;
    }

    return new_str;
}

/*
    Formats the given column row into a new fixed size string.
    Justified alignment is used if apply_justification is set to True.

    The given row gets DESTROYED.

    Returns NULL if an error occurred.

    In case of error, sets status to WORD_DEQUEUE_FAIL or STRING_ALLOC_FAIL
*/
static char* format_column_string(Row* row, bool apply_justification, short* status){

    char* out_str;
    char* tmp_str;

    int words_count;
    int next_space_amount;
    int remaining_space;
    bool apply_right_padding;

    short q_status;

    //assign empty heap string
    // => needed just for consistency with 
    //    the following while loop
    out_str = (char*) malloc(1);

    //if malloc has failed
    if(out_str == NULL){
        *status = STRING_ALLOC_FAIL;
        destroy_row(row);
        return NULL;
    }

    //set empty string
    *out_str = '\0';

    words_count = queue_size(row->words);
    remaining_space = row->max_chars - row->char_count;

    //return an empty string
    if(words_count == 0){
        out_str = right_pad_string(out_str, remaining_space);

        //if allocation has failed
        if(out_str == NULL){
            *status = STRING_ALLOC_FAIL;
            destroy_row(row);
            return NULL;
        }

        return out_str;
    }

    //return a string containing only the first word
    if(words_count == 1){
        apply_right_padding = true;
        out_str = append_next_row_word(row, out_str, remaining_space, apply_right_padding, status);

        //if allocation has failed
        if(out_str == NULL){
            //status already gets set by append_next_row_word()
            destroy_row(row);
            return NULL;
        }

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
        out_str = append_next_row_word(row, out_str, next_space_amount, true, status);
        free(tmp_str);

        //if allocation has failed
        if(out_str == NULL){
            //status already gets set by append_next_row_word()
            destroy_row(row);
            return NULL;
        }

        words_count--;
        remaining_space -= next_space_amount;
    }

    //use right padding for last word only if not applying justification
    apply_right_padding = !apply_justification;

    //append last word with left padding
    tmp_str = out_str;
    out_str = append_next_row_word(row, out_str, remaining_space, apply_right_padding, status);
    free(tmp_str);

    destroy_row(row);

    //if allocation has failed
    if(out_str == NULL){
        //status already gets set by append_next_row_word()
        destroy_row(row);
        return NULL;
    }

    return out_str;
}

/*
    Formats the next page string using the rows of the columns of the page.
    Each column row string is separated by inter_col_space spaces.

    The used rows get DEQUEUED and DESTROYED

    Returns NULL if an error occurred

    In case of success, sets status to FORMATTING_SUCCESS.
    In case of error, sets status to ROW_DEQUEUE_FAIL, WORD_DEQUEUE_FAIL or STRING_ALLOC_FAIL.
*/
char* format_page_string(Page* page, int inter_col_space, short* status){
    
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

    if(out_str == NULL){
        *status = STRING_ALLOC_FAIL;
        return NULL;
    }
    *out_str = '\0';

    //until all nodes have been analyzed
    while(node != NULL){
        col = (Column*) node->data_ptr;

        if(!is_queue_empty(col->rows)){
            curr_row = (Row*) dequeue(col->rows, &q_status);

            if(curr_row == NULL){
                *status = ROW_DEQUEUE_FAIL;
                return NULL;
            }

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
            next_str = format_column_string(curr_row, apply_justification, status);

            if(next_str == NULL){
                //status already gets set by format_column_string()
                free(out_str);
                return NULL;
            }

            //add inter column spacing on the left only if it's not the first column
            if(node != head){
                tmp_str = next_str;
                next_str = left_pad_string(tmp_str, inter_col_space);
                free(tmp_str);

                if(next_str == NULL){
                    *status = STRING_ALLOC_FAIL;
                    free(out_str);
                    return NULL;
                }
            }
            
            //append and free temporary or previous strings
            tmp_str = out_str;
            out_str = safe_strcat(out_str, next_str);
            free(tmp_str);
            free(next_str);

            if(out_str == NULL){
                *status = STRING_ALLOC_FAIL;
                return NULL;
            }
        }

        node = node->next;
    }

    //add \n
    tmp_str = out_str;
    out_str = safe_strcat(out_str, "\n");
    free(tmp_str);

    if(out_str == NULL){
        *status = STRING_ALLOC_FAIL;
        return NULL;
    }

    *status = FORMATTING_SUCCESS;
    return out_str;
}

