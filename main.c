#include "queue.h"
#include "pager.h"
#include "writer.h"

int main(int argc, char* argv[]) {

    if(argc < 6){
        printf("Wrong usage! Run %s -h for more informations\n", argv[0]);
        return 1;
    }

    const char* INPUT_FILE = argv[1];
    const char* OUTPUT_FILE;
    const int COLUMN_AMOUNT = atoi(argv[2]);
    const int COLUMN_HEIGHT = atoi(argv[3]);
    const int COLUMN_WIDTH = atoi(argv[4]);
    const int INTER_COL_SPACE = atoi(argv[5]);

    //default value
    OUTPUT_FILE = "output.txt";

    Queue* pages;

    pages = read_pages(INPUT_FILE, COLUMN_AMOUNT, COLUMN_HEIGHT, COLUMN_WIDTH);
    write_pages(OUTPUT_FILE, pages, INTER_COL_SPACE);
    
    return 0;
}