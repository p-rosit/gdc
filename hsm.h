#ifndef HSM_H
#define HSM_H
#include "gdc.h"

/*
 *  HSM: HaSh Map
 **/

/* Name of structure */
#define HSM_NAME hsm
#define HSM_FUNC(type_name, func_name)      GDC_FUNC(type_name, HSM_NAME, func_name)
#define HSM_PRIVATE(type_name, func_name)   GDC_FUNC_PRIVATE(type_name, HSM_NAME, func_name)
#define HSM(name)                           GDC_STRUCT(name, HSM_NAME)
#define HSM_STRUCT(name)                    GDC_STRUCT_PRIVATE(HSM_NAME, name)

#include <stddef.h>
#include <stdint.h>

typedef struct HSM_STRUCT(meta_data) {
    size_t offset;
    size_t hash;
} HSM_STRUCT(meta_data);

#define HSMP_IDX    GDC_JOIN(garp_index_, __LINE__)
#define HSMP_ITER   GDC_JOIN(garp_iter_, __LINE__)

#define for_each_hsm(map, key, value) \
    for ( \
        size_t HSMP_IDX = 0, HSMP_ITER = 1; \
        HSMP_IDX < (map).capacity + (map).max_offset; \
        HSMP_ITER = 1, HSMP_IDX++ \
    ) \
    for ( \
        ;\
        HSMP_ITER && (map).meta_data[HSMP_IDX].offset < (map).max_offset;\
        \
    ) \
    for ( \
        key = (map).keys[HSMP_IDX]; \
        HSMP_ITER; \
        \
    ) \
    for ( \
        value = (map).values[HSMP_IDX]; \
        HSMP_ITER; \
        HSMP_ITER = !HSMP_ITER \
    )

#define for_each_ptr_hsm(map, key, value_ptr) \
    for ( \
        size_t HSMP_IDX = 0, HSMP_ITER = 1; \
        HSMP_IDX < (map).capacity + (map).max_offset; \
        HSMP_ITER = 1, HSMP_IDX++ \
    ) \
    for ( \
        ;\
        HSMP_ITER && (map).meta_data[HSMP_IDX].offset < (map).max_offset;\
        \
    ) \
    for ( \
        key = (map).keys[HSMP_IDX]; \
        HSMP_ITER; \
        \
    ) \
    for ( \
        value_ptr = &(map).values[HSMP_IDX]; \
        HSMP_ITER; \
        HSMP_ITER = !HSMP_ITER \
    )

#define hsm_make_basic_h(name, key_type, value_type) \
    typedef struct HSM(name) {                                                  \
        size_t capacity;                                                        \
        size_t size;                                                            \
        size_t max_offset;                                                      \
        HSM_STRUCT(meta_data)* meta_data;                                       \
        key_type* keys;                                                         \
        value_type* values;                                                     \
    } HSM(name);                                                                \
                                                                                \
    void        HSM_FUNC(name, new)(HSM(name)*);                                \
    gdc_error_t HSM_FUNC(name, copy)(const HSM(name)*, HSM(name)*);             \
    void        HSM_FUNC(name, free)(HSM(name)*);                               \
    gdc_error_t HSM_FUNC(name, ensure_capacity)(HSM(name)*, size_t);            \
    gdc_error_t HSM_FUNC(name, fit_capacity)(HSM(name)*);                       \
                                                                                \
    gdc_error_t HSM_FUNC(name, hsm_insert)(HSM(name)*, key_type, value_type);   \
    gdc_error_t HSM_FUNC(name, hsm_delete)(HSM(name)*, key_type, value_type*);  \
                                                                                \
    gdc_error_t HSM_FUNC(name, hsm_set)(HSM(name)*, key_type, value_type);      \
    gdc_error_t HSM_FUNC(name, hsm_get)(HSM(name)*, key_type, value_type*);     \

