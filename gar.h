#ifndef GAR_H
#define GAR_H

#include <stddef.h>
#include <stdint.h>

typedef enum gar_error {
    GAR_OK,
    GAR_SMALL_CAPACITY,
    GAR_BIG_CAPACITY,
    GAR_IDX_OOB,
} gar_error_t;

#define GARP_JOIN(a, b) GARP_JOIN_(a, b)
#define GARP_JOIN_(a, b) a ## b

#define GARP_ARR(name)  GARP_JOIN(name, _gar_t)
#define GARP_IDX        GARP_JOIN(garp_index_, __LINE__)
#define GARP_ITER       GARP_JOIN(garp_iter_, __LINE__)

#define for_each_gar(arr, value) \
    for (                                                                       \
        size_t GARP_IDX = 0, GARP_ITER = 1;                                     \
        GARP_IDX < (arr)->size;                                                 \
        GARP_ITER = 1, GARP_IDX++                                               \
    )                                                                           \
    for (                                                                       \
        value = (arr)->values[GARP_IDX];                                        \
        GARP_ITER;                                                              \
        GARP_ITER = !GARP_ITER                                                  \
    )

#define for_each_ptr_gar(arr, value) \
    for (                                                                       \
        size_t GARP_IDX = 0, GARP_ITER = 1;                                     \
        GARP_IDX < (arr)->size;                                                 \
        GARP_ITER = 1, GARP_IDX++                                               \
    )                                                                           \
    for (                                                                       \
        value = &(arr)->values[GARP_IDX];                                       \
        GARP_ITER;                                                              \
        GARP_ITER = !GARP_ITER                                                  \
    )

#define gar_make_basic_h(name, type) \
    typedef struct GARP_ARR(name) {                                             \
        size_t capacity;                                                        \
        size_t size;                                                            \
        type* values;                                                           \
    } GARP_ARR(name);                                                           \
                                                                                \
    void        GARP_JOIN(name, _gar_new)(GARP_ARR(name)*);                     \
    gar_error_t GARP_JOIN(name, _gar_copy)(const GARP_ARR(name)*, GARP_ARR(name)*); \
    void        GARP_JOIN(name, _gar_free)(GARP_ARR(name)*);                    \
    gar_error_t GARP_JOIN(name, _gar_set_capacity)(GARP_ARR(name)*, size_t);    \
    gar_error_t GARP_JOIN(name, _gar_fit_capacity)(GARP_ARR(name)*);            \
                                                                                \
    gar_error_t GARP_JOIN(name, _gar_set)(GARP_ARR(name)*, size_t, type);       \
    gar_error_t GARP_JOIN(name, _gar_get)(GARP_ARR(name)*, size_t, type*);      \
                                                                                \
    gar_error_t GARP_JOIN(name, _gar_push)(GARP_ARR(name)*, type);              \
    gar_error_t GARP_JOIN(name, _gar_pop)(GARP_ARR(name)*, type*);              \
                                                                                \
    gar_error_t GARP_JOIN(name, _gar_concat)(GARP_ARR(name)*, const GARP_ARR(name)*); \
                                                                                \
    gar_error_t GARP_JOIN(name, _gar_insert)(GARP_ARR(name)*, size_t, type);    \
    gar_error_t GARP_JOIN(name, _gar_remove)(GARP_ARR(name)*, size_t, type*);   \
                                                                                \
    size_t      GARP_JOIN(name, _gar_count)(const GARP_ARR(name)*, int (*filter)(type)); \
    gar_error_t GARP_JOIN(name, _gar_find)(const GARP_ARR(name)*, int (*filter)(type), type*); \
    gar_error_t GARP_JOIN(name, _gar_filter)(GARP_ARR(name)*, int (*filter)(type), GARP_ARR(name)*); \
                                                                                \
    void        GARP_JOIN(name, _gar_sort)(GARP_ARR(name)*, int (*ord)(type, type)); \

#define gar_make_deepcopy_h(name, type) \
    gar_error_t GARP_JOIN(name, _gar_deepcopy)(const GARP_ARR(name)*, GARP_ARR(name)*); \

#define gar_make_free_h(name, type) \
    void        GARP_JOIN(name, _gar_free_values)(GARP_ARR(name)*);             \

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
    void GARP_JOIN(name, _gar_new)(GARP_ARR(name)* array) {                     \
        array->capacity = 0;                                                    \
        array->size = 0;                                                        \
        array->values = NULL;                                                   \
    }

