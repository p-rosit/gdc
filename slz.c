#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include "slz.h"


gdc_error_t int2string(char_gar_t* json, int value) {
    /* TODO: make robust*/
    char num[10];
    sprintf(num, "%d", value);

    return char_gar_push_string(json, num);
}

gdc_error_t string2int(int* value, char** json) {
    /* TODO: handle more cases */
    int result = 0;
    char* str = *json;

    if (!isdigit(**json)) {return GDC_PARSE_ERROR;}

    for (char c = *str++; c != '\0' && isdigit(c); c = *str++) {
        result = 10 * result + (c - '0');
    }
    
    *value = result;
    *json = str - 1;
    return GDC_OK;
}
