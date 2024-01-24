#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gar.h"
#include "slz.h"

gar_make_basic(short, short)

gar_make_basic(int, int)
gar_make_serialize(int, int, int2string)
gar_make_deserialize(int, int, string2int)

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

gdc_error_t GAR_FUNC(char, push_string)(char_gar_t* array, char* str) {
    gdc_error_t error;
    size_t len, new_capacity, new_size;

    new_capacity = array->capacity;
    new_size = array->size + (len = strlen(str));

    while (new_capacity < new_size) {new_capacity += new_capacity / 2;}
    error = char_gar_set_capacity(array, new_capacity);

    if (error != GDC_OK) {return error;}

    for (size_t i = 0; i < len; i++) {
        array->values[array->size + i] = str[i];
    }
    array->size += len;

    return error;
}

char* string_deepcopy(const char* src) {
    size_t len = strlen(src);
    char* dst = malloc(len + 1);
    
    for (size_t i = 0; i < len + 1; i++) {
        dst[i] = src[i];
    }

    return dst;
}

gar_make_basic(string, char*)
gar_make_deepcopy(string, char*, string_deepcopy)
gar_make_free(string, char*, free)

