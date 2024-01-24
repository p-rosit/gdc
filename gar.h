#ifndef GAR_H
#define GAR_H
#include "gdc.h"

/**
 * GAR: Growable ARray
 */

/* Name of structure */
#define GAR_NAME gar
#define GAR_FUNC(type_name, func_name)      GDC_FUNC(type_name, GAR_NAME, func_name)
#define GAR_PRIVATE(type_name, func_name)   GDC_FUNC_PRIVATE(type_name, GAR_NAME, func_name)
#define GAR(name)                           GDC_STRUCT(name, GAR_NAME)
#define GAR_STRUCT(name)                    GDC_STRUCT_PRIVATE(GAR_NAME, name)

#include <stddef.h>
#include <stdint.h>
#include <ctype.h>

#define GARP_IDX    GDC_JOIN(garp_index_, __LINE__)
#define GARP_ITER   GDC_JOIN(garp_iter_, __LINE__)

#define for_each_gar(arr, value) \
    for (                                                                       \
        size_t GARP_IDX = 0, GARP_ITER = 1;                                     \
        GARP_IDX < (arr).size;                                                 \
        GARP_ITER = 1, GARP_IDX++                                               \
    )                                                                           \
    for (                                                                       \
        value = (arr).values[GARP_IDX];                                        \
        GARP_ITER;                                                              \
        GARP_ITER = !GARP_ITER                                                  \
    )

#define for_each_ptr_gar(arr, value) \
    for (                                                                       \
        size_t GARP_IDX = 0, GARP_ITER = 1;                                     \
        GARP_IDX < (arr).size;                                                 \
        GARP_ITER = 1, GARP_IDX++                                               \
    )                                                                           \
    for (                                                                       \
        value = &(arr).values[GARP_IDX];                                       \
        GARP_ITER;                                                              \
        GARP_ITER = !GARP_ITER                                                  \
    )

#define gar_make_basic_h(name, type) \
    typedef struct GAR(name) {                                                  \
        size_t capacity;                                                        \
        size_t size;                                                            \
        type* values;                                                           \
    } GAR(name);                                                                \
                                                                                \
    void        GAR_FUNC(name, new)(GAR(name)*);                                \
    gdc_error_t GAR_FUNC(name, copy)(const GAR(name)*, GAR(name)*);             \
    void        GAR_FUNC(name, free)(GAR(name)*);                               \
    gdc_error_t GAR_FUNC(name, set_capacity)(GAR(name)*, size_t);               \
    gdc_error_t GAR_FUNC(name, fit_capacity)(GAR(name)*);                       \
                                                                                \
    gdc_error_t GAR_FUNC(name, set)(GAR(name)*, size_t, type);                  \
    gdc_error_t GAR_FUNC(name, get)(GAR(name)*, size_t, type*);                 \
                                                                                \
    gdc_error_t GAR_FUNC(name, push)(GAR(name)*, type);                         \
    gdc_error_t GAR_FUNC(name, pop)(GAR(name)*, type*);                         \
                                                                                \
    gdc_error_t GAR_FUNC(name, concat)(GAR(name)*, GAR(name)*);                 \
                                                                                \
    gdc_error_t GAR_FUNC(name, insert)(GAR(name)*, size_t, type);               \
    gdc_error_t GAR_FUNC(name, remove)(GAR(name)*, size_t, type*);              \
                                                                                \
    size_t      GAR_FUNC(name, count)(const GAR(name)*, int (*filter)(type));   \
    gdc_error_t GAR_FUNC(name, find)(const GAR(name)*, int (*filter)(type), type*); \
    gdc_error_t GAR_FUNC(name, filter)(GAR(name)*, int (*filter)(type), GAR(name)*); \
                                                                                \
    void        GAR_FUNC(name, sort)(GAR(name)*, int (*ord)(type, type));       \

#define gar_make_deepcopy_h(name, type) \
    gdc_error_t GAR_FUNC(name, deepcopy)(const GAR(name)*, GAR(name)*);         \

#define gar_make_free_h(name, type) \
    void        GAR_FUNC(name, free_values)(GAR(name)*);                        \

#define gar_make_serialize_h(name, type) \
    gdc_error_t GAR_FUNC(name, to_json)(const GAR(name)*, char**);              \
    gdc_error_t GAR_FUNC(name, to_json_helper)(const GAR(name)*, char_gar_t*);  \

#define gar_make_deserialize_h(name, type) \
    gdc_error_t GAR_FUNC(name, from_json)(GAR(name)*, char*);                   \

