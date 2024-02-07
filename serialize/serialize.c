#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../macro.h"
#include "serialize.h"

#define SERIALIZE_DATA_H(name, type) \
    error_t JOIN_TOKENS(name, _to_json)(const type* data, char** json); \
    error_t JOIN_TOKENS(name, _to_json_recursive)(const type* data, char** json);

#define SERIALIZE_BASE(name, array_type) \
    error_t JOIN_TOKENS(name, _to_json)(const array_type* array, char** str) { \
        error_t error; \
        json_str_t json; \
        \
        json = json_new(); \
        error = JOIN_TOKENS(name, _to_json_recursive)(array, &json); \
        if (error != NO_ERROR) {goto exeuction_failed;} \
        \
        error = json_to_str(&json, *str); \
        if (error != NO_ERROR) {goto execution_failed;} \
        \
        return error; \
        \
        execution_failed: \
        *str = NULL; \
        json_free(json); \
        return error; \
    }

#define SERIALIZE_ARRAY(name, array_type, type, for_each, value_to_json) \
    SERIALIZE_BASE(name, array_type) \
    \
    error_t JOIN_TOKENS(name, _to_json_recursive)(const array_type* array, json_str_t* json) { \
        size_t index; \
        error_t error; \
        \
        error = json_start_array(json); \
        if (error != NO_ERROR) {goto execution_failed; } \
        \
        index = 0;\
        for_each(*array, type value) { \
            if (index > 0) { \
                error = json_next_entry(json); \
                if (error != NO_ERROR) {goto execution_failed;} \
            } \
            \
            error = value_to_json(json, value); \
            if (error != NO_ERROR) {goto execution_failed;}\
            \
            index++;\
        } \
        \
        error = json_end_array(json); \
        if (error != NO_ERROR) {goto execution_failed; } \
        \
        return error; \
        \
        execution_failed:\
        json_free(json); \
        return error;\
    }

#define SERIALIZE_MAP(name, map_type, key_type, value_type, for_each, key_to_json, value_to_json) \
    SERIALIZE_BASE(name, array_type) \
    \
    error_t JOIN_TOKENS(name, _to_json_recursive)(const map_type* map, char** json) { \
        size_t index; \
        error_t error; \
        \
        error = json_start_array(json); \
        if (error != NO_ERROR) {goto execution_failed;} \
        \
        index = 0;\
        for_each(*map, key_type key, value_type value) { \
            if (index > 0) { \
                error = json_next_entry(json); \
                if (error != NO_ERROR) {goto execution_failed;} \
            } \
            \
            error = json_start_array(json); \
            if (error != NO_ERROR) {goto execution_failed;} \
            \
            error = key_to_json(json, key);\
            if (error != NO_ERROR) {goto execution_failed;} \
            \
            error = json_next_entry(json); \
            if (error != NO_ERROR) {goto execution_failed;} \
            \
            error = value_to_json(json, value); \
            if (error != NO_ERROR) {goto execution_failed;}\
            \
            error = json_end_array(json); \
            if (error != NO_ERROR) {goto execution_failed;} \
            \
            index++;\
        } \
        \
        error = json_end_array(json); \
        if (error != NO_ERROR) {goto execution_failed; } \
        \
        return error; \
        \
        execution_failed:\
        json_free(json); \
        return error;\
    }

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

#define APPROX_LOG10(name, type) \
    size_t JOIN_TOKENS(name, _approx_log10)(type num) {                         \
        size_t log = 0;                                                         \
                                                                                \
        num = (num < 0) ? 1 - num : num;                                        \
        for (size_t i = 0; num > 0; num /= 2, i++) {                            \
            log = (num % 2) ? i : log;                                          \
        }                                                                       \
                                                                                \
        /* return approx_log2(num) * log10(2) */                                \
        return log * 0.301;                                                     \
    }

#define SERIALIZE_NUMBER(name, type, print_option) \
    APPROX_LOG10(name, type)                                                    \
                                                                                \
    error_t JOIN_TOKENS(serialize_, name)(json_str_t* json, type value) {       \
        char num[JOIN_TOKENS(name, _approx_log10)(value) + 5];                  \
        sprintf(num, print_option, value);                                      \
        return json_push_str(json, num);                                        \
    }

SERIALIZE_NUMBER(short, short, "%d")
SERIALIZE_NUMBER(int, int, "%d")

error_t char_ptr2string(json_str_t* json, char* value) {
    error_t error;

    error = json_push(json, '"');
    if (error != NO_ERROR) {return error;}

    error = json_push_str(json, value);
    if (error != NO_ERROR) {return error;}

    error = json_push(json, '"');
    if (error != NO_ERROR) {return error;}

    return NO_ERROR;
}

