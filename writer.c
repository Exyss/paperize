#include "writer.h"

/*
    Writes the given string on the given file.
    Sets status to WRITE_SUCCESS or WRITE_FAIL
*/
void write_to_file(FILE* fout, char* str, short* status){
    fprintf(fout, "%s", str);

    //check write error
    if(ferror(fout) != 0){
        *status = WRITE_FAIL;
    }
    else{
        *status = WRITE_SUCCESS;
    }
}

/*
    Write the given row on the given file
    without justified alignment
*/
void write_unjustified_col_row(FILE* fout, Row* row){

    Word* word;
    int words_count;
    int remaining_space;

    short q_status;
    short write_status;
    
    words_count = queue_size(row->words);
    remaining_space = row->MAX_CHARS - row->char_count;

    //add spacing for all rows except the last one            
    while(words_count > 1){
        
        word = (Word*) dequeue(row->words, &q_status);

        fprintf(fout, "%s", right_pad_string(word->str, 1));

        words_count--;
        remaining_space--;
        destroy_word(word);
    }

    word = (Word*) dequeue(row->words, &q_status);

    //write the last word with remaining space
    fprintf(fout, "%s", right_pad_string(word->str, remaining_space));

    destroy_word(word);
}


/*
    Write the given row on the given file
    with justified alignment
*/
void write_justified_col_row(FILE* fout, Row* row){

    Word* word;
    int words_count;

    int remaining_space;
    int next_space;
    int excess_space;

    short q_status;

    words_count = queue_size(row->words);
    remaining_space = row->MAX_CHARS - row->char_count;

    //add spacing for all rows except the last one            
    while(words_count > 1){
        
        //equally divide remaining space (floor rounded)
        next_space = remaining_space / (words_count - 1);

        //ensure that it's at least one space
        next_space = (next_space > 0) ? next_space : 1; 

        word = (Word*) dequeue(row->words, &q_status);

        fprintf(fout, "%s", right_pad_string(word->str, next_space));

        words_count--;
        remaining_space -= next_space;
        destroy_word(word);
    }

    word = (Word*) dequeue(row->words, &q_status);

    //write the last word with remaining space
    fprintf(fout, "%s", left_pad_string(word->str, remaining_space));

    destroy_word(word);
}

/*
    Writes the given column row on the given file
    using the given INTER_COL_SPACE (space between each column)
*/
void write_column_row(FILE* fout, Row* row, int INTER_COL_SPACE, bool DO_NOT_JUSTIFY){

    Word* word;
    short q_status;
    int words_count;

    words_count = queue_size(row->words);
    
    //write an empty row
    if(words_count == 0){
        fprintf(fout, "%s", right_pad_string("", row->MAX_CHARS));  //pad the whole row
    }
    //if it's the last row of a paragraph or it's a single word
    else if(DO_NOT_JUSTIFY || words_count == 1){
        write_unjustified_col_row(fout, row);
    }
    else{
        write_justified_col_row(fout, row);
    }

    destroy_row(row);
}

/*
    Writes a row from each column of the given column list
    using the given INTER_COL_SPACE (space between each column)
*/
bool write_page_row(FILE* fout, LinkedList* columns, int INTER_COL_SPACE, bool IS_LAST_PAGE){
    Node* head;
    Node* node;
    Column* col;
    Row* curr_row;
    Row* next_row;

    short q_status;
    bool all_columns_empty;
    bool DO_NOT_JUSTIFY;

    head = columns->head;
    node = head;
    all_columns_empty = true;

    while(node != NULL){
        col = (Column*) node->data_ptr;

        if(!is_queue_empty(col->rows)){
            all_columns_empty = false;

            //write inter column spacing on the left only if it's not the first column
            if(node != head){
                fprintf(fout, "%s", right_pad_string("", INTER_COL_SPACE));
            }

            curr_row = (Row*) dequeue(col->rows, &q_status);
            next_row = (Row*) peek_queue(col->rows);    //peek doesn't dequeue

            //if there is another row
            if(next_row != NULL){
                //do not justify if the next row is empty
                DO_NOT_JUSTIFY = ((next_row)->char_count == 0);
            }
            else{
                //if it's the last row of the last column of the last page
                if(node->next == NULL && IS_LAST_PAGE){
                    DO_NOT_JUSTIFY = true;
                }
                else{
                    DO_NOT_JUSTIFY = false;
                }
            }

            write_column_row(fout, curr_row, INTER_COL_SPACE, DO_NOT_JUSTIFY);
        }

        node = node->next;
    }

    if(!all_columns_empty){
        fprintf(fout, "\n");
    }

    return all_columns_empty;
}

/*
    Writes all the given pages on the given file
    using the given INTER_COL_SPACE (space between each column)
*/
int write_pages(char* out_filename, Queue* pages, int INTER_COL_SPACE){

    FILE* fout;
    Page* page;

    short q_status;
    bool all_columns_empty;
    bool is_last_page;

    //open output file
    if ((fout = fopen(out_filename, "w")) == NULL) {
        fprintf(stderr, "Cannot write to out.txt");
        return 1;
    }

    while(!is_queue_empty(pages)){
        page = (Page*) dequeue(pages, &q_status);

        all_columns_empty = false;
        is_last_page = is_queue_empty(pages);

        while(!all_columns_empty){
            all_columns_empty = write_page_row(fout, page->columns, INTER_COL_SPACE, is_last_page);
        }
        
        if(!is_last_page){
            fprintf(fout, "\n%%%%%%\n\n");
        }
    }

    destroy_page(page);     //columns get destroyed too

    /*
        NOTE: destroy_word and destroy_row must be called in the previous
        functions due to the words/rows being dequeued, implying that 
        destroy_page wouldn't call free() on them
    */
    
    //close output file
    fclose(fout);
}