#define gar_make_basic(name, type) \
    GARP_NEW(name, type)                                                        \
    GARP_COPY(name, type)                                                       \
    GARP_FREE(name, type)                                                       \
    GARP_SET_CAPACITY(name, type)                                               \
    GARP_FIT_CAPACITY(name, type)                                               \
                                                                                \
    GARP_SET(name, type)                                                        \
    GARP_GET(name, type)                                                        \
                                                                                \
    GARP_PUSH(name, type)                                                       \
    GARP_POP(name, type)                                                        \
                                                                                \
    GARP_CONCAT(name, type)                                                     \
                                                                                \
    GARP_INSERT(name, type)                                                     \
    GARP_REMOVE(name, type)                                                     \
                                                                                \
    GARP_COUNT(name, type)                                                      \
    GARP_FIND(name, type)                                                       \
    GARP_FILTER(name, type)                                                     \
                                                                                \
    GARP_SORT(name, type)                                                       \

#define GARP_NEW(name, type) \
    void GAR_FUNC(name, new)(GAR(name)* array) {                                \
        array->capacity = 0;                                                    \
        array->size = 0;                                                        \
        array->values = NULL;                                                   \
    }

#define GARP_COPY(name, type) \
    gdc_error_t GAR_FUNC(name, copy)(const GAR(name)* src, GAR(name)* dst) {    \
        gdc_error_t error = GDC_OK;                                             \
                                                                                \
        dst->capacity = src->size;                                              \
        dst->size = src->size;                                                  \
                                                                                \
        dst->values = malloc(src->size * sizeof(type));                         \
        if (dst->values == NULL && src->size != 0) {                            \
            dst->capacity = 0;                                                  \
            dst->size = 0;                                                      \
            return GDC_MEMORY_ERROR;                                            \
        }                                                                       \
                                                                                \
        memcpy(dst->values, src->values, src->size * sizeof(type));             \
        return error;                                                           \
    }

#define GARP_FREE(name, type) \
    void GAR_FUNC(name, free)(GAR(name)* array) {                               \
        array->capacity = 0;                                                    \
        array->size = 0;                                                        \
        free(array->values);                                                    \
        array->values = NULL;                                                   \
    }

#define GARP_SET_CAPACITY(name, type) \
    gdc_error_t GAR_FUNC(name, set_capacity)(GAR(name)* array, size_t capacity) { \
        type* values;                                                           \
                                                                                \
        if (capacity < array->size) {                                           \
            return GDC_CAPACITY_ERROR;                                          \
        }                                                                       \
        if (capacity == array->capacity) {                                      \
            return GDC_OK;                                                      \
        }                                                                       \
                                                                                \
        values = malloc(capacity * sizeof(type));                               \
                                                                                \
        if (values == NULL && capacity != 0) {                                  \
            return GDC_MEMORY_ERROR;                                            \
        }                                                                       \
                                                                                \
        memcpy(values, array->values, array->size * sizeof(type));              \
        array->capacity = capacity;                                             \
        free(array->values);                                                    \
        array->values = values;                                                 \
                                                                                \
        return GDC_OK;                                                          \
    }

#define GARP_FIT_CAPACITY(name, type) \
    gdc_error_t GAR_FUNC(name, fit_capacity)(GAR(name)* array) {                \
        return GAR_FUNC(name, set_capacity)(array, array->size);                \
    }

#define GARP_SET(name, type) \
    gdc_error_t GAR_FUNC(name, set)(GAR(name)* array, size_t index, type value) { \
        if (array->size <= index) {                                             \
            return GDC_INDEX_OOB;                                               \
        }                                                                       \
        array->values[index] = value;                                           \
        return GDC_OK;                                                          \
    }

#define GARP_GET(name, type) \
    gdc_error_t GAR_FUNC(name, get)(GAR(name)* array, size_t index, type* value) { \
        if (array->size <= index) {                                             \
            return GDC_INDEX_OOB;                                               \
        }                                                                       \
        *value = array->values[index];                                          \
        return GDC_OK;                                                          \
    }

