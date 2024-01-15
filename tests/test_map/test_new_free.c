#include "../../cut/cut.h"

#include "../../map.h"

#include "utils.c"

UNIT_TEST(new_map) {
    size_t cap = 5;
    s2i_hsm_t map;

    s2i_hsm_new(&map);
    ASSERT_EQUAL(map.capacity, 0, "Capacity is %lu instead of 0.", map.capacity);
    ASSERT_EQUAL(map.size, 0, "Size is %lu instead of 0.", map.size);
    ASSERT_EQUAL(map.max_offset, 0, "Max offset is %lu instead of 0.", map.max_offset);

    s2i_hsm_ensure_capacity(&map, cap);
    ASSERT_TRUE(map.capacity >= cap, "Capacity is %lu, smaller than %lu.", map.capacity, cap);
    ASSERT_EQUAL(map.size, 0, "Size is %lu instead of 0.", map.size);

    s2i_hsm_free(&map);
    ASSERT_EQUAL(map.capacity, 0, "Capacity is %lu instead of 0.", map.capacity);
    ASSERT_EQUAL(map.size, 0, "Size is %lu instead of 0.", map.size);
    ASSERT_EQUAL(map.max_offset, 0, "Max offset is %lu instead of 0.", map.max_offset);

    TEST_END;
}

UNIT_TEST(free_empty) {
    s2i_hsm_t map;

    s2i_hsm_new(&map);
    s2i_hsm_free(&map);
    ASSERT_EQUAL(map.capacity, 0, "Capacity is %lu instead of 0.", map.capacity);
    ASSERT_EQUAL(map.size, 0, "Size is %lu instead of 0.", map.size);
    ASSERT_EQUAL(map.max_offset, 0, "Max offset is %lu instead of 0.", map.max_offset);

    TEST_END;
}

UNIT_TEST(free_kvp) {
    size_t cap, offset, size = 3;
    s2i_hsm_t map;
    char k1[] = "key1";
    char k2[] = "key2";
    char k3[] = "key3";
    char *keys[3] = {k1, k2, k3};

    s2i_hsm_new(&map);
    s2i_hsm_ensure_capacity(&map, 10);

    for (size_t i = 0; i < size; i++) {
        map.meta_data[i] = (hsmp_meta_data_t) {.offset = 0, .hash = i};
        map.keys[i] = str_duplicate(keys[i]);
        map.values[i] = i;
    }

    cap = map.capacity;
    offset = map.max_offset;
    s2i_hsm_free_items(&map);
    ASSERT_EQUAL(map.capacity, cap, "Capacity is %lu instead of %lu.", map.capacity, cap);
    ASSERT_EQUAL(map.size, 0, "Size is %lu instead of 0.", map.size);
    ASSERT_EQUAL(map.max_offset, offset, "Max offset is %lu instead of %lu.", map.max_offset, offset);

    s2i_hsm_free(&map);
    ASSERT_EQUAL(map.capacity, 0, "Capacity is %lu instead of 0.", map.capacity);
    ASSERT_EQUAL(map.size, 0, "Size is %lu instead of 0.", map.size);
    ASSERT_EQUAL(map.max_offset, 0, "Max offset is %lu instead of 0.", map.max_offset);

    TEST_END;
}

LIST_TESTS(
    new_map,
    free_empty,
    free_kvp,
)

