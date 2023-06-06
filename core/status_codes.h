#ifndef STATUS_CODES_H
#define STATUS_CODES_H

//STATUS CODES

#define PROCESS_SUCCESS         0       // Program completed without errors
#define WRONG_USAGE             1       // The given program arguments are invalid
#define PROCESS_FORK_FAIL       3       // An error occurred while forking main process

#define FILE_OPEN_FAIL          4       // An error occurred while opening a file
#define FILE_CLOSE_FAIL         5       // An error occurred while closing a file
#define FILE_READ_SUCCESS       6       // Data was correctly read from a file
#define FILE_READ_FAIL          7       // An error occurred while reading from a file
#define FILE_WRITE_SUCCESS      8       // Data was correctly written to a file buffer
#define FILE_WRITE_FAIL         9       // An error occurred while writing to a file

#define PIPE_OPEN_FAIL          10      // An error occurred while opening a pipe
#define PIPE_CLOSE_FAIL         11      // An error occurred while closing a pipe
#define PIPE_READ_SUCCESS       12      // Data was correctly read from a pipe
#define PIPE_READ_FAIL          13      // An error occurred while reading from a pipe buffer
#define PIPE_READ_TERMINATED    14      // Signal to stop reading from a pipe was received
#define PIPE_WRITE_SUCCESS      15      // Data was correctly written to a pipe buffer
#define PIPE_WRITE_FAIL         16      // An error occurred while writing to a pipe

#define PAGE_INIT_FAIL          17      // Page initialization or allocation has failed
#define PAGE_ENQUEUE_FAIL       18      // An error occurred while enqueueing a page in a queue
#define PAGE_DEQUEUE_FAIL       19      // An error occurred while dequeueing a page from a queue

#define COLUMN_INIT_FAIL        20      // Column initialization or allocation has failed
#define COLUMN_APPEND_FAIL      21      // An error occurred while appending a column to a linked list

#define ROW_INIT_FAIL           22      // Row initialization or allocation has failed
#define ROW_ENQUEUE_FAIL        23      // An error occurred while enqueueing a row in a queue
#define ROW_DEQUEUE_FAIL        24      // An error occurred while dequeueing a row from a queue

#define WORD_INIT_FAIL          25      // Word initialization or allocation has failed
#define WORD_ENQUEUE_FAIL       26      // An error occurred while enqueueing a word in a queue
#define WORD_DEQUEUE_FAIL       27      // An error occurred while dequeueing a word from a queue

#define STRING_ALLOC_FAIL       28      // String allocation has failed
#define QUEUE_ALLOC_FAIL        29      // Queue allocation has failed
#define FORMATTING_SUCCESS      30      // String formatting completed without errors

#endif