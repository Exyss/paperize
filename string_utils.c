#include "string_utils.h"
#include <stdio.h>

/*
    Returns a copy of the prefix string concatenated with the suffix string.
    The two given strings are left unchanged.

    Returns NULL if heap allocation has failed.
*/
char* safe_strcat(char* prefix, char* suffix){
    
    // +1 for null byte
    char* new_string = (char*) malloc(strlen(prefix) + strlen(suffix) + 1);

    //if malloc has failed
    if(new_string != NULL){
        //copy prefix on the new string and concatenate with suffix
        strcpy(new_string, prefix);
        strcat(new_string, suffix);
    }

    return new_string;
}

/*
    Returns a right-padded copy of the string.
    The given string is left unchanged.

    Returns NULL if allocation has failed or
    if the given pad_amount is negative
*/
char* right_pad_string(char* str, int pad_amount){

    if(pad_amount < 0){
        return NULL;
    }

    char spaces[pad_amount + 1];

    for(int i = 0; i < pad_amount; i++){
        spaces[i] = ' ';
    }
    spaces[pad_amount] = '\0';

    return safe_strcat(str, spaces);
}

/*
    Returns a left-padded copy of the string.
    The given string is left unchanged.

    Returns NULL if allocation has failed or
    if the given pad_amount is negative
*/
char* left_pad_string(char* str, int pad_amount){

    if(pad_amount < 0){
        return NULL;
    }

    char spaces[pad_amount + 1];

    for(int i = 0; i < pad_amount; i++){
        spaces[i] = ' ';
    }
    spaces[pad_amount] = '\0';

    return safe_strcat(spaces, str);
}