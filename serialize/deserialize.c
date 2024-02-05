#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include "../error.h"
#include "deserialize.h"

void parse_skip_whitespace(char** str) {
    while (isspace(**str)) {(*str)++;}
}

EXPECT_TOKEN(parse_start_array, '[')
EXPECT_TOKEN(parse_stop_array, ']')
EXPECT_TOKEN(parse_start_map, '{')
EXPECT_TOKEN(parse_stop_map, '}')
EXPECT_TOKEN(parse_next_entry, ',')
EXPECT_TOKEN(parse_key_value_divider, ':')

PARSE_SIGNED_NUM(short, short, SHRT_MIN, SHRT_MAX)
PARSE_SIGNED_NUM(int, int, INT_MIN, INT_MAX)
PARSE_SIGNED_NUM(long, long, LONG_MIN, LONG_MAX)
PARSE_SIGNED_NUM(long_long, long long, LLONG_MIN, LLONG_MAX)
PARSE_UNSIGNED_NUM(ushort, unsigned short, USHRT_MAX)
PARSE_UNSIGNED_NUM(uint, unsigned int, UINT_MAX)
PARSE_UNSIGNED_NUM(ulong, unsigned long, ULONG_MAX)
PARSE_UNSIGNED_NUM(ulong_long, unsigned long long, ULLONG_MAX)

PARSE_SIGNED_NUM(i8, int8_t, INT8_MIN, INT8_MAX)
PARSE_SIGNED_NUM(i16, int16_t, INT16_MIN, INT16_MAX)
PARSE_SIGNED_NUM(i32, int32_t, INT32_MIN, INT32_MAX)
PARSE_SIGNED_NUM(i64, int64_t, INT64_MIN, INT64_MAX)
PARSE_UNSIGNED_NUM(u8, uint8_t, UINT8_MAX)
PARSE_UNSIGNED_NUM(u16, uint16_t, UINT16_MAX)
PARSE_UNSIGNED_NUM(u32, uint32_t, UINT32_MAX)
PARSE_UNSIGNED_NUM(u64, uint64_t, UINT64_MAX)


error_t validate_json_string(char* json, size_t* length) {
    size_t visible_length = 0;
    char c;

    c = *json++;
    if (c != '"') {return PARSE_ERROR;} /* String did not start correctly */

    for (c = *json++; c != '\0'; c = *json++) {
        if (c == '\\') {
            c = *json++;
            switch (c) {
                case '\"':
                case '\\':
                case '/':
                case 'b':
                case 'f':
                case 'n':
                case 'r':
                case 't':
                    visible_length++;
                    break;
                case 'u':
                    visible_length += 2;
                    for (int hex_length = 0; hex_length < 4; hex_length++) {
                        c = *json++;
                        if (!isxdigit(c)) {return PARSE_ERROR;}
                    }
                    break;
                default:
                    return PARSE_ERROR;
            }
        } else if (c == '\"') {
            break;
        } else {
            visible_length++;
        }
    }
    if (c != '"') {return PARSE_ERROR;} /* String did not end correctly */

    *length = visible_length;
    return NO_ERROR;
}

char* parse_json_string(char* result, char* json) {
    size_t size = 0;
    char c;

    for (c = *json++; c != '\0'; c = *json++) {
        if (c == '\\') {
            c = *json++;
            switch (c) {
                case '\"':
                case '\\':
                case '/':
                    result[size++] = c;
                    break;
                case 'b':
                    result[size++] = '\b';
                    break;
                case 'f':
                    result[size++] = '\f';
                    break;
                case 'n':
                    result[size++] = '\n';
                    break;
                case 'r':
                    result[size++] = '\r';
                    break;
                case 't':
                    result[size++] = '\t';
                    break;
                case 'u':
                    /* TODO: fix? */
                    for (int i = 0; i < 4; i++) {c = *json++;}
                    result[size++] = 'A';
                    result[size++] = 'A';
                    break;
            }
        } else if (c == '\"') {
            break;
        } else {
            result[size++] = c;
        }
    }

    result[size] = '\0';
    return json;
}

error_t parse_string(char** value, char** json) {
    error_t error;
    size_t length;
    char* result;

    error = validate_json_string(*json, &length);
    if (error != NO_ERROR) {return error;}

    result = malloc(length + 1);
    if (result == NULL) {return MEMORY_ERROR;}

    *json = parse_json_string(result, *json + 1);

    *value = result;
    return NO_ERROR;
}

