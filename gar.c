#include <stdio.h>
#include "gar.h"

gar_make_basic(short, short)
gar_make_basic(int, int)
gar_make_basic(long, long)
gar_make_basic(long_long, long long)
gar_make_basic(ushort, unsigned short)
gar_make_basic(uint, unsigned int)
gar_make_basic(ulong, unsigned long)
gar_make_basic(ulong_long, unsigned long long)

gar_make_basic(i8, int8_t)
gar_make_basic(i16, int16_t)
gar_make_basic(i32, int32_t)
gar_make_basic(i64, int64_t)
gar_make_basic(u8, uint8_t)
gar_make_basic(u16, uint16_t)
gar_make_basic(u32, uint32_t)
gar_make_basic(u64, uint64_t)

gar_make_basic(f32, float)
gar_make_basic(f64, double)
gar_make_basic(f128, long double)

gar_make_basic(char, char)
gar_make_basic(uchar, unsigned char)
gar_make_basic(schar, signed char)

gar_make_basic(string, char*)
gar_make_deepcopy(string, char*, strdup)
gar_make_free(string, char*, free)
