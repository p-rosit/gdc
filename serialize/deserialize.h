#ifndef DESERIALIZE_H
#define DESERIALIZE_H
#include <stdint.h>
#include "../error.h"
#include "../macro.h"

#define PARSE_ARRAY_H(name, type) error_t JOIN_TOKENS(name, _from_json)(type*)
#define PARSE_MAP_H(name, type) error_t JOIN_TOKENS(name, _from_json)(type*)

#define PARSE_ARRAY(name, type, new, push, parse_value) \
    error_t JOIN_TOKENS(name, _from_json)(type* array) { \
        /* TODO */\
    }

#define PARSE_MAP(name, type, new, insert, parse_value) \
    error_t JOIN_TOKENS(name, _from_json)(type* map) { \
        /* TODO */\
    }

#define DESERIALIZE(name) error_t JOIN_TOKENS(parse_, name)
#define PROT_DESERIALIZE(name, type) DESERIALIZE(name)(type*, char**)

void    parse_skip_whitespace(char** json);
error_t parse_start_array(char** json);
error_t parse_stop_array(char** json);
error_t parse_start_map(char** json);
error_t parse_stop_map(char** json);
error_t parse_next_entry(char** json);
error_t parse_key_value_divider(char** json);

PROT_DESERIALIZE(short, short);
PROT_DESERIALIZE(int, int);
PROT_DESERIALIZE(long, long);
PROT_DESERIALIZE(long_long, long long);
PROT_DESERIALIZE(ushort, unsigned short);
PROT_DESERIALIZE(uint, unsigned int);
PROT_DESERIALIZE(ulong, unsigned long);
PROT_DESERIALIZE(ulong_long, unsigned long long);

PROT_DESERIALIZE(i8, int8_t);
PROT_DESERIALIZE(i16, int16_t);
PROT_DESERIALIZE(i32, int32_t);
PROT_DESERIALIZE(i64, int64_t);
PROT_DESERIALIZE(u8, uint8_t);
PROT_DESERIALIZE(u16, uint16_t);
PROT_DESERIALIZE(u32, uint32_t);
PROT_DESERIALIZE(u64, uint64_t);

PROT_DESERIALIZE(f32, float);
PROT_DESERIALIZE(f64, double);
PROT_DESERIALIZE(f128, long double);

PROT_DESERIALIZE(char, char);
PROT_DESERIALIZE(uchar, unsigned char);
PROT_DESERIALIZE(schar, signed char);
PROT_DESERIALIZE(string, char*);

#define EXPECT_TOKEN(name, token) \
    error_t name(char** json) {                                                 \
        if (**json == token) {                                                  \
            (*json)++; return NO_ERROR;                                         \
        } else {                                                                \
            return PARSE_ERROR;                                                 \
        }                                                                       \
    }

#define PARSE_UNSIGNED_NUM(name, type, upper_limit) \
    DESERIALIZE(name)(type* value, char** json) {                               \
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
    DESERIALIZE(name)(type* value, char** json) {                               \
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
