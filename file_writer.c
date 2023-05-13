
#include "queue.h"
#include "file_reader.h"
#include "structs.h"
#include "linked_list.h"
#include "node.h"

void write_column_row(FILE* fout, Row* row, int INTER_COL_SPACE){
    Word* word;

    bool q_status;
    int char_diff;
    int words_count;
    int inter_word_space;
    int excess_space;
    int bytes_to_print;

    char_diff = row->MAX_CHARS - row->char_count;
    words_count = queue_size(row->words);
    
    //NOTE: by using %-*s in fprintf, the next given argument
    //      will define the amount of bytes to be printed
    //      (if the given string is shorter than that amount,
    //      a right pad will be automatically added)
    //
    //NOTE: %*s works the same as %-*s, but the pad
    //      gets added on the left 

    switch(words_count){
        case 0:
            //write a whole empty row
            fprintf(fout, "%-*s", row->MAX_CHARS, "");  //pad the whole row
            break;

        case 1:
            //write the only word and fill the remaining row with spaces
            word = (Word*) dequeue(row->words, &q_status);
            fprintf(fout, "%-*s", row->MAX_CHARS, word->str);   //pad the remaining row
            break;

        default:
            //equally divide spaces (floor rounded)
            inter_word_space = char_diff / (words_count - 1);
            excess_space = char_diff % (words_count - 1);

            //ensure that it's at least one space
            inter_word_space = (inter_word_space > 0) ? inter_word_space : 1; 

            //add spacing for all rows except the last one            
            while(words_count > 1){
                word = (Word*) dequeue(row->words, &q_status);
                words_count--;

                bytes_to_print = strlen(word->str) + inter_word_space;
                fprintf(fout, "%-*s", bytes_to_print, word->str);
            }

            //write the last word with left spacing if needed
            word = (Word*) dequeue(row->words, &q_status);

            bytes_to_print = strlen(word->str) + excess_space;
            fprintf(fout, "%*s", bytes_to_print, word->str);
    }
}

bool write_page_row(FILE* fout, LinkedList* columns, int INTER_COL_SPACE){
    Node* node;
    Column* col;
    Row* row;

    bool q_status;
    bool all_columns_empty;

    node = columns->head;
    all_columns_empty = true;

    while(node != NULL){
        col = (Column*) node->data_ptr;

        if(!is_queue_empty(col->rows)){
            all_columns_empty = false;

            row = (Row*) dequeue(col->rows, &q_status);
            write_column_row(fout, row, INTER_COL_SPACE);
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
    Node* node;
    Column* col;
    Row* row;
    Word* word;

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

        fprintf(fout, "\n%%%%%%\n\n");
    }
	
    //close output file
    fclose(fout);
}
