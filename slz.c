#include <stdio.h>
#include <limits.h>
#include "slz.h"


gdc_error_t int2string(char_gar_t* json, int value) {
    /* TODO: make generic*/
    char num[11];
    sprintf(num, "%d", value);

    return char_gar_push_string(json, num);
}

gdc_error_t string2int(int* value, char** json) {
    int result = 0, digit_value, sign;
    char c, *str = *json;

    c = **json;
    if (!isdigit(c) && c != '+' && c != '-') {return GDC_PARSE_ERROR;}

    sign = c == '-' ? -1 : 1;
    str += c == '+' || c == '-';
    for (char c = *str++; c != '\0' && isdigit(c); c = *str++) {
        digit_value = sign * (c - '0');

        if (result > INT_MAX / 10 || result < INT_MIN / 10) {
            return GDC_PARSE_ERROR; /* Overflow in multiplication */
        }
        result *= 10;
        if (((sign > 0) && (result > INT_MAX - digit_value)) || ((sign < 0) && (result < INT_MIN - digit_value))) {
            return GDC_PARSE_ERROR; /* Overflow in addition */
        }

        result = result + digit_value;
    }
    
    *value = result;
    *json = str - 1;
    return GDC_OK;
}
