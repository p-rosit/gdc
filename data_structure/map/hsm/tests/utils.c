#include <stdlib.h>
#include <string.h>

#include "../../../../cut/cut.h"
#include "../hsm.h"
#include "../hsm_serialize.h"
#include "../hsm_parse.h"

#define result_ok(error, ...) \
    ASSERT_EQUAL(error, NO_ERROR, __VA_ARGS__)

size_t str_hash(const char* str) {
    return *str;
}

void s2i_free(char* key, int value) {
    free(key);
}

char* str_duplicate(const char* src) {
    size_t len = strlen(src);
    char* dst = malloc(len + 1);

    for (size_t i = 0; i < len + 1; i++) {
        dst[i] = src[i];
    }

    return dst;
}

void s2i_copy_item(const char* str_src, const int int_src, char** str_dst, int *int_dst) {
    *str_dst = str_duplicate(str_src);
    *int_dst = int_src;
}


hsm_make_basic_h(s2i, char*, int)
hsm_make_free_h(s2i, char*, int)
hsm_make_deepcopy_h(s2i, char*, int)
hsm_make_serialize_h(s2i, char*, int)
hsm_make_parse_h(s2i, char*, int)

hsm_make_basic(s2i, char*, int, str_hash)
hsm_make_free(s2i, char*, int, s2i_free)
hsm_make_deepcopy(s2i, char*, int, s2i_copy_item)
hsm_make_serialize(s2i, char*, int, serialize_string, serialize_int)
hsm_make_parse(s2i, char*, int, parse_string, parse_int)


SUB_TEST(make_map, s2i_hsm_t* map, size_t capacity) {
    s2i_hsm_new(map);
    result_ok(s2i_hsm_ensure_capacity(map, capacity), "Could not allocate map with capacity %lu.", capacity);
    ASSERT_EQUAL(map->capacity, capacity, "Capacity is %lu instead of %lu.", map->capacity, capacity);
    ASSERT_EQUAL(map->size, 0, "Size is %lu instead of %lu.", map->size, 0);
    TEST_END;
}

void print_map(s2i_hsm_t* map) {
    printf("Map[offset=%lu, capacity=%lu, size=%lu]\n", map->max_offset, map->capacity, map->size);
    for (size_t i = 0; i < map->capacity + map->max_offset; i++) {
        printf(
            "%2lu: %ckey: %p, val: %d, hash: %lu, offset: %lu%c,\n",
            i, i < map->capacity ? '{' : '[',
            map->keys[i], map->values[i],
            map->meta_data[i].hash, map->meta_data[i].offset,
            i < map->capacity ? '}' : ']'
        );
    }
}

SUB_TEST(check_data, s2i_hsm_t* map, HSM_STRUCT(meta_data) md, char* key, int value) {
    size_t index;
    if (md.offset < map->max_offset) {
        index = hsmp_target_s2i(map, md);
        ASSERT_EQUAL(map->meta_data[index].offset, md.offset, "Offset is %lu instead of %lu.", map->meta_data[index].offset, md.offset);
    } else {
        hsmp_find_kvp_s2i(map, &md);
        index = hsmp_target_s2i(map, md);
    }
    ASSERT_EQUAL(map->meta_data[index].hash, md.hash, "Hash is %lu instead of %lu.", map->meta_data[index].hash, md.hash);
    ASSERT_EQUAL(map->keys[index], key, "Key is %p instead of %p.", map->keys[index], key);
    ASSERT_EQUAL(map->values[index], value, "Value is %d instead of %d.", map->values[index], value);
    TEST_END;
}

SUB_TEST(check_map, s2i_hsm_t* map, size_t size, char* keys, int* values) {
    size_t hash, index, target, i;
    char key;
    int value, exists;
    HSM_STRUCT(meta_data) md;

    for (index = 0; index < size; index++) {
        exists = 0;
        hash = str_hash(&keys[index]);
        key = keys[index];
        value = values[index];

        for (i = 0; i < map->capacity + map->max_offset; i++) {
            if (map->meta_data[i].offset < map->max_offset && map->meta_data[i].hash == hash) {
                if (exists) {
                    TEST_FAIL("Pair {\"%c\": %d} encountered more than once.", key, value);
                } else {
                    target = i;
                    exists = 1;
                }
            }
        }
        if (!exists) {
            TEST_FAIL("Pair {\"%c\": %d} not encountered.", key, value);
        }

        md = (HSM_STRUCT(meta_data)) {.offset = target - (hash % map->capacity), .hash = hash};
        ASSERT_EQUAL(map->meta_data[target].offset, md.offset, "Offset is %lu instead of %lu.", map->meta_data[index].offset, md.offset);
        ASSERT_EQUAL(map->meta_data[target].hash, md.hash, "Hash is %lu instead of %lu.", map->meta_data[index].hash, md.hash);
        ASSERT_EQUAL(*map->keys[target], key, "Key is \"%c\" instead of \"%c\".", map->keys[index], key);
        ASSERT_EQUAL(map->values[target], value, "Value is %d instead of %d.", map->values[index], value);
    }

    TEST_END;
}

SUB_TEST(map_eq, s2i_hsm_t* map, size_t size, char** keys, int* values) {
    error_t error;
    int value;

    ASSERT_EQUAL(map->size, size, "Size is %lu instead of %lu.", map->size, size);

    for (size_t i = 0; i < size; i++) {
        result_ok(s2i_hsm_get(map, keys[i], &value), "Map does not contain kvp {\"%s\": %d}.", keys[i], values[i]);
        ASSERT_EQUAL(value, values[i], "\"%s\" is mapped to %d instead of %d.", keys[i], value, values[i]);
    }

    TEST_END;
}
