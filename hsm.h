#ifndef HSM_H
#define HSM_H

#include <stddef.h>
#include <stdint.h>

typedef enum hsm_error {
    HSM_OK,
    HSM_SMALL_CAPACITY,
    HSM_BIG_CAPACITY,
    HSM_KEY_EXISTS,
    HSM_NOT_A_KEY,
    HSMP_TOO_MANY_COLLISIONS,
} hsm_error_t;

typedef struct hsmp_meta_data {
    size_t offset;
    size_t hash;
} hsmp_meta_data_t;

#define HSMP_JOIN(a, b) HSMP_JOIN_(a, b)
#define HSMP_JOIN_(a, b) a ## b

#define HSMP_MAP(name) HSMP_JOIN(name, _hsm_t)

#define hsm_make_basic_h(name, key_type, value_type) \
    typedef struct HSMP_MAP(name) {                                             \
        size_t capacity;                                                        \
        size_t size;                                                            \
        size_t max_offset;                                                      \
        hsmp_meta_data_t* meta_data;                                            \
        key_type* keys;                                                         \
        value_type* values;                                                     \
    } HSMP_MAP(name);                                                           \
                                                                                \
    void        HSMP_JOIN(name, _hsm_new)(HSMP_MAP(name)*);                     \
    hsm_error_t HSMP_JOIN(name, _hsm_copy)(const HSMP_MAP(name)*, HSMP_MAP(name)*); \
    void        HSMP_JOIN(name, _hsm_free)(HSMP_MAP(name)*);                    \
    hsm_error_t HSMP_JOIN(name, _hsm_ensure_capacity)(HSMP_MAP(name)*, size_t); \
    hsm_error_t HSMP_JOIN(name, _hsm_fit_capacity)(HSMP_MAP(name)*);            \
                                                                                \
    hsm_error_t HSMP_JOIN(name, _hsm_insert)(HSMP_MAP(name)*, key_type, value_type); \
    hsm_error_t HSMP_JOIN(name, _hsm_delete)(HSMP_MAP(name)*, key_type, value_type*); \
                                                                                \
    hsm_error_t HSMP_JOIN(name, _hsm_set)(HSMP_MAP(name)*, key_type, value_type); \
    hsm_error_t HSMP_JOIN(name, _hsm_get)(HSMP_MAP(name)*, key_type, value_type*); \

#define hsm_make_deepcopy_h(name, key_type, value_type) \
    hsm_error_t HSMP_JOIN(name, _hsm_deepcopy)(const HSMP_MAP(name)*, HSMP_MAP(name)*); \

#define hsm_make_free_h(name, key_type, value_type) \
    hsm_error_t HSMP_JOIN(name, _hsm_free_items)(HSMP_MAP(name)*);              \

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
    void HSMP_JOIN(name, _hsm_new)(HSMP_MAP(name)* map) {                       \
        map->capacity = 0;                                                      \
        map->size = 0;                                                          \
        map->max_offset = 0;                                                    \
        map->meta_data = NULL;                                                  \
        map->keys = NULL;                                                       \
        map->values = NULL;                                                     \
    }

