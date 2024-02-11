#include "gar_serialize.h"

gar_make_serialize(char, char, serialize_char)
gar_make_serialize(string, char*, serialize_string)

gar_make_serialize(short, short, serialize_short)
gar_make_serialize(int, int, serialize_int)
gar_make_serialize(long, long, serialize_long)
gar_make_serialize(long_long, long long, serialize_long_long)
gar_make_serialize(ushort, unsigned short, serialize_ushort)
gar_make_serialize(uint, unsigned int, serialize_uint)
gar_make_serialize(ulong, unsigned long, serialize_ulong)
gar_make_serialize(ulong_long, unsigned long long, serialize_ulong_long)
