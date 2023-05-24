#include "string_utils.h"

/*
    Right-pads the given string with pad_amount spaces
*/
char* right_pad_string(char* str, int pad_amount){

    int str_len = strlen(str);
    int byte_count = str_len + pad_amount + 1;      // the final \0 must be included

    char* padded_str = (char*) malloc(byte_count);

    //strcpy stops copying after reading the first \0
    strcpy(padded_str, str);

    for(int i = str_len; i < byte_count-1; i++){
        *(padded_str + i) = ' ';
    }

    *(padded_str + byte_count - 1) = '\0';
    return padded_str;
}

/*
    Left-pads the given string with pad_amount spaces
*/
char* left_pad_string(char* str, int pad_amount){
    int str_len = strlen(str);
    int byte_count = str_len + pad_amount + 1;      // the final \0 must be included

    char* padded_str = (char*) malloc(byte_count);

    for(int i = 0; i < pad_amount; i++){
        *(padded_str + i) = ' ';
    }

    strcpy((padded_str + pad_amount), str);
    return padded_str;
}
