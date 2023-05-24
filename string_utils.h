#ifndef STRING_UTILS_H
#define STRING_UTILS_H

// STANDARD INCLUDES
#include <string.h>
#include <stdlib.h>

// METHODS

/*
    Right-pads the given string with pad_amount spaces
*/
char* right_pad_string(char* str, int pad_amount);

/*
    Left-pads the given string with pad_amount spaces
*/
char* left_pad_string(char* str, int pad_amount);

#endif