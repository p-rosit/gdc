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

SERIALIZE(string, char*);

#endif
