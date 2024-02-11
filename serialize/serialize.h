#ifndef SERIALIZE_H
#define SERIALIZE_H
#include <stdlib.h>
#include "../error.h"
#include "../macro.h"

/**
 *  Serialize: serialization of data
 */

#define SERIALIZE(name, type) \
    error_t JOIN_TOKENS(serialize_, name)(json_str_t* json, type value)

typedef struct json_str {
    size_t capacity;
    size_t size;
    char* characters;
} json_str_t;

json_str_t json_new();
void json_free(json_str_t*);
error_t json_push(json_str_t*, char);
error_t json_push_str(json_str_t*, char*);
error_t json_to_str(json_str_t*, char**);

error_t json_start_array(json_str_t*);
error_t json_end_array(json_str_t*);
error_t json_start_map(json_str_t*);
error_t json_end_map(json_str_t*);
error_t json_next_entry(json_str_t*);
error_t json_map_divider(json_str_t*);

SERIALIZE(short, short);
SERIALIZE(int, int);
SERIALIZE(long, long);
SERIALIZE(long_long, long long);

SERIALIZE(ushort, unsigned short);
SERIALIZE(uint, unsigned int);
SERIALIZE(ulong, unsigned long);
SERIALIZE(ulong_long, unsigned long long);

SERIALIZE(char, char);
SERIALIZE(string, char*);

#define SERIALIZE_DATA_H(name, type) \
    error_t JOIN_TOKENS(name, _to_json)(const type* data, char** json); \
    error_t JOIN_TOKENS(name, _to_json_recursive)(const type* data, json_str_t* json);

#define SERIALIZE_BASE(name, array_type) \
    error_t JOIN_TOKENS(name, _to_json)(const array_type* array, char** str) {  \
        error_t error;                                                          \
        json_str_t json;                                                        \
                                                                                \
        json = json_new();                                                      \
        error = JOIN_TOKENS(name, _to_json_recursive)(array, &json);            \
        if (error != NO_ERROR) {goto execution_failed;}                         \
                                                                                \
        error = json_to_str(&json, str);                                        \
        if (error != NO_ERROR) {goto execution_failed;}                         \
                                                                                \
        return error;                                                           \
                                                                                \
        execution_failed:                                                       \
        *str = NULL;                                                            \
        json_free(&json);                                                       \
        return error;                                                           \
    }

#define SERIALIZE_ARRAY(name, array_type, type, for_each, value_to_json) \
    SERIALIZE_BASE(name, array_type)                                            \
                                                                                \
    error_t JOIN_TOKENS(name, _to_json_recursive)(const array_type* array, json_str_t* json) { \
        size_t index;                                                           \
        error_t error;                                                          \
                                                                                \
        error = json_start_array(json);                                         \
        if (error != NO_ERROR) {goto execution_failed;}                         \
                                                                                \
        index = 0;                                                              \
        for_each(*array, type value) {                                          \
            if (index > 0) {                                                    \
                error = json_next_entry(json);                                  \
                if (error != NO_ERROR) {goto execution_failed;}                 \
            }                                                                   \
                                                                                \
            error = value_to_json(json, value);                                 \
            if (error != NO_ERROR) {goto execution_failed;}                     \
                                                                                \
            index++;                                                            \
        }                                                                       \
                                                                                \
        error = json_end_array(json);                                           \
        if (error != NO_ERROR) {goto execution_failed;}                         \
                                                                                \
        return error;                                                           \
                                                                                \
        execution_failed:                                                       \
        json_free(json);                                                        \
        return error;                                                           \
    }

#define SERIALIZE_MAP(name, map_type, key_type, value_type, for_each, key_to_json, value_to_json) \
    SERIALIZE_BASE(name, map_type)                                              \
                                                                                \
    error_t JOIN_TOKENS(name, _to_json_recursive)(const map_type* map, json_str_t* json) { \
        size_t index;                                                           \
        error_t error;                                                          \
                                                                                \
        error = json_start_array(json);                                         \
        if (error != NO_ERROR) {goto execution_failed;}                         \
                                                                                \
        index = 0;                                                              \
        for_each(*map, key_type key, value_type value) {                        \
            if (index > 0) {                                                    \
                error = json_next_entry(json);                                  \
                if (error != NO_ERROR) {goto execution_failed;}                 \
            }                                                                   \
                                                                                \
            error = json_start_array(json);                                     \
            if (error != NO_ERROR) {goto execution_failed;}                     \
                                                                                \
            error = key_to_json(json, key);                                     \
            if (error != NO_ERROR) {goto execution_failed;}                     \
                                                                                \
            error = json_next_entry(json);                                      \
            if (error != NO_ERROR) {goto execution_failed;}                     \
                                                                                \
            error = value_to_json(json, value);                                 \
            if (error != NO_ERROR) {goto execution_failed;}                     \
                                                                                \
            error = json_end_array(json);                                       \
            if (error != NO_ERROR) {goto execution_failed;}                     \
                                                                                \
            index++;                                                            \
        }                                                                       \
                                                                                \
        error = json_end_array(json);                                           \
        if (error != NO_ERROR) {goto execution_failed; }                        \
                                                                                \
        return error;                                                           \
                                                                                \
        execution_failed:                                                       \
        json_free(json);                                                        \
        return error;                                                           \
    }

#endif
