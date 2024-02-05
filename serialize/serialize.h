#ifndef SERIALIZE_H
#define SERIALIZE_H
#include "../error.h"
#include "../data_structure/array/gar.h"

/**
 *  Serialize: serialization of data
 */

error_t int2string(char_gar_t*, int);
error_t char_ptr2string(char_gar_t*, char*);

#endif
