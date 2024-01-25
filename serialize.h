#ifndef SERIALIZE_H
#define SERIALIZE_H
#include "gdc.h"
#include "gar.h"

/**
 *  Serialize: serialization of data
 */

gdc_error_t int2string(char_gar_t*, int);
gdc_error_t string2int(int*, char**);

#endif
