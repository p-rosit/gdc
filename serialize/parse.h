#ifndef DESERIALIZE_H
#define DESERIALIZE_H
#include <stdint.h>
#include "../error.h"
#include "../macro.h"

#define PARSE_DATA_H(name, type) \
    error_t JOIN_TOKENS(name, _from_json)(type*, char** json);

#define PARSE_ARRAY(name, array_type, type, new_array, push_value, free_array, free_value, parse_value) \
    error_t JOIN_TOKENS(name, _from_json)(array_type* array, char** str) {      \
        error_t error, stop_error;                                              \
        type value;                                                             \
        char* json = *str;                                                      \
                                                                                \
        new_array(array);                                                       \
                                                                                \
        parse_skip_whitespace(&json);                                           \
        error = parse_start_array(&json);                                       \
        if (error != NO_ERROR) {goto execution_failed;}                         \
                                                                                \
        parse_skip_whitespace(&json);                                           \
        stop_error = parse_end_array(&json);                                    \
                                                                                \
        while (stop_error != NO_ERROR) {                                        \
            parse_skip_whitespace(&json);                                       \
            error = parse_value(&value, &json);                                 \
            if (error != NO_ERROR) {goto execution_failed;}                     \
                                                                                \
            error = push_value(array, value);                                   \
            if (error != NO_ERROR) {goto parsing_failed;}                     \
                                                                                \
            parse_skip_whitespace(&json);                                       \
                                                                                \
            error = parse_next_entry(&json);                                    \
            if (error == NO_ERROR) {continue;}                                  \
                                                                                \
            stop_error = parse_end_array(&json);                                \
            if (stop_error == NO_ERROR) {                                       \
                break;                                                          \
            } else {                                                            \
                goto execution_failed;                                          \
            }                                                                   \
            \
            parsing_failed:\
            free_value(value); \
            goto execution_failed; \
        }                                                                       \
                                                                                \
        *str = json;                                                            \
        return NO_ERROR;                                                        \
                                                                                \
        execution_failed:                                                       \
        free_array(array);                                                      \
        return error;                                                           \
    }

#define PARSE_MAP(name, map_type, key_type, value_type, new_map, push_kvp, free_map, free_key, free_value, parse_key, parse_value) \
    error_t JOIN_TOKENS(name, _from_json)(map_type* map, char** str) {          \
        error_t error, stop_error;                                              \
        key_type key; \
        value_type value;                                                       \
        char* json = *str;                                                      \
                                                                                \
        new_map(map);                                                           \
                                                                                \
        parse_skip_whitespace(&json);                                           \
        error = parse_start_array(&json);                                       \
        if (error != NO_ERROR) {goto execution_failed;}                         \
                                                                                \
        parse_skip_whitespace(&json);                                           \
        stop_error = parse_end_array(&json);                                    \
                                                                                \
        while (stop_error != NO_ERROR) {                                        \
            parse_skip_whitespace(&json);                                       \
            error = parse_start_array(&json);                                   \
            if (error != NO_ERROR) {goto execution_failed;}                      \
            \
            parse_skip_whitespace(&json);                                       \
            error = parse_key(&key, &json);                                 \
            if (error != NO_ERROR) {goto execution_failed;}                     \
            \
            parse_skip_whitespace(&json);                                       \
            error = parse_next_entry(&json); \
            if (error != NO_ERROR) {goto parse_value_failed;}                     \
            \
            parse_skip_whitespace(&json);                                       \
            error = parse_value(&value, &json); \
            if (error != NO_ERROR) {goto parse_value_failed;}                     \
            \
            parse_skip_whitespace(&json);                                       \
            error = parse_end_array(&json); \
            if (error != NO_ERROR) {goto parse_failed;}                     \
            \
                                                                                \
            error = push_kvp(map, key, value);                                    \
            if (error != NO_ERROR) {goto parse_failed;}                     \
                                                                                \
            parse_skip_whitespace(&json);                                       \
                                                                                \
            error = parse_next_entry(&json);                                    \
            if (error == NO_ERROR) {continue;}                                  \
                                                                                \
            stop_error = parse_end_array(&json);                                \
            if (stop_error == NO_ERROR) {                                       \
                break;                                                          \
            } else {                                                            \
                goto execution_failed;                                          \
            }                                                                   \
            \
            parse_failed: \
            free_value(value); \
            parse_value_failed: \
            free_key(key); \
            goto execution_failed;\
        }                                                                       \
                                                                                \
        *str = json;                                                            \
        return NO_ERROR;                                                        \
                                                                                \
        execution_failed:                                                       \
        free_map(map);                                                      \
        return error;                                                           \
    }

