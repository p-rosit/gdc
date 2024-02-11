#ifndef GAR_PARSE_H
#define GAR_PARSE_H
#include "gar.h"
#include "../../../serialize/parse.h"

#define gar_make_parse_h(name, type) \
    PARSE_DATA_H(JOIN_TOKENS(name, _gar), GAR(name))

#define gar_make_parse(name, type, json_to_value) \
    PARSE_ARRAY(JOIN_TOKENS(name, _gar), GAR(name), type, GAR_FUNC(name, new), GAR_FUNC(name, push), GAR_FUNC(name, free), json_to_value)
 
gar_make_parse_h(char, char)
gar_make_parse_h(string, char*)

gar_make_parse_h(short, short)
gar_make_parse_h(int, int)
gar_make_parse_h(long, long)
gar_make_parse_h(long_long, long long)
gar_make_parse_h(ushort, unsigned short)
gar_make_parse_h(uint, unsigned int)
gar_make_parse_h(ulong, unsigned long)
gar_make_parse_h(ulong_long, unsigned long long)

gar_make_parse_h(f32, float)
gar_make_parse_h(f64, double)
gar_make_parse_h(f128, long double)

#endif
