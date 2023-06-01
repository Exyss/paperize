#include "reader.h"

/*
    Analyzes next_byte in order to calculate
    how many more bytes make up the current
    character.
    Returns 0 if it's an ASCII char,
    1, 2 or 3 if it's an UTF-8 char
*/
static short get_bytes_to_read(int next_byte){

    /*
        The first 2 MS bits of the byte are used to 
        determinate if it's an ASCII char or if
        it's part of UTF-8 char
        =>  if the bits are set to 0xxxxxxx,
            it's an ASCII char
        =>  if the bits are set to 11xxxxxx,
            it's the first byte of an UTF-8 char
        =>  if the bits are set to 10xxxxxx,
            it's the next byte of an UTF-8 char
    */
   
    short bytes_to_read = 0;
    
    //if it's the first byte of an UTF-8 char
    if((next_byte >> 6) == 0b11 ){

        //the first 5 MS bits of the char are used 
        //to determinate how many bytes form the UTF-8 char
        switch(next_byte & 0b11110000){
            
            case 0b11000000:
                bytes_to_read = 1;
                break;

            case 0b11100000:
                bytes_to_read = 2;
                break;

            case 0b11110000:
                bytes_to_read = 3;
                break;

            case 0b11111000:
                bytes_to_read = 4;
                break;

            default:
                break;
        }
    }

    return bytes_to_read;
}

/*
    Creates and fills a Word with the given data.
    The given max-size stack buffer gets converted
    to a perfect-sized heap buffer.
*/
static Word* pack_word(char* buffer, int byte_count, int char_count, bool REACHED_EOL, bool REACHED_EOF){
    
    //signal word end for strcpy (see below)
    buffer[byte_count] = '\0';
    byte_count++;

    /*
        strcpy stops copying after reading the first \0
        => we get a perfect sized string in the heap
            while the bloated string in stack gets reused
    */
    char* str = (char*) malloc(byte_count);
    strcpy(str, buffer);

    Word* word = init_word(str, char_count, REACHED_EOL, REACHED_EOF);
    return word;
}

/*
    Reads the file byte-by-byte until a full
    Word has been read or MAX_CHARS have been read

    Sets status to FILE_READ_SUCCESS or READ_FAIL.
*/
Word* read_word_from_file(FILE* fin, int MAX_CHARS, short* status){
    
    /*
    worst case: all row chars are UTF-8 (max 4 bytes per UTF-8 char), +1 for \0
        => using a "big" buffer is preferable to continuously using malloc()
            in order to append characters since it would impact on performance
            due to too much malloc() and free() calls (also, the stack gets
            cleared once a word has been read, meaning the same bytes would be reused)
    */
    
    const int MAX_BUFFER = MAX_CHARS * 4 + 1;

    char buffer[MAX_BUFFER];

    //DO NOT use char type for next_byte (conflict with UTF-8 chars)
    short next_byte;

    int char_count = 0;
    int byte_count = 0;
    short bytes_to_read = 0;

    bool REACHED_EOL = false;
    bool REACHED_EOF = false;

    //read byte-by-byte (slower but simpler to manage)
    while((next_byte = fgetc(fin)) != EOF){
        
        //stop reading on newline
        if(next_byte == '\n'){
            REACHED_EOL = true;
            break;
        }

        //stop reading on spaces
        if(next_byte == '\t' || next_byte == ' '){
            
            //ignore if word is empty
            if(char_count == 0){
                continue;
            }
            break;
        }

        //ignore carriage return
        if(next_byte == '\r'){
            continue;
        }

        bytes_to_read += get_bytes_to_read(next_byte);

        //append char
        buffer[byte_count] = next_byte;
        byte_count++;
        
        //if bytes_to_read == 0, count char
        if(bytes_to_read > 0){
            bytes_to_read--;
        }
        else{
            char_count++;

            //this control can't be inserted in AND with the while condition
            //since the next byte would get read
            if(char_count >= MAX_CHARS){
                break;
            }
        }
    }

    /*
        EOF is returned when end of file has been reached
        or when an error occurred
        => must check if an error occurred
    */
    if(ferror(fin) != 0){
        *status = FILE_READ_FAIL;
        return NULL;
    }

    //check if EOF has been reached
    REACHED_EOF = (next_byte == EOF);

    Word* word = pack_word(buffer, byte_count, char_count, REACHED_EOL, REACHED_EOF);
    *status = FILE_READ_SUCCESS;
    return word;
}

Word* read_word_from_pipe(int fd_in, short* status){

    Word* word;

    int str_len;
    char* str;
    int char_count;
    bool REACHED_EOL;
    bool REACHED_EOF;
    
    word = NULL;

    if(read(fd_in, &str_len, sizeof(int)) == -1){
        // return -1;
    };

    str = (char*) malloc(str_len);

    if(read(fd_in, str, str_len) == -1 ||
       read(fd_in, &char_count, sizeof(int)) == -1 ||
       read(fd_in, &REACHED_EOL, sizeof(bool)) == -1 ||
       read(fd_in, &REACHED_EOF, sizeof(bool)) == -1){
        
        //read() returns -1 if an error occurred
        // *status = PIPE_READ_FAIL;
    }
    else{
        word = init_word(str, char_count, REACHED_EOL, REACHED_EOF);
        // *status = PIPE_READ_SUCCESS;
    }

    return word;
}

char* read_string_from_pipe(int fd_in, short* status){

    int str_len;
    char* str;
    bool close_pipe;
    
    str = NULL;

    if(read(fd_in, &str_len, sizeof(int)) == -1){
        // return -1;
    };

    str = (char*) malloc(str_len);

    if(read(fd_in, str, str_len) == -1 ||
       read(fd_in, &close_pipe, sizeof(bool)) == -1){
        
        //read() returns -1 if an error occurred
        *status = PIPE_READ_FAIL;
    }
    else{
        if(close_pipe){
            *status = PIPE_READ_TERMINATED;
        }
        else{
            *status = PIPE_READ_SUCCESS;
        }
    }

    return str;
}