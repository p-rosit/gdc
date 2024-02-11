#include <stdlib.h>
#include "gar_parse.h"

gar_make_parse(char, char, char_gar_free, , parse_char)
gar_make_parse(string, char*, string_gar_free, free, parse_string)

gar_make_parse(short, short, short_gar_free, , parse_short)
gar_make_parse(int, int, int_gar_free, , parse_int)
gar_make_parse(long, long, long_gar_free, , parse_long)
gar_make_parse(long_long, long long, long_long_gar_free, , parse_long_long)
gar_make_parse(ushort, unsigned short, ushort_gar_free, , parse_ushort)
gar_make_parse(uint, unsigned int, uint_gar_free, , parse_uint)
gar_make_parse(ulong, unsigned long, ulong_gar_free, , parse_ulong)
gar_make_parse(ulong_long, unsigned long long, ulong_long_gar_free, , parse_ulong_long)
