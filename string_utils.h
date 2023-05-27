#ifndef STRING_UTILS_H
#define STRING_UTILS_H

// STANDARD INCLUDES
#include <string.h>
#include <stdlib.h>

// METHODS

/*
    Returns a copy of the prefix string concatenated with the suffix string.
    The two given strings are left unchanged.

    Returns NULL if heap allocation has failed.
*/
char* safe_strcat(char* prefix, char* suffix);

/*
    Returns a right-padded copy of the string.
    The given string is left unchanged.

    Returns NULL if allocation has failed or
    if the given pad_amount is negative
*/
char* right_pad_string(char* str, int pad_amount);

/*
    Returns a left-padded copy of the string.
    The given string is left unchanged.

    Returns NULL if allocation has failed or
    if the given pad_amount is negative
*/
char* left_pad_string(char* str, int pad_amount);

#endif