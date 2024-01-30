#include <stdio.h>
#include <limits.h>
#include "serialize.h"

void serialize_skip_whitespace(char** str) {
    while (isspace(**str)) {(*str)++;}
}

error_t serialize_start_array(char** json) {
    if (**json == '[') {
        (*json)++; return NO_ERROR;
    } else {
        return PARSE_ERROR;
    }
}

error_t serialize_stop_array(char** json) {
    if (**json == ']') {
        (*json)++; return NO_ERROR;
    } else {
        return PARSE_ERROR;
    }
}

error_t serialize_start_map(char** json) {
    if (**json == '{') {
        (*json)++; return NO_ERROR;
    } else {
        return PARSE_ERROR;
    }
}

error_t serialize_stop_map(char** json) {
    if (**json == '}') {
        (*json)++; return NO_ERROR;
    } else {
        return PARSE_ERROR;
    }
}

error_t serialize_next_item(char** json) {
    if (**json == ',') {
        (*json)++; return NO_ERROR;
    } else {
        return PARSE_ERROR;
    }
}

error_t int2string(char_gar_t* json, int value) {
    /* TODO: make generic*/
    char num[11];
    sprintf(num, "%d", value);

    return char_gar_push_string(json, num);
}

error_t string2int(int* value, char** json) {
    int result = 0, digit_value, sign;
    char c, *str = *json;

    c = **json;
    if (!isdigit(c) && c != '+' && c != '-') {return PARSE_ERROR;}

    sign = c == '-' ? -1 : 1;
    str += c == '+' || c == '-';
    for (char c = *str++; c != '\0' && isdigit(c); c = *str++) {
        digit_value = sign * (c - '0');

        if (result > INT_MAX / 10 || result < INT_MIN / 10) {
            return PARSE_ERROR; /* Overflow in multiplication */
        }
        result *= 10;
        if (((sign > 0) && (result > INT_MAX - digit_value)) || ((sign < 0) && (result < INT_MIN - digit_value))) {
            return PARSE_ERROR; /* Overflow in addition */
        }

        result = result + digit_value;
    }
    
    *value = result;
    *json = str - 1;
    return NO_ERROR;
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

error_t string2char_ptr(char** value, char** json) {
    error_t error;
    char_gar_t dyn_str;
    char c, *str = *json;
    
    char_gar_new(&dyn_str);

    c = *str++;
    if (c != '"') {error = PARSE_ERROR; goto execution_failed;} /* String did not start correctly */

    for (c = *str++; c != '"' && c != '\0'; c = *str++) {
        error = char_gar_push(&dyn_str, c);
        if (error != NO_ERROR) {goto execution_failed;}
    }

    if (c != '"') {error = PARSE_ERROR; goto execution_failed;} /* String did not end correctly */

    *json = str;
    char_gar_make_string(&dyn_str, value);
    return error;

    execution_failed:
    char_gar_free(&dyn_str);
    return error;
}

