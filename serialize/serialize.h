#ifndef SERIALIZE_H
#define SERIALIZE_H
#include "../error.h"
#include "../data_structure/array/gar.h"

/**
 *  Serialize: serialization of data
 */

void    serialize_skip_whitespace(char** str);
error_t serialize_start_array(char** json);
error_t serialize_stop_array(char** json);
error_t serialize_start_map(char** json);
error_t serialize_stop_map(char** json);
error_t serialize_next_item(char** json);

error_t int2string(char_gar_t*, int);
error_t string2int(int*, char**);

error_t char_ptr2string(char_gar_t*, char*);
error_t string2char_ptr(char**, char**);

#endif
