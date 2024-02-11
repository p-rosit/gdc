#ifndef GAR_SERIALIZE_H
#define GAR_SERIALIZE_H
#include "gar.h"
#include "../../../serialize/serialize.h"

#define gar_make_serialize_h(name, type) \
    SERIALIZE_DATA_H(JOIN_TOKENS(name, _gar), GAR(name))

#define gar_make_serialize(name, type, value_to_json) \
    SERIALIZE_ARRAY(JOIN_TOKENS(name, _gar), GAR(name), type, for_each_gar, value_to_json)

gar_make_serialize_h(char, char)
gar_make_serialize_h(string, char*)

gar_make_serialize_h(short, short)
gar_make_serialize_h(int, int)
gar_make_serialize_h(long, long)
gar_make_serialize_h(long_long, long long)
gar_make_serialize_h(ushort, unsigned short)
gar_make_serialize_h(uint, unsigned int)
gar_make_serialize_h(ulong, unsigned long)
gar_make_serialize_h(ulong_long, unsigned long long)

gar_make_serialize_h(f32, float)
gar_make_serialize_h(f64, double)
gar_make_serialize_h(f128, long double)

#endif
