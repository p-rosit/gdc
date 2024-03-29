#include <stdlib.h>
#include <string.h>

#include "gar.h"

gar_make_basic(short, short, )

gar_make_basic(int, int, )
gar_make_basic(long, long, )
gar_make_basic(long_long, long long, )
gar_make_basic(ushort, unsigned short, )
gar_make_basic(uint, unsigned int, )
gar_make_basic(ulong, unsigned long, )
gar_make_basic(ulong_long, unsigned long long, )

gar_make_basic(f32, float, )
gar_make_basic(f64, double, )
gar_make_basic(f128, long double, )

gar_make_basic(char, char, )
gar_make_basic(uchar, unsigned char, )
gar_make_basic(schar, signed char, )

char* string_deepcopy(const char* src) {
    size_t len = strlen(src);
    char* dst = malloc(len + 1);
    
    for (size_t i = 0; i < len + 1; i++) {
        dst[i] = src[i];
    }

    return dst;
}

gar_make_basic(string, char*, free)
gar_make_deepcopy(string, char*, string_deepcopy)