#define HSMP_COPY(name, key_type, value_type, hash_func) \
    hsm_error_t HSMP_JOIN(name, _hsm_copy)(const HSMP_MAP(name)* src, HSMP_MAP(name)* dst) { \
        hsm_error_t error;                                                      \
        HSMP_JOIN(name, _hsm_new)(dst);                                         \
        error = HSMP_JOIN(hsmp_set_capacity_, name)(dst, src->capacity);        \
                                                                                \
        if (error != HSM_OK) {                                                  \
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
        return HSM_OK;                                                          \
    }

#define HSMP_FREE(name, key_type, value_type, hash_func) \
    void HSMP_JOIN(name, _hsm_free)(HSMP_MAP(name)* map) {                      \
        map->capacity = 0;                                                      \
        map->size = 0;                                                          \
        map->max_offset = 0;                                                    \
        free(map->meta_data);                                                   \
        free(map->keys);                                                        \
        free(map->values);                                                      \
    }

#define HSMP_ENSURE_CAPACITY(name, key_type, value_type, hash_func) \
    hsm_error_t HSMP_JOIN(name, _hsm_ensure_capacity)(HSMP_MAP(name)* map, size_t capacity) {\
        hsm_error_t error = HSMP_TOO_MANY_COLLISIONS;                           \
        size_t test_capacity = capacity;                                        \
                                                                                \
        while (error == HSMP_TOO_MANY_COLLISIONS) {                             \
            error = HSMP_JOIN(hsmp_set_capacity_, name)(map, test_capacity);    \
            if (error == HSM_BIG_CAPACITY) {                                    \
                break;                                                          \
            }                                                                   \
            test_capacity += test_capacity / 2 + 2 * (test_capacity < 2);       \
        }                                                                       \
                                                                                \
        return error;                                                           \
    }

#define HSMP_FIT_CAPACITY(name, key_type, value_type, hash_func) \
    hsm_error_t HSMP_JOIN(name, _hsm_fit_capacity)(HSMP_MAP(name)* map) {       \
        return HSMP_JOIN(name, _hsm_ensure_capacity)(map, map->size);           \
    }

#define HSMP_INSERT(name, key_type, value_type, hash_func) \
    hsm_error_t HSMP_JOIN(name, _hsm_insert)(HSMP_MAP(name)* map, key_type key, value_type value) { \
        hsm_error_t error;                                                      \
        size_t target;                                                          \
        hsmp_meta_data_t md;                                                    \
                                                                                \
        md.hash = hash_func(key);                                               \
                                                                                \
        if (map->capacity > 0) {                                                \
            error = HSMP_JOIN(hsmp_find_kvp_, name)(map, &md);                  \
            target = HSMP_JOIN(hsmp_target_, name)(map, md);                    \
                                                                                \
            if (error == HSM_OK && map->meta_data[target].hash == md.hash && map->meta_data[target].offset < map->max_offset) { \
                return HSM_KEY_EXISTS;                                          \
            }                                                                   \
        } else {                                                                \
            target = 0;                                                         \
            error = HSMP_TOO_MANY_COLLISIONS;                                   \
        }                                                                       \
                                                                                \
        error = HSMP_JOIN(hsmp_ir_kvp_, name)(map, error, target, md, key, value); \
                                                                                \
        return error;                                                           \
    }

#define HSMP_DELETE(name, key_type, value_type, hash_func) \
    hsm_error_t HSMP_JOIN(name, _hsm_delete)(HSMP_MAP(name)* map, key_type key, value_type* value) { \
        size_t target;                                                          \
        hsmp_meta_data_t md;                                                    \
                                                                                \
        if (map->capacity == 0) {                                               \
            return HSM_NOT_A_KEY;                                               \
        }                                                                       \
                                                                                \
        md.hash = hash_func(key);                                               \
        HSMP_JOIN(hsmp_find_kvp_, name)(map, &md);                              \
        target = HSMP_JOIN(hsmp_target_, name)(map, md);                        \
                                                                                \
        if (map->meta_data[target].offset >= map->max_offset) {                 \
            return HSM_NOT_A_KEY;                                               \
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
        return HSM_OK;                                                          \
    }

#define HSMP_SET(name, key_type, value_type, hash_func) \
    hsm_error_t HSMP_JOIN(name, _hsm_set)(HSMP_MAP(name)* map, key_type key, value_type value) { \
        hsm_error_t error;                                                      \
        size_t target;                                                          \
        hsmp_meta_data_t md;                                                    \
                                                                                \
        md.hash = hash_func(key);                                               \
                                                                                \
        if (map->capacity > 0) {                                                \
            error = HSMP_JOIN(hsmp_find_kvp_, name)(map, &md);                  \
            target = HSMP_JOIN(hsmp_target_, name)(map, md);                    \
                                                                                \
            if (error == HSM_OK && map->meta_data[target].hash == md.hash && map->meta_data[target].offset < map->max_offset) { \
                map->values[target] = value;                                    \
                return HSM_OK;                                                  \
            }                                                                   \
        } else {                                                                \
            target = 0;                                                         \
            error = HSMP_TOO_MANY_COLLISIONS;                                   \
        }                                                                       \
                                                                                \
        error = HSMP_JOIN(hsmp_ir_kvp_, name)(map, error, target, md, key, value); \
                                                                                \
        return error;                                                           \
    }

#define HSMP_GET(name, key_type, value_type, hash_func) \
    hsm_error_t HSMP_JOIN(name, _hsm_get)(HSMP_MAP(name)* map, key_type key, value_type* value) { \
        hsm_error_t error;                                                      \
        size_t target;                                                          \
        hsmp_meta_data_t md;                                                    \
                                                                                \
        if (map->size <= 0) {                                                   \
            return HSM_NOT_A_KEY;                                               \
        }                                                                       \
                                                                                \
        md.hash = hash_func(key);                                               \
        error = HSMP_JOIN(hsmp_find_kvp_, name)(map, &md);                      \
        target = HSMP_JOIN(hsmp_target_, name)(map, md);                        \
                                                                                \
        if (error == HSM_OK && map->meta_data[target].hash == md.hash && map->meta_data[target].offset < map->max_offset) { \
            *value = map->values[target];                                       \
            return HSM_OK;                                                      \
        }                                                                       \
        return HSM_NOT_A_KEY;                                                   \
    }

#define HSMP_HELPER_FUNCTIONS(name, key_type, value_type) \
    size_t HSMP_JOIN(hsmp_compute_offset_, name)(size_t capacity) {             \
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
    size_t HSMP_JOIN(hsmp_target_, name)(HSMP_MAP(name)* map, hsmp_meta_data_t md) { \
        return (md.hash % map->capacity) + md.offset;                           \
    }                                                                           \
                                                                                \
    size_t HSMP_JOIN(hsmp_find_kvp_, name)(HSMP_MAP(name)* map, hsmp_meta_data_t *md) { \
        size_t i, target_index;                                                 \
                                                                                \
        if (map->capacity <= 0) {                                               \
            md->offset = map->max_offset;                                       \
            return HSMP_TOO_MANY_COLLISIONS;                                    \
        }                                                                       \
                                                                                \
        md->offset = 0;                                                         \
        target_index = HSMP_JOIN(hsmp_target_, name)(map, *md);                 \
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
            return HSM_OK;                                                      \
        } else {                                                                \
            return HSMP_TOO_MANY_COLLISIONS;                                    \
        }                                                                       \
    }                                                                           \
    void HSMP_JOIN(hsmp_swap_kvp_, name)(HSMP_MAP(name)* map, size_t target_index, hsmp_meta_data_t* md, key_type* key, value_type* value) { \
        hsmp_meta_data_t temp_md;                                               \
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
    hsm_error_t HSMP_JOIN(hsmp_add_kvp_, name)(HSMP_MAP(name)* map, size_t target_index, hsmp_meta_data_t *md, key_type *key, value_type *value) { \
        hsm_error_t error = HSMP_TOO_MANY_COLLISIONS;                           \
                                                                                \
        if (md->offset >= map->max_offset) {                                    \
            return HSM_OK;                                                      \
        }                                                                       \
                                                                                \
        for (size_t i = target_index; i < map->capacity + map->max_offset; i++) { \
                                                                                \
            HSMP_JOIN(hsmp_swap_kvp_, name)(map, i, md, key, value);            \
                                                                                \
            if (md->offset >= map->max_offset) {                                \
                error = HSM_OK;                                                 \
                break;                                                          \
            }                                                                   \
                                                                                \
            md->offset += 1;                                                    \
                                                                                \
            if (md->offset >= map->max_offset) {                                \
                error = HSMP_TOO_MANY_COLLISIONS;                               \
                break;                                                          \
            }                                                                   \
        }                                                                       \
                                                                                \
        return error;                                                           \
    }                                                                           \
                                                                                \
    hsm_error_t HSMP_JOIN(hsmp_set_capacity_, name)(HSMP_MAP(name)* map, size_t capacity) { \
        hsm_error_t error = HSM_OK;                                             \
        size_t i, total_capacity, target;                                       \
        hsmp_meta_data_t md;                                                    \
        key_type key;                                                           \
        value_type value;                                                       \
        HSMP_MAP(name) new_map;                                                 \
                                                                                \
        if (capacity < map->size) {                                             \
            return HSM_SMALL_CAPACITY;                                          \
        }                                                                       \
                                                                                \
        new_map.capacity = capacity;                                            \
        new_map.size = map->size;                                               \
        new_map.max_offset = HSMP_JOIN(hsmp_compute_offset_, name)(capacity);   \
                                                                                \
        total_capacity = new_map.capacity + new_map.max_offset;                 \
        new_map.meta_data = malloc(total_capacity * sizeof(hsmp_meta_data_t));  \
        new_map.keys = malloc(total_capacity * sizeof(key_type));               \
        new_map.values = malloc(total_capacity * sizeof(value_type));           \
                                                                                \
        if (capacity != 0 && (new_map.meta_data == NULL || new_map.keys == NULL || new_map.values == NULL)) { \
            free(new_map.meta_data);                                            \
            free(new_map.keys);                                                 \
            free(new_map.values);                                               \
            return HSM_BIG_CAPACITY;                                            \
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
            error = HSMP_JOIN(hsmp_find_kvp_, name)(&new_map, &md);             \
            if (error != HSM_OK) {                                              \
                break;                                                          \
            }                                                                   \
                                                                                \
            target = HSMP_JOIN(hsmp_target_, name)(&new_map, md);               \
            HSMP_JOIN(hsmp_swap_kvp_, name)(&new_map, target, &md, &key, &value); \
            if (md.offset >= map->max_offset) {                                 \
                continue;                                                       \
            }                                                                   \
                                                                                \
            target += 1; md.offset += 1;                                        \
            error = HSMP_JOIN(hsmp_add_kvp_, name)(&new_map, target, &md, &key, &value); \
            if (error != HSM_OK) {                                              \
                break;                                                          \
            }                                                                   \
        }                                                                       \
                                                                                \
        if (error == HSM_OK) {                                                  \
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
    hsm_error_t HSMP_JOIN(hsmp_ir_kvp_, name)(HSMP_MAP(name)* map, hsm_error_t error, size_t target, hsmp_meta_data_t md, key_type key, value_type value) { \
        size_t new_capacity = map->capacity;                                    \
                                                                                \
        if (error == HSM_OK) {                                                  \
            goto place_kvp;                                                     \
        } else if (error == HSMP_TOO_MANY_COLLISIONS) {                         \
            goto increase_size;                                                 \
        } else {                                                                \
            return error;                                                       \
        }                                                                       \
                                                                                \
        do {                                                                    \
            increase_size:                                                      \
            while (error == HSMP_TOO_MANY_COLLISIONS) {                         \
                new_capacity += new_capacity / 2 + 2 * (new_capacity < 2);      \
                error = HSMP_JOIN(hsmp_set_capacity_, name)(map, new_capacity); \
            }                                                                   \
            error = HSMP_JOIN(hsmp_find_kvp_, name)(map, &md);                  \
            target = HSMP_JOIN(hsmp_target_, name)(map, md);                    \
                                                                                \
            if (error == HSMP_TOO_MANY_COLLISIONS) {                            \
                continue;                                                       \
            }                                                                   \
                                                                                \
            place_kvp:                                                          \
            HSMP_JOIN(hsmp_swap_kvp_, name)(map, target, &md, &key, &value);    \
            if (md.offset < map->max_offset) {                                  \
                target += 1; md.offset += 1;                                    \
                error = HSMP_JOIN(hsmp_add_kvp_, name)(map, target, &md, &key, &value); \
                if (error != HSMP_TOO_MANY_COLLISIONS) {                        \
                    break;                                                      \
                }                                                               \
            }                                                                   \
        } while (error == HSMP_TOO_MANY_COLLISIONS);                            \
                                                                                \
        map->size += error == HSM_OK;                                           \
        return error;                                                           \
    }

#define HSMP_DEEP_COPY(name, key_type, value_type, copy_item_func) \
    hsm_error_t HSMP_JOIN(name, _hsm_deepcopy)(const HSMP_MAP(name)* src, HSMP_MAP(name)* dst) { \
        hsm_error_t error;                                                      \
                                                                                \
        HSMP_JOIN(name, _hsm_new)(dst);                                         \
        error = HSMP_JOIN(hsmp_set_capacity_, name)(dst, src->capacity);        \
        if (error != HSM_OK) {                                                  \
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
    hsm_error_t HSMP_JOIN(name, _hsm_free_items(HSMP_MAP(name)* map)) {         \
        for (size_t i = 0; i < map->capacity + map->max_offset; i++) {          \
            map->meta_data[i].offset = map->max_offset;                         \
            if (map->meta_data[i].offset < map->max_offset) {                   \
                free_item_func(map->keys[i], map->values[i]);                   \
            }                                                                   \
        }                                                                       \
        map->size = 0;                                                          \
        return HSM_OK;                                                          \
    }

#endif