#define hsm_make_deepcopy_h(name, key_type, value_type) \
    gdc_error_t HSM_FUNC(name, deepcopy)(const HSM(name)*, HSM(name)*);         \

#define hsm_make_free_h(name, key_type, value_type) \
    gdc_error_t HSM_FUNC(name, free_items)(HSM(name)*);                         \

#define hsm_make_serialize_h(name, key_type, value_type) \
    

#define hsm_make_basic(name, key_type, value_type, hash_func) \
    HSMP_HELPER_FUNCTIONS(name, key_type, value_type)                           \
                                                                                \
    HSMP_NEW(name, key_type, value_type, hash_func)                             \
    HSMP_COPY(name, key_type, value_type, hash_func)                            \
    HSMP_FREE(name, key_type, value_type, hash_func)                            \
    HSMP_ENSURE_CAPACITY(name, key_type, value_type, hash_func)                 \
    HSMP_FIT_CAPACITY(name, key_type, value_type, hash_func)                    \
                                                                                \
    HSMP_INSERT(name, key_type, value_type, hash_func)                          \
    HSMP_DELETE(name, key_type, value_type, hash_func)                          \
                                                                                \
    HSMP_SET(name, key_type, value_type, hash_func)                             \
    HSMP_GET(name, key_type, value_type, hash_func)                             \

#define hsm_make_deepcopy(name, key_type, value_type, copy_item_func) \
    HSMP_DEEP_COPY(name, key_type, value_type, copy_item_func)

#define hsm_make_free(name, key_type, value_type, free_item_func) \
    HSMP_FREE_ITEMS(name, key_type, value_type, free_item_func)

#define HSMP_NEW(name, key_type, value_type, hash_func) \
    void HSM_FUNC(name, new)(HSM(name)* map) {                                  \
        map->capacity = 0;                                                      \
        map->size = 0;                                                          \
        map->max_offset = 0;                                                    \
        map->meta_data = NULL;                                                  \
        map->keys = NULL;                                                       \
        map->values = NULL;                                                     \
    }

#define HSMP_COPY(name, key_type, value_type, hash_func) \
    gdc_error_t HSM_FUNC(name, copy)(const HSM(name)* src, HSM(name)* dst) {    \
        gdc_error_t error;                                                      \
        HSM_FUNC(name, new)(dst);                                               \
        error = HSM_PRIVATE(name, set_capacity)(dst, src->capacity);            \
                                                                                \
        if (error != GDC_OK) {                                                  \
            return error;                                                       \
        }                                                                       \
                                                                                \
        for (size_t i = 0; i < src->capacity + src->max_offset; i++) {          \
            dst->meta_data[i] = src->meta_data[i];                              \
            dst->keys[i] = src->keys[i];                                        \
            dst->values[i] = src->values[i];                                    \
        }                                                                       \
                                                                                \
        dst->size = src->size;                                                  \
                                                                                \
        return GDC_OK;                                                          \
    }

#define HSMP_FREE(name, key_type, value_type, hash_func) \
    void HSM_FUNC(name, free)(HSM(name)* map) {                                 \
        map->capacity = 0;                                                      \
        map->size = 0;                                                          \
        map->max_offset = 0;                                                    \
        free(map->meta_data);                                                   \
        free(map->keys);                                                        \
        free(map->values);                                                      \
        map->meta_data = NULL;                                                  \
        map->keys = NULL;                                                       \
        map->values = NULL;                                                     \
    }

#define HSMP_ENSURE_CAPACITY(name, key_type, value_type, hash_func) \
    gdc_error_t HSM_FUNC(name, ensure_capacity)(HSM(name)* map, size_t capacity) { \
        gdc_error_t error = GDC_INCORRECT_CALL;                                 \
        size_t test_capacity = capacity;                                        \
                                                                                \
        while (error == GDC_INCORRECT_CALL) {                                   \
            error = HSM_PRIVATE(name, set_capacity)(map, test_capacity);        \
            if (error == GDC_MEMORY_ERROR) {                                    \
                break;                                                          \
            }                                                                   \
            test_capacity += test_capacity / 2 + 2 * (test_capacity < 2);       \
        }                                                                       \
                                                                                \
        return error;                                                           \
    }