#define GARP_COPY(name, type) \
    gar_error_t GARP_JOIN(name, _gar_copy)(const GARP_ARR(name)* src, GARP_ARR(name)* dst) { \
        gar_error_t error = GAR_OK;                                             \
                                                                                \
        dst->capacity = src->size;                                              \
        dst->size = src->size;                                                  \
                                                                                \
        dst->values = malloc(src->size * sizeof(type));                         \
        if (dst->values == NULL && src->size != 0) {                            \
            dst->capacity = 0;                                                  \
            dst->size = 0;                                                      \
            return GAR_BIG_CAPACITY;                                            \
        }                                                                       \
                                                                                \
        memcpy(dst->values, src->values, src->size * sizeof(type));             \
        return GAR_OK;                                                          \
    }

#define GARP_FREE(name, type) \
    void GARP_JOIN(name, _gar_free)(GARP_ARR(name)* array) {                    \
        free(array->values);                                                    \
    }

#define GARP_SET_CAPACITY(name, type) \
    gar_error_t GARP_JOIN(name, _gar_set_capacity)(GARP_ARR(name)* array, size_t capacity) { \
        type* values;                                                           \
                                                                                \
        if (capacity < array->size) {                                           \
            return GAR_SMALL_CAPACITY;                                          \
        }                                                                       \
        if (capacity == array->capacity) {                                      \
            return GAR_OK;                                                      \
        }                                                                       \
                                                                                \
        values = malloc(capacity * sizeof(type));                               \
                                                                                \
        if (values == NULL && capacity != 0) {                                  \
            return GAR_BIG_CAPACITY;                                            \
        }                                                                       \
                                                                                \
        memcpy(values, array->values, array->size * sizeof(type));              \
        array->capacity = capacity;                                             \
        free(array->values);                                                    \
        array->values = values;                                                 \
                                                                                \
        return GAR_OK;                                                          \
    }

#define GARP_FIT_CAPACITY(name, type) \
    gar_error_t GARP_JOIN(name, _gar_fit_capacity)(GARP_ARR(name)* array) {     \
        return GARP_JOIN(name, _gar_set_capacity)(array, array->size);          \
    }

#define GARP_SET(name, type) \
    gar_error_t GARP_JOIN(name, _gar_set)(GARP_ARR(name)* array, size_t index, type value) { \
        if (array->size <= index) {                                             \
            return GAR_IDX_OOB;                                                 \
        }                                                                       \
        array->values[index] = value;                                           \
        return GAR_OK;                                                          \
    }

#define GARP_GET(name, type) \
    gar_error_t GARP_JOIN(name, _gar_get)(GARP_ARR(name)* array, size_t index, type* value) { \
        if (array->size <= index) {                                             \
            return GAR_IDX_OOB;                                                 \
        }                                                                       \
        *value = array->values[index];                                          \
        return GAR_OK;                                                          \
    }

