#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../macro.h"
#include "serialize.h"


#define JSON_PUSH_TOKEN(name, token) \
    error_t name(json_str_t* json) {                                            \
        return json_push(json, token);                                          \
    }

#define JSON_PUSH_DIVIDER(name, token) \
    error_t name(json_str_t* json) {                                            \
        error_t error = json_push(json, token);                                 \
        if (error != NO_ERROR) {return error;}                                  \
        return json_push(json, ' ');                                            \
    }

json_str_t json_new() {
    return (json_str_t) {
        .capacity = 0,
        .size = 0,
        .characters = NULL
    };
}

void json_free(json_str_t* json) {
    free(json->characters);
    *json = json_new();
}

error_t json_push(json_str_t* json, char c) {
    size_t capacity;
    char* characters;

    if (json->size >= json->capacity) {
        capacity = json->capacity;
        capacity += capacity / 2 + 2 * (capacity < 2);

        characters = malloc(capacity);
        if (characters == NULL) {return MEMORY_ERROR;}

        memcpy(characters, json->characters, json->size);
        free(json->characters);
        json->characters = characters;
        json->capacity = capacity;
    }

    json->characters[json->size++] = c;
    return NO_ERROR;
}

error_t json_push_str(json_str_t* json, char* str) {
    size_t capacity = json->capacity;
    size_t length = strlen(str);
    size_t new_size = json->size + length;
    char* characters;

    while (capacity < new_size) {
        capacity += capacity / 2 + 2 * (capacity < 2);
    }
    if (json->capacity != capacity) {
        characters = malloc(capacity);
        if (characters == NULL) {return MEMORY_ERROR;}

        memcpy(characters, json->characters, json->size);
        free(json->characters);
        json->characters = characters;
        json->capacity = capacity;
    }

    for (size_t i = 0; i < length; i++) {
        json->characters[json->size++] = str[i];
    }

    return NO_ERROR;
}

error_t json_to_str(json_str_t* json, char** str) {
    error_t error = json_push(json, '\0');
    if (error != NO_ERROR) {return error;}

    *str = json->characters;
    *json = json_new();
    return NO_ERROR;
}

JSON_PUSH_TOKEN(json_start_array,   '[')
JSON_PUSH_TOKEN(json_end_array,     ']')
JSON_PUSH_TOKEN(json_start_map,     '{')
JSON_PUSH_TOKEN(json_end_map,       '}')
JSON_PUSH_DIVIDER(json_next_entry,  ',')
JSON_PUSH_DIVIDER(json_map_divider, ':')

#define SERIALIZE_NUMBER(name, type) \
    error_t JOIN_TOKENS(serialize_, name)(json_str_t* json, type value) {       \
        error_t error;                                                          \
        size_t initial_size = json->size, size = 0;                             \
        char *str, c;                                                           \
        int sign = 1 - 2 * (value < 0);                                         \
                                                                                \
        while (value != 0) {                                                    \
            error = json_push(json, '0' + sign * (value % 10));                 \
            if (error != NO_ERROR) {return error;}                              \
            value /= 10;                                                        \
        }                                                                       \
                                                                                \
        if (sign < 0) {                                                         \
            error = json_push(json, '-');                                       \
            if (error != NO_ERROR) {return error;}                              \
        }                                                                       \
                                                                                \
        size = json->size - initial_size;                                       \
        str = json->characters + initial_size;                                  \
        for (int i = 0; i < size / 2; i++) {                                    \
            /* Reverse in-place */                                              \
            c = str[size - i - 1];                                              \
            str[size - i - 1] = str[i];                                         \
            str[i] = c;                                                         \
        }                                                                       \
                                                                                \
        return error;                                                           \
    }

SERIALIZE_NUMBER(short, short)
SERIALIZE_NUMBER(int, int)
SERIALIZE_NUMBER(long, long)
SERIALIZE_NUMBER(long_long, long long)
SERIALIZE_NUMBER(ushort, unsigned short)
SERIALIZE_NUMBER(uint, unsigned int)
SERIALIZE_NUMBER(ulong, unsigned long)
SERIALIZE_NUMBER(ulong_long, unsigned long long)

error_t serialize_string(json_str_t* json, char* value) {
    error_t error;

    error = json_push(json, '"');
    if (error != NO_ERROR) {return error;}

    error = json_push_str(json, value);
    if (error != NO_ERROR) {return error;}

    error = json_push(json, '"');
    return error;
}

error_t serialize_char(json_str_t* json, char value) {
    error_t error;

    error = json_push(json, '\'');
    if (error != NO_ERROR) {return error;}

    error = json_push(json, value);
    if (error != NO_ERROR) {return error;}

    error = json_push(json, '\'');
    return error;
}