#define HSMP_FIT_CAPACITY(name, key_type, value_type, hash_func) \
    gdc_error_t HSM_FUNC(name, fit_capacity)(HSM(name)* map) {                  \
        return HSM_FUNC(name, ensure_capacity)(map, map->size);                 \
    }

#define HSMP_INSERT(name, key_type, value_type, hash_func) \
    gdc_error_t HSM_FUNC(name, insert)(HSM(name)* map, key_type key, value_type value) { \
        gdc_error_t error;                                                      \
        size_t target;                                                          \
        HSM_STRUCT(meta_data) md;                                               \
                                                                                \
        md.hash = hash_func(key);                                               \
                                                                                \
        if (map->capacity > 0) {                                                \
            error = HSM_PRIVATE(name, find_kvp)(map, &md);                      \
            target = HSM_PRIVATE(name, target)(map, md);                        \
                                                                                \
            if (error == GDC_OK && map->meta_data[target].hash == md.hash && map->meta_data[target].offset < map->max_offset) { \
                return GDC_ALREADY_PRESENT;                                     \
            }                                                                   \
        } else {                                                                \
            target = 0;                                                         \
            error = GDC_INCORRECT_CALL;                                         \
        }                                                                       \
                                                                                \
        error = HSM_PRIVATE(name, ir_kvp)(map, error, target, md, key, value);  \
                                                                                \
        return error;                                                           \
    }

#define HSMP_DELETE(name, key_type, value_type, hash_func) \
    gdc_error_t HSM_FUNC(name, delete)(HSM(name)* map, key_type key, value_type* value) { \
        size_t target;                                                          \
        HSM_STRUCT(meta_data) md;                                               \
                                                                                \
        if (map->capacity == 0) {                                               \
            return GDC_NOT_PRESENT;                                             \
        }                                                                       \
                                                                                \
        md.hash = hash_func(key);                                               \
        HSM_PRIVATE(name, find_kvp)(map, &md);                                  \
        target = HSM_PRIVATE(name, target)(map, md);                            \
                                                                                \
        if (map->meta_data[target].offset >= map->max_offset) {                 \
            return GDC_NOT_PRESENT;                                             \
        }                                                                       \
                                                                                \
        map->meta_data[target].offset = map->max_offset;                        \
        *value = map->values[target];                                           \
                                                                                \
        for (size_t i = target; i < map->capacity + map->max_offset - 1; i++) { \
            if (map->meta_data[i + 1].offset >= map->max_offset) {              \
                break;                                                          \
            }                                                                   \
                                                                                \
            map->meta_data[i] = map->meta_data[i + 1];                          \
            map->keys[i] = map->keys[i + 1];                                    \
            map->values[i] = map->values[i + 1];                                \
                                                                                \
            map->meta_data[i].offset -= 1;                                      \
        }                                                                       \
                                                                                \
        map->size -= 1;                                                         \
        return GDC_OK;                                                          \
    }

#define HSMP_SET(name, key_type, value_type, hash_func) \
    gdc_error_t HSM_FUNC(name, set)(HSM(name)* map, key_type key, value_type value) { \
        gdc_error_t error;                                                      \
        size_t target;                                                          \
        HSM_STRUCT(meta_data) md;                                               \
                                                                                \
        md.hash = hash_func(key);                                               \
                                                                                \
        if (map->capacity > 0) {                                                \
            error = HSM_PRIVATE(name, find_kvp)(map, &md);                      \
            target = HSM_PRIVATE(name, target)(map, md);                        \
                                                                                \
            if (error == GDC_OK && map->meta_data[target].hash == md.hash && map->meta_data[target].offset < map->max_offset) { \
                map->values[target] = value;                                    \
                return GDC_OK;                                                  \
            }                                                                   \
        } else {                                                                \
            target = 0;                                                         \
            error = GDC_INCORRECT_CALL;                                         \
        }                                                                       \
                                                                                \
        error = HSM_PRIVATE(name, ir_kvp)(map, error, target, md, key, value);  \
                                                                                \
        return error;                                                           \
    }

