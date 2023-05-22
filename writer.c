#include "writer.h"

void write_unjustified_col_row(FILE* fout, Row* row){

    Word* word;
    int words_count;
    int remaining_space;
    bool q_status;

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


void write_justified_col_row(FILE* fout, Row* row){

    Word* word;
    int words_count;

    int remaining_space;
    int next_space;
    int excess_space;

    bool q_status;

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

void write_column_row(FILE* fout, Row* row, int INTER_COL_SPACE, bool DO_NOT_JUSTIFY){

    Word* word;
    bool q_status;
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

bool write_page_row(FILE* fout, LinkedList* columns, int INTER_COL_SPACE){
    Node* node;
    Column* col;
    Row* curr_row;

    bool q_status;
    bool all_columns_empty;
    bool DO_NOT_JUSTIFY;

    node = columns->head;
    all_columns_empty = true;

    while(node != NULL){
        col = (Column*) node->data_ptr;

        if(!is_queue_empty(col->rows)){
            all_columns_empty = false;

            curr_row = (Row*) dequeue(col->rows, &q_status);

            if(!is_queue_empty(col->rows)){
                
                //do not justify if the next row is empty
                DO_NOT_JUSTIFY = (((Row*) peek_queue(col->rows))->char_count == 0);
            }
            else{
                DO_NOT_JUSTIFY = false;
            }

            write_column_row(fout, curr_row, INTER_COL_SPACE, DO_NOT_JUSTIFY);
        }

        //write inter column spacing only if it's not the last column
        if(node->next != NULL){
            fprintf(fout, "%-*s", INTER_COL_SPACE, "");
        }

        node = node->next;
    }

    if(!all_columns_empty){
        fprintf(fout, "\n");
    }

    return all_columns_empty;
}

int write_pages(const char* out_filename, Queue* pages, int INTER_COL_SPACE){

    FILE* fout;
    Page* page;

    bool q_status;
    bool all_columns_empty;

    //open output file
    if ((fout = fopen(out_filename, "w")) == NULL) {
        fprintf(stderr, "Cannot write to out.txt");
        return 1;
    }

    while(!is_queue_empty(pages)){
        page = (Page*) dequeue(pages, &q_status);

        all_columns_empty = false;

        while(!all_columns_empty){
            all_columns_empty = write_page_row(fout, page->columns, INTER_COL_SPACE);
        }
        
        if(!is_queue_empty(pages)){
            fprintf(fout, "\n%%%%%%\n\n");
        }
    }

    destroy_page(page);     //columns get destroyed too

    //NOTE: destroy_word and destroy_row must be called in the previous
    //functions due to the words/rows being dequeued, implying that 
    //destroy_page wouldn't call free() on them
	
    //close output file
    fclose(fout);
}