#define GARP_PUSH(name, type) \
    gdc_error_t GAR_FUNC(name, push)(GAR(name)* array, type value) {            \
        size_t capacity = array->capacity;                                      \
        if (array->size >= capacity) {                                          \
            gdc_error_t error = GAR_FUNC(name, set_capacity)(                   \
                array, capacity + capacity / 2 + 2 * (capacity == 0)            \
            );                                                                  \
            if (error != GDC_OK) {                                              \
                return error;                                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        array->values[array->size++] = value;                                   \
        return GDC_OK;                                                          \
    }

#define GARP_POP(name, type) \
    gdc_error_t GAR_FUNC(name, pop)(GAR(name)* array, type* value_ptr) {        \
        if (array->size <= 0) {                                                 \
            return GDC_INDEX_OOB;                                               \
        }                                                                       \
                                                                                \
        if (value_ptr != NULL) {                                                \
            *value_ptr = array->values[array->size - 1];                        \
        }                                                                       \
                                                                                \
        array->size -= 1;                                                       \
        return GDC_OK;                                                          \
    }

#define GARP_CONCAT(name, type) \
    gdc_error_t GAR_FUNC(name, concat)(GAR(name)* array, GAR(name)* from_array) { \
        gdc_error_t error;                                                      \
        size_t capacity, new_size;                                              \
        new_size = array->size + from_array->size;                              \
                                                                                \
        if (new_size > array->capacity) {                                       \
            capacity = array->capacity + 2 * (array->capacity == 0);            \
            while (capacity < new_size) {                                       \
                capacity += capacity / 2;                                       \
            }                                                                   \
            error = GAR_FUNC(name, set_capacity)(array, capacity);              \
            if (error) {                                                        \
                return error;                                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        for (size_t i = 0; i < from_array->size; i++) {                         \
            array->values[array->size + i] = from_array->values[i];             \
        }                                                                       \
        array->size += from_array->size;                                        \
                                                                                \
        from_array->size = 0;                                                   \
                                                                                \
        return GDC_OK;                                                          \
    }

#define GARP_INSERT(name, type) \
    gdc_error_t GAR_FUNC(name, insert)(GAR(name)* array, size_t index, type value) { \
        size_t capacity;                                                        \
                                                                                \
        if (index < 0 || array->size + 1 <= index) {                            \
            return GDC_INDEX_OOB;                                               \
        }                                                                       \
                                                                                \
        capacity = array->capacity;                                             \
        if (array->size >= capacity) {                                          \
            gdc_error_t error = GAR_FUNC(name, set_capacity)(                   \
                array, capacity + capacity / 2 + 2 * (capacity == 0)            \
            );                                                                  \
            if (error != GDC_OK) {                                              \
                return error;                                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        for (size_t i = array->size; i >= index; i--) {                         \
            array->values[i + 1] = array->values[i];                            \
        }                                                                       \
                                                                                \
        array->values[index] = value;                                           \
        array->size += 1;                                                       \
                                                                                \
        return GDC_OK;                                                          \
    }

#define GARP_REMOVE(name, type) \
    gdc_error_t GAR_FUNC(name, remove)(GAR(name)* array, size_t index, type* value_ptr) { \
        if (index < 0 || array->size < index) {                                 \
            return GDC_INDEX_OOB;                                               \
        }                                                                       \
                                                                                \
        if (value_ptr != NULL) {                                                \
            *value_ptr = array->values[index];                                  \
        }                                                                       \
                                                                                \
        for (size_t i = index; i < array->size; i++) {                          \
            array->values[i] = array->values[i + 1];                            \
        }                                                                       \
        array->size -= 1;                                                       \
                                                                                \
        return GDC_OK;                                                          \
    }

#define GARP_COUNT(name, type) \
    size_t GAR_FUNC(name, count)(const GAR(name)* array, int (*filter)(type)) { \
        size_t total_count = 0;                                                 \
        for (size_t i = 0; i < array->size; i++) {                              \
            total_count += filter(array->values[i]) != 0;                       \
        }                                                                       \
                                                                                \
        return total_count;                                                     \
    }

#define GARP_FIND(name, type) \
    gdc_error_t GAR_FUNC(name, find)(const GAR(name)* array, int (*filter)(type), type* value_ptr) { \
        int exists = 0;                                                         \
        for (size_t i = 0; i < array->size; i++) {                              \
            if (filter(array->values[i])) {                                     \
                exists = 1;                                                     \
                if (value_ptr != NULL) {                                        \
                    *value_ptr = array->values[i];                              \
                }                                                               \
                break;                                                          \
            }                                                                   \
        }                                                                       \
                                                                                \
        if (!exists) {                                                          \
            return GDC_INDEX_OOB;                                               \
        }                                                                       \
                                                                                \
        return GDC_OK;                                                          \
    }

#define GARP_FILTER(name, type) \
    gdc_error_t GAR_FUNC(name, filter)(GAR(name)* src, int (*filter)(type), GAR(name)* dst) { \
        gdc_error_t error;                                                      \
                                                                                \
        GAR_FUNC(name, new)(dst);                                               \
        error = GAR_FUNC(name, set_capacity)(dst, src->size);                   \
        if (error != GDC_OK) {                                                  \
            return error;                                                       \
        }                                                                       \
                                                                                \
        for (size_t i = 0; i < src->size; i++) {                                \
            if (filter(src->values[i])) {                                       \
                dst->values[dst->size++] = src->values[i];                      \
            }                                                                   \
        }                                                                       \
                                                                                \
        return GDC_OK;                                                          \
    }

#define GARP_SORT(name, type) \
    size_t GAR_PRIVATE(name, partition)(GAR(name)* array, int (*ord)(type, type), size_t lo, size_t hi) {\
        int set = 0;                                                            \
        size_t i = lo;                                                          \
        type pivot;                                                             \
        type temp;                                                              \
        pivot = array->values[hi];                                              \
                                                                                \
        for (size_t j = lo; j < hi; j++) {                                      \
            if (ord(array->values[j], pivot) < 0) {                             \
                i += set;                                                       \
                set = 1;                                                        \
                temp = array->values[i];                                        \
                array->values[i] = array->values[j];                            \
                array->values[j] = temp;                                        \
            }                                                                   \
        }                                                                       \
                                                                                \
        i += set;                                                               \
        temp = array->values[i];                                                \
        array->values[i] = array->values[hi];                                   \
        array->values[hi] = temp;                                               \
                                                                                \
        return i;                                                               \
    }                                                                           \
                                                                                \
    void GAR_PRIVATE(name, qsort)(GAR(name)* array, int (*ord)(type, type), size_t lo, size_t hi) { \
        size_t pivot;                                                           \
        if (lo >= hi) {                                                         \
            return;                                                             \
        }                                                                       \
                                                                                \
        pivot = GAR_PRIVATE(name, partition)(array, ord, lo, hi);               \
                                                                                \
        if (pivot > 0) {                                                        \
            GAR_PRIVATE(name, qsort)(array, ord, lo, pivot - 1);                \
        }                                                                       \
        GAR_PRIVATE(name, qsort)(array, ord, pivot + 1, hi);                    \
    }                                                                           \
                                                                                \
    void GAR_FUNC(name, sort)(GAR(name)* array, int (*ord)(type, type)) {       \
        if (array->size > 1) {                                                  \
            GAR_PRIVATE(name, qsort)(array, ord, 0, array->size - 1);           \
        }                                                                       \
                                                                                \
    }

#define gar_make_deepcopy(name, type, copy_function) \
    gdc_error_t GAR_FUNC(name, deepcopy)(const GAR(name)* src, GAR(name)* dst) { \
        gdc_error_t error;                                                      \
                                                                                \
        GAR_FUNC(name, new)(dst);                                               \
        error = GAR_FUNC(name, set_capacity)(dst, src->size);                   \
        if (error != GDC_OK) {                                                  \
            return error;                                                       \
        }                                                                       \
                                                                                \
        for (size_t i = 0; i < src->size; i++) {                                \
            dst->values[i] = copy_function(src->values[i]);                     \
        }                                                                       \
        dst->size = src->size;                                                  \
                                                                                \
        return GDC_OK;                                                          \
    }

#define gar_make_free(name, type, free_func) \
    void GAR_FUNC(name, free_values)(GAR(name)* array) {                        \
        for (size_t i = 0; i < array->size; i++) {                              \
            free_func(array->values[i]);                                        \
        }                                                                       \
        array->size = 0;                                                        \
    }

#define gar_make_serialize(name, type, item_to_json) \
    gdc_error_t GAR_FUNC(name, to_json)(const GAR(name)* array, char** json) { \
        gdc_error_t error;\
        char_gar_t str; \
        \
        *json = NULL; \
        char_gar_new(&str); \
        error = GAR_FUNC(name, to_json_helper)(array, &str); \
        \
        if (error == GDC_OK) { \
            *json = str.values; \
        }\
        \
        return error; \
    } \
    \
    gdc_error_t GAR_FUNC(name, to_json_helper)(const GAR(name)* array, char_gar_t* json) { \
        gdc_error_t error; \
        \
        error = char_gar_push(json, '['); \
        if (error != GDC_OK) {goto execution_failed;} \
        \
        for (size_t i = 0; i < array->size; i++) { \
            if (i > 0) { \
                error = char_gar_push_string(json, ", ");\
                if (error != GDC_OK) {goto execution_failed;} \
            } \
            \
            error = item_to_json(json, array->values[i]); \
            if (error != GDC_OK) {goto execution_failed;} \
        } \
        \
        error = char_gar_push(json, ']');\
        if (error != GDC_OK) {goto execution_failed;} \
        error = char_gar_push(json, '\0');\
        if (error != GDC_OK) {goto execution_failed;} \
        \
        return error; \
        \
        execution_failed:\
        GAR_FUNC(char, free)(json);\
        return error;\
    }

#define gar_make_deserialize(name, type, json_to_item) \
    void GAR_PRIVATE(name, skip_whitespace)(char** str) { \
        while (isspace(**str)) {(*str)++;} \
    } \
    \
    gdc_error_t GAR_PRIVATE(name, start_array)(char** json) { \
        if (**json == '[') { \
            (*json)++; return GDC_OK; \
        } else { \
            return GDC_PARSE_ERROR; \
        } \
    } \
    \
    gdc_error_t GAR_PRIVATE(name, stop_array)(char** json) {\
        if (**json == ']') { \
            (*json)++; return GDC_OK; \
        } else { \
            return GDC_PARSE_ERROR; \
        } \
    } \
    \
    gdc_error_t GAR_PRIVATE(name, next_item)(char** json) {\
        if (**json == ',') { \
            (*json)++; return GDC_OK; \
        } else { \
            return GDC_PARSE_ERROR; \
        } \
    } \
    \
    gdc_error_t GAR_FUNC(name, from_json)(GAR(name)* array, char* json) { \
        gdc_error_t error, stop_error; \
        type value; \
        \
        GAR_FUNC(name, new)(array);\
        \
        GAR_PRIVATE(name, skip_whitespace)(&json); \
        error = GAR_PRIVATE(name, start_array)(&json); \
        if (error != GDC_OK) {goto execution_failed;} \
        \
        GAR_PRIVATE(name, skip_whitespace)(&json); \
        stop_error = GAR_PRIVATE(name, stop_array)(&json);\
        \
        while (stop_error != GDC_OK) { \
            GAR_PRIVATE(name, skip_whitespace)(&json); \
            error = json_to_item(&value, &json);\
            if (error != GDC_OK) {goto execution_failed;}\
            error = GAR_FUNC(name, push)(array, value); \
            if (error != GDC_OK) {goto execution_failed;}\
            \
            GAR_PRIVATE(name, skip_whitespace)(&json); \
            error = GAR_PRIVATE(name, next_item)(&json); \
            if (error == GDC_OK) { \
                continue; \
            } \
            \
            stop_error = GAR_PRIVATE(name, stop_array)(&json);\
            if (stop_error == GDC_OK) { \
                break;\
            } else {\
                goto execution_failed; \
            } \
        } \
        \
        return GDC_OK; \
        \
        execution_failed: \
        GAR_FUNC(name, free)(array); \
        return error; \
    }

gar_make_basic_h(char, char)
gdc_error_t GAR_FUNC(char, push_string)(char_gar_t*, char*);

gar_make_basic_h(uchar, unsigned char)
gar_make_basic_h(schar, signed char)

gar_make_basic_h(string, char*)
gar_make_deepcopy_h(string, char*)
gar_make_free_h(string, char*)

gar_make_basic_h(short, short)

gar_make_basic_h(int, int)
gar_make_serialize_h(int, int)
gar_make_deserialize_h(int, int)

gar_make_basic_h(long, long)
gar_make_basic_h(long_long, long long)
gar_make_basic_h(ushort, unsigned short)
gar_make_basic_h(uint, unsigned int)
gar_make_basic_h(ulong, unsigned long)
gar_make_basic_h(ulong_long, unsigned long long)

gar_make_basic_h(i8, int8_t)
gar_make_basic_h(i16, int16_t)
gar_make_basic_h(i32, int32_t)
gar_make_basic_h(i64, int64_t)
gar_make_basic_h(u8, uint8_t)
gar_make_basic_h(u16, uint16_t)
gar_make_basic_h(u32, uint32_t)
gar_make_basic_h(u64, uint64_t)

gar_make_basic_h(f32, float)
gar_make_basic_h(f64, double)
gar_make_basic_h(f128, long double)

#endif

