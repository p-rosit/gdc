#include <stdio.h>
#include "serialize.h"


error_t int2string(char_gar_t* json, int value) {
    /* TODO: make generic*/
    char num[11];
    sprintf(num, "%d", value);

    return char_gar_push_string(json, num);
}

error_t char_ptr2string(char_gar_t* json, char* value) {
    error_t error;

    error = char_gar_push(json, '"');
    if (error != NO_ERROR) {return error;}

    error = char_gar_push_string(json, value);
    if (error != NO_ERROR) {return error;}

    error = char_gar_push(json, '"');
    if (error != NO_ERROR) {return error;}

    return NO_ERROR;
}