#define HSMP_GET(name, key_type, value_type, hash_func) \
    gdc_error_t HSM_FUNC(name, get)(HSM(name)* map, key_type key, value_type* value) { \
        gdc_error_t error;                                                      \
        size_t target;                                                          \
        HSM_STRUCT(meta_data) md;                                               \
                                                                                \
        if (map->size <= 0) {                                                   \
            return GDC_NOT_PRESENT;                                             \
        }                                                                       \
                                                                                \
        md.hash = hash_func(key);                                               \
        error = HSM_PRIVATE(name, find_kvp)(map, &md);                          \
        target = HSM_PRIVATE(name, target)(map, md);                            \
                                                                                \
        if (error == GDC_OK && map->meta_data[target].hash == md.hash && map->meta_data[target].offset < map->max_offset) { \
            *value = map->values[target];                                       \
            return GDC_OK;                                                      \
        }                                                                       \
        return GDC_NOT_PRESENT;                                                 \
    }

#define HSMP_HELPER_FUNCTIONS(name, key_type, value_type) \
    size_t HSM_PRIVATE(name, compute_offset)(size_t capacity) {                 \
        size_t max_offset;                                                      \
                                                                                \
        max_offset = 0;                                                         \
        for (size_t i = 0; capacity > 0; capacity >>= 1, i++) {                 \
            max_offset = i * (capacity % 2) + max_offset / 2;                   \
        }                                                                       \
                                                                                \
        return max_offset + (max_offset == 0 && capacity != 0);                 \
    }                                                                           \
                                                                                \
    size_t HSM_PRIVATE(name, target)(HSM(name)* map, HSM_STRUCT(meta_data) md) { \
        return (md.hash % map->capacity) + md.offset;                           \
    }                                                                           \
                                                                                \
    size_t HSM_PRIVATE(name, find_kvp)(HSM(name)* map, HSM_STRUCT(meta_data) *md) { \
        size_t i, target_index;                                                 \
                                                                                \
        if (map->capacity <= 0) {                                               \
            md->offset = map->max_offset;                                       \
            return GDC_INCORRECT_CALL;                                          \
        }                                                                       \
                                                                                \
        md->offset = 0;                                                         \
        target_index = HSM_PRIVATE(name, target)(map, *md);                     \
        for (i = target_index; md->offset < map->max_offset; i++) {             \
            if (map->meta_data[i].offset < md->offset ||                        \
                map->meta_data[i].offset >= map->max_offset ||                  \
                map->meta_data[i].hash == md->hash) {                           \
                break;                                                          \
            }                                                                   \
            md->offset += 1;                                                    \
        }                                                                       \
                                                                                \
        if (md->offset < map->max_offset) {                                     \
            return GDC_OK;                                                      \
        } else {                                                                \
            return GDC_INCORRECT_CALL;                                          \
        }                                                                       \
    }                                                                           \
    void HSM_PRIVATE(name, swap_kvp)(HSM(name)* map, size_t target_index, HSM_STRUCT(meta_data)* md, key_type* key, value_type* value) { \
        HSM_STRUCT(meta_data) temp_md;                                          \
        key_type temp_key;                                                      \
        value_type temp_value;                                                  \
                                                                                \
        temp_md = map->meta_data[target_index];                                 \
        temp_key = map->keys[target_index];                                     \
        temp_value = map->values[target_index];                                 \
                                                                                \
        map->meta_data[target_index] = *md;                                     \
        map->keys[target_index] = *key;                                         \
        map->values[target_index] = *value;                                     \
                                                                                \
        *md = temp_md;                                                          \
        *key = temp_key;                                                        \
        *value = temp_value;                                                    \
    }                                                                           \
                                                                                \
    gdc_error_t HSM_PRIVATE(name, add_kvp)(HSM(name)* map, size_t target_index, HSM_STRUCT(meta_data) *md, key_type *key, value_type *value) { \
        gdc_error_t error = GDC_INCORRECT_CALL;                                 \
                                                                                \
        if (md->offset >= map->max_offset) {                                    \
            return GDC_OK;                                                      \
        }                                                                       \
                                                                                \
        for (size_t i = target_index; i < map->capacity + map->max_offset; i++) { \
                                                                                \
            HSM_PRIVATE(name, swap_kvp)(map, i, md, key, value);                \
                                                                                \
            if (md->offset >= map->max_offset) {                                \
                error = GDC_OK;                                                 \
                break;                                                          \
            }                                                                   \
                                                                                \
            md->offset += 1;                                                    \
                                                                                \
            if (md->offset >= map->max_offset) {                                \
                error = GDC_INCORRECT_CALL;                                     \
                break;                                                          \
            }                                                                   \
        }                                                                       \
                                                                                \
        return error;                                                           \
    }                                                                           \
                                                                                \
    gdc_error_t HSM_PRIVATE(name, set_capacity)(HSM(name)* map, size_t capacity) { \
        gdc_error_t error = GDC_OK;                                             \
        size_t i, total_capacity, target;                                       \
        HSM_STRUCT(meta_data) md;                                               \
        key_type key;                                                           \
        value_type value;                                                       \
        HSM(name) new_map;                                                      \
                                                                                \
        if (capacity < map->size) {                                             \
            return GDC_CAPACITY_ERROR;                                          \
        }                                                                       \
                                                                                \
        new_map.capacity = capacity;                                            \
        new_map.size = map->size;                                               \
        new_map.max_offset = HSM_PRIVATE(name, compute_offset)(capacity);       \
                                                                                \
        total_capacity = new_map.capacity + new_map.max_offset;                 \
        new_map.meta_data = malloc(total_capacity * sizeof(HSM_STRUCT(meta_data))); \
        new_map.keys = malloc(total_capacity * sizeof(key_type));               \
        new_map.values = malloc(total_capacity * sizeof(value_type));           \
                                                                                \
        if (capacity != 0 && (new_map.meta_data == NULL || new_map.keys == NULL || new_map.values == NULL)) { \
            free(new_map.meta_data);                                            \
            free(new_map.keys);                                                 \
            free(new_map.values);                                               \
            return GDC_MEMORY_ERROR;                                            \
        }                                                                       \
                                                                                \
        for (i = 0; i < new_map.capacity + new_map.max_offset; i++) {           \
            new_map.meta_data[i].offset = new_map.max_offset;                   \
        }                                                                       \
                                                                                \
        for (i = 0; i < map->capacity + map->max_offset; i++) {                 \
            if (map->meta_data[i].offset >= map->max_offset) {                  \
                continue;                                                       \
            }                                                                   \
            md.hash = map->meta_data[i].hash;                                   \
            key = map->keys[i];                                                 \
            value = map->values[i];                                             \
                                                                                \
            error = HSM_PRIVATE(name, find_kvp)(&new_map, &md);                 \
            if (error != GDC_OK) {                                              \
                break;                                                          \
            }                                                                   \
                                                                                \
            target = HSM_PRIVATE(name, target)(&new_map, md);                   \
            HSM_PRIVATE(name, swap_kvp)(&new_map, target, &md, &key, &value);   \
            if (md.offset >= map->max_offset) {                                 \
                continue;                                                       \
            }                                                                   \
                                                                                \
            target += 1; md.offset += 1;                                        \
            error = HSM_PRIVATE(name, add_kvp)(&new_map, target, &md, &key, &value); \
            if (error != GDC_OK) {                                              \
                break;                                                          \
            }                                                                   \
        }                                                                       \
                                                                                \
        if (error == GDC_OK) {                                                  \
            free(map->meta_data);                                               \
            free(map->keys);                                                    \
            free(map->values);                                                  \
            *map = new_map;                                                     \
        } else {                                                                \
            free(new_map.meta_data);                                            \
            free(new_map.keys);                                                 \
            free(new_map.values);                                               \
        }                                                                       \
                                                                                \
        return error;                                                           \
    }                                                                           \
                                                                                \
    gdc_error_t HSM_PRIVATE(name, ir_kvp)(HSM(name)* map, gdc_error_t error, size_t target, HSM_STRUCT(meta_data) md, key_type key, value_type value) { \
        size_t new_capacity = map->capacity;                                    \
                                                                                \
        if (error == GDC_OK) {                                                  \
            goto place_kvp;                                                     \
        } else if (error == GDC_INCORRECT_CALL) {                               \
            goto increase_size;                                                 \
        } else {                                                                \
            return error;                                                       \
        }                                                                       \
                                                                                \
        do {                                                                    \
            increase_size:                                                      \
            while (error == GDC_INCORRECT_CALL) {                               \
                new_capacity += new_capacity / 2 + 2 * (new_capacity < 2);      \
                error = HSM_PRIVATE(name, set_capacity)(map, new_capacity);     \
            }                                                                   \
            error = HSM_PRIVATE(name, find_kvp)(map, &md);                      \
            target = HSM_PRIVATE(name, target)(map, md);                        \
                                                                                \
            if (error == GDC_INCORRECT_CALL) {                                  \
                continue;                                                       \
            }                                                                   \
                                                                                \
            place_kvp:                                                          \
            HSM_PRIVATE(name, swap_kvp)(map, target, &md, &key, &value);        \
            if (md.offset < map->max_offset) {                                  \
                target += 1; md.offset += 1;                                    \
                error = HSM_PRIVATE(name, add_kvp)(map, target, &md, &key, &value); \
                if (error != GDC_INCORRECT_CALL) {                              \
                    break;                                                      \
                }                                                               \
            }                                                                   \
        } while (error == GDC_INCORRECT_CALL);                                  \
                                                                                \
        map->size += error == GDC_OK;                                           \
        return error;                                                           \
    }

