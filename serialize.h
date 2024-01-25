#ifndef SERIALIZE_H
#define SERIALIZE_H
#include "error.h"
#include "gar.h"

/**
 *  Serialize: serialization of data
 */

error_t int2string(char_gar_t*, int);
error_t string2int(int*, char**);

#endif
