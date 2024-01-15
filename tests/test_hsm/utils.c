#include <stdlib.h>
#include <string.h>

#include "../../cut/cut.h"
#include "../../hsm.h"

#define result_ok(error, ...) \
    ASSERT_EQUAL(error, HSM_OK, __VA_ARGS__)

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

hsm_make_basic(s2i, char*, int, str_hash)
hsm_make_free(s2i, char*, int, s2i_free)
hsm_make_deepcopy(s2i, char*, int, s2i_copy_item)


SUB_TEST(make_map, s2i_hsm_t* map, size_t capacity) {
    s2i_hsm_new(map);
    ASSERT_EQUAL(s2i_hsm_ensure_capacity(map, capacity), HSM_OK, "Could not allocate map with capacity %lu.", capacity);
    ASSERT_EQUAL(map->capacity, capacity, "Capacity is %lu instead of %lu.", map->capacity, capacity);
    ASSERT_EQUAL(map->size, 0, "Size is %lu instead of %lu.", map->size, 0);
    TEST_END;
}

SUB_TEST(print_map, s2i_hsm_t* map) {
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

    TEST_END;
}

SUB_TEST(check_data, s2i_hsm_t* map, hsmp_meta_data_t md, char* key, int value) {
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
    char* key;
    int value, exists;
    hsmp_meta_data_t md;

    for (index = 0; index < size; index++) {
        exists = 0;
        hash = str_hash(&keys[index]);
        key = &keys[index];
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
        md = (hsmp_meta_data_t) {.offset = target - (hash % map->capacity), .hash = hash};
        CALL_TEST(check_data, map, md, key, value);
    }

    TEST_END;
}