#define DESERIALIZE(name, type) error_t JOIN_TOKENS(parse_, name)(type* value, char** json)

void    parse_skip_whitespace(char** json);
error_t parse_start_array(char** json);
error_t parse_end_array(char** json);
error_t parse_start_map(char** json);
error_t parse_end_map(char** json);
error_t parse_next_entry(char** json);
error_t parse_key_value_divider(char** json);

DESERIALIZE(short, short);
DESERIALIZE(int, int);
DESERIALIZE(long, long);
DESERIALIZE(long_long, long long);
DESERIALIZE(ushort, unsigned short);
DESERIALIZE(uint, unsigned int);
DESERIALIZE(ulong, unsigned long);
DESERIALIZE(ulong_long, unsigned long long);

DESERIALIZE(i8, int8_t);
DESERIALIZE(i16, int16_t);
DESERIALIZE(i32, int32_t);
DESERIALIZE(i64, int64_t);
DESERIALIZE(u8, uint8_t);
DESERIALIZE(u16, uint16_t);
DESERIALIZE(u32, uint32_t);
DESERIALIZE(u64, uint64_t);

DESERIALIZE(f32, float);
DESERIALIZE(f64, double);
DESERIALIZE(f128, long double);

DESERIALIZE(char, char);
DESERIALIZE(uchar, unsigned char);
DESERIALIZE(schar, signed char);
DESERIALIZE(string, char*);

#define EXPECT_TOKEN(name, token) \
    error_t name(char** json) {                                                 \
        if (**json == token) {                                                  \
            (*json)++; return NO_ERROR;                                         \
        } else {                                                                \
            return PARSE_ERROR;                                                 \
        }                                                                       \
    }

#define PARSE_UNSIGNED_NUM(name, type, upper_limit) \
    DESERIALIZE(name, type) {                                                   \
        type result = 0;                                                        \
        type digit_value;                                                       \
        char c, *str = *json;                                                   \
                                                                                \
        c = **json;                                                             \
        if (!isdigit(c) && c != '+') {return PARSE_ERROR;}                      \
                                                                                \
        str += c == '+';                                                        \
        for (char c = *str++; c != '\0' && isdigit(c); c = *str++) {            \
            digit_value = c - '0';                                              \
                                                                                \
            if (result > upper_limit / 10) {                                    \
                return PARSE_ERROR; /* Overflow in multiplication */            \
            }                                                                   \
            result *= 10;                                                       \
            if (result > upper_limit - digit_value) {                           \
                return PARSE_ERROR; /* Overflow in addition */                  \
            }                                                                   \
                                                                                \
            result = result + digit_value;                                      \
        }                                                                       \
                                                                                \
        *value = result;                                                        \
        *json = str - 1;                                                        \
        return NO_ERROR;                                                        \
    }

#define PARSE_SIGNED_NUM(name, type, lower_limit, upper_limit) \
    DESERIALIZE(name, type) {                                                   \
        int overflow, underflow;                                                \
        type result = 0;                                                        \
        type digit_value;                                                       \
        type sign;                                                              \
        char c, *str = *json;                                                   \
                                                                                \
        c = **json;                                                             \
        if (!isdigit(c) && c != '+' && c != '-') {return PARSE_ERROR;}          \
                                                                                \
        sign = c == '-' ? -1 : 1;                                               \
        str += c == '+' || c == '-';                                            \
        for (char c = *str++; c != '\0' && isdigit(c); c = *str++) {            \
            digit_value = sign * (c - '0');                                     \
                                                                                \
            if (result > upper_limit / 10 || result < lower_limit / 10) {       \
                return PARSE_ERROR; /* Overflow in multiplication */            \
            }                                                                   \
            result *= 10;                                                       \
            overflow = (sign > 0) && (result > upper_limit - digit_value);      \
            underflow = (sign < 0) && (result < lower_limit - digit_value);     \
            if (overflow || underflow) {                                        \
                return PARSE_ERROR; /* Overflow in addition */                  \
            }                                                                   \
                                                                                \
            result = result + digit_value; \
        } \
        \
        *value = result; \
        *json = str - 1; \
        return NO_ERROR;\
    }

#endif