#define GARP_PUSH(name, type) \
    gar_error_t GARP_JOIN(name, _gar_push)(GARP_ARR(name)* array, type value) { \
        if (array->size >= array->capacity) {                                   \
            gar_error_t error = GARP_JOIN(name, _gar_set_capacity)(             \
                array, 2 * array->capacity + (array->capacity == 0)             \
            );                                                                  \
            if (error != GAR_OK) {                                              \
                return error;                                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        array->values[array->size++] = value;                                   \
        return GAR_OK;                                                          \
    }

#define GARP_POP(name, type) \
    gar_error_t GARP_JOIN(name, _gar_pop)(GARP_ARR(name)* array, type* value_ptr) { \
        if (array->size <= 0) {                                                 \
            return GAR_IDX_OOB;                                                 \
        }                                                                       \
                                                                                \
        if (value_ptr != NULL) {                                                \
            *value_ptr = array->values[array->size - 1];                        \
        }                                                                       \
                                                                                \
        array->size -= 1;                                                       \
        return GAR_OK;                                                          \
    }

#define GARP_CONCAT(name, type) \
    gar_error_t GARP_JOIN(name, _gar_concat)(GARP_ARR(name)* array, const GARP_ARR(name)* append_array) { \
        gar_error_t error;                                                      \
        size_t capacity, new_size;                                              \
        new_size = array->size + append_array->size;                            \
                                                                                \
        if (new_size > array->capacity) {                                       \
            capacity = array->capacity + (array->capacity == 0);                \
            while (capacity < new_size) {                                       \
                capacity *= 2;                                                  \
            }                                                                   \
            error = GARP_JOIN(name, _gar_set_capacity)(array, capacity);        \
            if (error) {                                                        \
                return error;                                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        for (size_t i = 0; i < append_array->size; i++) {                       \
            array->values[array->size + i] = append_array->values[i];           \
        }                                                                       \
        array->size += append_array->size;                                      \
                                                                                \
        return GAR_OK;                                                          \
    }

#define GARP_INSERT(name, type) \
    gar_error_t GARP_JOIN(name, _gar_insert)(GARP_ARR(name)* array, size_t index, type value) { \
        if (index < 0 || array->size + 1 <= index) {                            \
            return GAR_IDX_OOB;                                                 \
        }                                                                       \
                                                                                \
        if (array->size >= array->capacity) {                                   \
            gar_error_t error = GARP_JOIN(name, _gar_set_capacity)(             \
                array, 2 * array->capacity + (array->capacity == 0)             \
            );                                                                  \
            if (error != GAR_OK) {                                              \
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
        return GAR_OK;                                                          \
    }

#define GARP_REMOVE(name, type) \
    gar_error_t GARP_JOIN(name, _gar_remove)(GARP_ARR(name)* array, size_t index, type* value_ptr) { \
        if (index < 0 || array->size < index) {                                 \
            return GAR_IDX_OOB;                                                 \
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
        return GAR_OK;                                                          \
    }

#define GARP_COUNT(name, type) \
    size_t GARP_JOIN(name, _gar_count)(const GARP_ARR(name)* array, int (*filter)(type)) { \
        size_t total_count = 0;                                                 \
        for (size_t i = 0; i < array->size; i++) {                              \
            total_count += filter(array->values[i]) != 0;                       \
        }                                                                       \
                                                                                \
        return total_count;                                                     \
    }

#define GARP_FIND(name, type) \
    gar_error_t GARP_JOIN(name, _gar_find)(const GARP_ARR(name)* array, int (*filter)(type), type* value_ptr) { \
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
            return GAR_IDX_OOB;                                                 \
        }                                                                       \
                                                                                \
        return GAR_OK;                                                          \
    }

#define GARP_FILTER(name, type) \
    gar_error_t GARP_JOIN(name, _gar_filter)(GARP_ARR(name)* src, int (*filter)(type), GARP_ARR(name)* dst) { \
        gar_error_t error;                                                      \
                                                                                \
        GARP_JOIN(name, _gar_new)(dst);                                         \
        error = GARP_JOIN(name, _gar_set_capacity)(dst, src->size);             \
        if (error != GAR_OK) {                                                  \
            return error;                                                       \
        }                                                                       \
                                                                                \
        for (size_t i = 0; i < src->size; i++) {                                \
            if (filter(src->values[i])) {                                       \
                dst->values[dst->size++] = src->values[i];                      \
            }                                                                   \
        }                                                                       \
                                                                                \
        return GAR_OK;                                                          \
    }

#define GARP_SORT(name, type) \
    size_t GARP_JOIN(garp_partition_, name)(GARP_ARR(name)* array, int (*ord)(type, type), size_t lo, size_t hi) {\
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
    void GARP_JOIN(garp_qsort_, name)(GARP_ARR(name)* array, int (*ord)(type, type), size_t lo, size_t hi) { \
        size_t pivot;                                                           \
        if (lo >= hi) {                                                         \
            return;                                                             \
        }                                                                       \
                                                                                \
        pivot = GARP_JOIN(garp_partition_, name)(array, ord, lo, hi);           \
                                                                                \
        if (pivot > 0) {                                                        \
            GARP_JOIN(garp_qsort_, name)(array, ord, lo, pivot - 1);            \
        }                                                                       \
        GARP_JOIN(garp_qsort_, name)(array, ord, pivot + 1, hi);                \
    }                                                                           \
                                                                                \
    void GARP_JOIN(name, _gar_sort)(GARP_ARR(name)* array, int (*ord)(type, type)) {\
        if (array->size > 1) {                                                  \
            GARP_JOIN(garp_qsort_, name)(array, ord, 0, array->size - 1);       \
        }                                                                       \
                                                                                \
    }

#define gar_make_deepcopy(name, type, copy_function) \
    gar_error_t GARP_JOIN(name, _gar_deepcopy)(const GARP_ARR(name)* src, GARP_ARR(name)* dst) { \
        gar_error_t error;                                                      \
                                                                                \
        GARP_JOIN(name, _gar_new)(dst);                                         \
        error = GARP_JOIN(name, _gar_set_capacity)(dst, src->size);             \
        if (error != GAR_OK) {                                                  \
            return error;                                                       \
        }                                                                       \
                                                                                \
        for (size_t i = 0; i < src->size; i++) {                                \
            dst->values[i] = copy_function(src->values[i]);                     \
        }                                                                       \
        dst->size = src->size;                                                  \
                                                                                \
        return GAR_OK;                                                          \
    }

#define gar_make_free(name, type, free_func) \
    void GARP_JOIN(name, _gar_free_values)(GARP_ARR(name)* array) {             \
        for (size_t i = 0; i < array->size; i++) {                              \
            free_func(array->values[i]);                                        \
        }                                                                       \
        array->size = 0;                                                        \
    }


gar_make_basic_h(short, short)
gar_make_basic_h(int, int)
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

gar_make_basic_h(char, char)
gar_make_basic_h(uchar, unsigned char)
gar_make_basic_h(schar, signed char)

gar_make_basic_h(string, char*)
gar_make_deepcopy_h(string, char*)
gar_make_free_h(string, char*)

#endif