#define HSMP_DEEP_COPY(name, key_type, value_type, copy_item_func) \
    gdc_error_t HSM_FUNC(name, deepcopy)(const HSM(name)* src, HSM(name)* dst) { \
        gdc_error_t error;                                                      \
                                                                                \
        HSM_FUNC(name, new)(dst);                                               \
        error = HSM_PRIVATE(name, set_capacity)(dst, src->capacity);            \
        if (error != GDC_OK) {                                                  \
            return error;                                                       \
        }                                                                       \
                                                                                \
        for (size_t i = 0; i < src->capacity + src->max_offset; i++) {          \
            if (src->meta_data[i].offset < src->max_offset) {                   \
                dst->meta_data[i] = src->meta_data[i];                          \
                copy_item_func(                                                 \
                    src->keys[i], src->values[i],                               \
                    &dst->keys[i], &dst->values[i]                              \
                );                                                              \
            }                                                                   \
        }                                                                       \
                                                                                \
        dst->size = src->size;                                                  \
                                                                                \
        return error;                                                           \
    }

#define HSMP_FREE_ITEMS(name, key_type, value_type, free_item_func) \
    gdc_error_t HSM_FUNC(name, free_items(HSM(name)* map)) {         \
        for (size_t i = 0; i < map->capacity + map->max_offset; i++) {          \
            if (map->meta_data[i].offset < map->max_offset) {                   \
                free_item_func(map->keys[i], map->values[i]);                   \
            }                                                                   \
            map->meta_data[i].offset = map->max_offset;                         \
        }                                                                       \
        map->size = 0;                                                          \
        return GDC_OK;                                                          \
    }

#endif

