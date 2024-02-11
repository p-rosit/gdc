#include "../../../../cut/cut.h"
#include "../hsm.h"
#include "utils.c"


UNIT_TEST(ensure_from_empty) {
    size_t cap = 10;
    s2i_hsm_t map;

    s2i_hsm_new(&map);
    result_ok(s2i_hsm_ensure_capacity(&map, cap), "Could not make capacity %lu.", cap);
    ASSERT_EQUAL(map.capacity, cap, "Capacity is %lu instead of %lu.", map.capacity, cap);
    ASSERT_EQUAL(map.size, 0, "Size is %lu instead of %lu.", map.size, 0);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(ensure_from_filled) {
    size_t cap = 5, i, size;
    s2i_hsm_t map;
    char k[] = {10, 20, 30, 40};
    size = sizeof(k) / sizeof(*k);

    CALL_TEST(make_map, &map, cap);

    for (i = 0; i < size; i++) {
        result_ok(s2i_hsm_insert(&map, &k[i], i), "Could not insert pair {\"%c\": %d}.", &k[i], i);
    }

    cap = map.capacity + 10;
    result_ok(s2i_hsm_ensure_capacity(&map, cap), "Could not increase capacity.");

    ASSERT_TRUE(map.capacity >= cap, "Capacity is %lu, not larger than %lu.", map.capacity, cap);
    ASSERT_EQUAL(map.size, size, "Size is %lu instead of %lu.", map.size, size);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(smaller_ensure) {
    size_t cap = 5, target_cap, i, size;
    s2i_hsm_t map;
    char k[] = {10, 20, 30, 40};
    size = sizeof(k) / sizeof(*k);

    CALL_TEST(make_map, &map, cap);

    for (i = 0; i < size; i++) {
        result_ok(s2i_hsm_insert(&map, &k[i], i), "Could not insert pair {\"%c\": %d}.", &k[i], i);
    }

    cap = map.capacity;
    target_cap = map.size - 3;
    ASSERT_EQUAL(s2i_hsm_ensure_capacity(&map, target_cap), CAPACITY_ERROR, "Capacity accidentally lowered.");

    ASSERT_EQUAL(map.capacity, cap, "Capacity is %lu instead of %lu.", map.capacity, cap);
    ASSERT_EQUAL(map.size, size, "Size is %lu instead of %lu.", map.size, size);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(fit_capacity) {
    size_t cap = 50, target_cap, i, size;
    s2i_hsm_t map;
    char k[] = {10, 20, 30, 40};
    size = sizeof(k) / sizeof(*k);

    CALL_TEST(make_map, &map, cap);

    for (i = 0; i < size; i++) {
        result_ok(s2i_hsm_insert(&map, &k[i], i), "Could not insert pair {\"%c\": %d}.", &k[i], i);
    }

    target_cap = map.size;
    result_ok(s2i_hsm_fit_capacity(&map), "Could not fit capacity.");
    ASSERT_TRUE(map.capacity < cap, "Capacity is %lu, not smaller than %lu.", map.capacity, cap);
    ASSERT_TRUE(map.capacity >= target_cap, "Capacity is %lu, not larger than %lu.", map.capacity, target_cap);

    ASSERT_EQUAL(map.size, size, "Size is %lu instead of %lu.", map.size, size);

    s2i_hsm_free(&map);
    TEST_END;
}


LIST_TESTS(
    ensure_from_empty,
    ensure_from_filled,
    smaller_ensure,
    fit_capacity,
)
