#include "../../cut/cut.h"

#include "../../map.h"

#include "utils.c"

const char key[] = "key";


UNIT_TEST(insert) {
    size_t cap = 10;
    s2i_hsm_t map;
    char* k = (char*) key;
    int v = 4;
    hsmp_meta_data_t md;

    CALL_TEST(make_map, &map, cap);

    ASSERT_EQUAL(s2i_hsm_insert(&map, k, v), HSM_OK, "Could not insert pair {\"%s\": %d}.", key, v);
    ASSERT_EQUAL(map.size, 1, "Size is %lu instead of 1.", map.size);

    md = (hsmp_meta_data_t) {.offset = 0, .hash = str_hash(k)};
    CALL_TEST(check_data, &map, md, k, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(delete) {
    size_t cap = 10, target;
    s2i_hsm_t map;
    char* k = (char*) key;
    int v = 4, w;

    CALL_TEST(make_map, &map, cap);

    result_ok(s2i_hsm_insert(&map, k, v), "Could not insert pair {\"%s\": %d}.", k, v);
    ASSERT_EQUAL(map.size, 1, "Size is %lu instead of 1.", map.size);
    result_ok(s2i_hsm_delete(&map, k, &w), "Could not delete value by key \"%s\".", k);
    ASSERT_EQUAL(map.size, 0, "Size is %lu instead of 0.", map.size);
    ASSERT_EQUAL(v, w, "Got %d instead of %d.", v, w);

    target = str_hash(key) % map.capacity;
    ASSERT_EQUAL(map.meta_data[target].offset, map.max_offset, "Offset is %lu instead of %lu.", map.meta_data[target].offset, map.max_offset);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(insert_twice) {
    size_t cap = 10;
    s2i_hsm_t map;
    char* k = (char*) key;
    int v = 4;
    hsmp_meta_data_t md;
    hsm_error_t error;

    CALL_TEST(make_map, &map, cap);

    ASSERT_EQUAL(s2i_hsm_insert(&map, k, v), HSM_OK, "Could not insert pair {\"%s\": %d}.", key, v);
    ASSERT_EQUAL(map.size, 1, "Size is %lu instead of 1.", map.size);

    error = s2i_hsm_insert(&map, k, v + 2);
    ASSERT_EQUAL(error, HSM_KEY_EXISTS, "Accidentally overwrote key.");

    md = (hsmp_meta_data_t) {.offset = 0, .hash = str_hash(k)};
    CALL_TEST(check_data, &map, md, k, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(delete_missing) {
    size_t cap = 10;
    s2i_hsm_t map;
    char k = 'A';
    int v;
    hsm_error_t error;

    CALL_TEST(make_map, &map, cap);
    
    error = s2i_hsm_delete(&map, &k, &v);
    ASSERT_EQUAL(error, HSM_NOT_A_KEY, "Key accidentally deleted.");

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(insert_collision) {
    size_t cap = 10, t1, t2;
    s2i_hsm_t map;
    char k1 = 110, k2 = 120;
    int v1 = 4, v2 = 8;
    hsmp_meta_data_t md1, md2;

    CALL_TEST(make_map, &map, cap);
    md1 = (hsmp_meta_data_t) {.offset = 0, .hash = k1};
    md2 = (hsmp_meta_data_t) {.offset = 0, .hash = k2};
    t1 = hsmp_target_s2i(&map, md1); t2 = hsmp_target_s2i(&map, md2);
    ASSERT_EQUAL(t1, t2, "Target indices are %lu and %lu, i.e. not equal.", t1, t2);

    result_ok(s2i_hsm_insert(&map, &k1, v1), "Could not insert pair {\"%c\": %d}.", k1, v1);
    result_ok(s2i_hsm_insert(&map, &k2, v2), "Could not insert pair {\"%c\": %d}.", k1, v1);
    ASSERT_EQUAL(map.size, 2, "Size is %lu instead of 2.", map.size);

    md1 = (hsmp_meta_data_t) {.offset = 0, .hash = k1};
    CALL_TEST(check_data, &map, md1, &k1, v1);
    md2 = (hsmp_meta_data_t) {.offset = 1, .hash = k2};
    CALL_TEST(check_data, &map, md2, &k2, v2);

    s2i_hsm_free(&map);
    TEST_END;
}


UNIT_TEST(verify_delete_move) {
    size_t cap = 10, t1, t2;
    s2i_hsm_t map;
    char k1 = 110, k2 = 120;
    int v1 = 4, v2 = 8, w;
    hsmp_meta_data_t md1, md2;

    CALL_TEST(make_map, &map, cap);
    md1 = (hsmp_meta_data_t) {.offset = 0, .hash = k1};
    md2 = (hsmp_meta_data_t) {.offset = 0, .hash = k2};
    t1 = hsmp_target_s2i(&map, md1); t2 = hsmp_target_s2i(&map, md2);
    ASSERT_EQUAL(t1, t2, "Target indices are %lu and %lu, i.e. not equal.", t1, t2);

    result_ok(s2i_hsm_insert(&map, &k1, v1), "1: Could not insert pair {\"%c\": %d}.", k1, v1);
    result_ok(s2i_hsm_insert(&map, &k2, v2), "2: Could not insert pair {\"%c\": %d}.", k2, v2);
    ASSERT_EQUAL(map.size, 2, "Size is %lu instead of 2.", map.size);
    result_ok(s2i_hsm_delete(&map, &k1, &w), "Could not delete key \"%c\".", k1);
    ASSERT_EQUAL(map.size, 1, "Size is %lu instead of 1.", map.size);
    ASSERT_EQUAL(v1, w, "Got %d instead of %d.", v1, w);

    md2 = (hsmp_meta_data_t) {.offset = 0, .hash = k2};
    CALL_TEST(check_data, &map, md2, &k2, v2);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(insert_resize) {
    size_t cap = 5, size;
    s2i_hsm_t map;
    char k[] = {5, 10, 15};
    int v[] = {0, 1, 2};

    size = sizeof(k) / sizeof(*k);
    ASSERT_EQUAL(size, sizeof(v) / sizeof(*v), "Unequal amount of keys and values.");

    CALL_TEST(make_map, &map, cap);
    ASSERT_TRUE(map.max_offset < size, "Max offset is %lu, not less than %lu.", map.max_offset, size);

    for (size_t i = 0; i < size; i++) {
        result_ok(s2i_hsm_insert(&map, &k[i], v[i]), "%lu: Could not insert pair {\"%c\": %d}.", i, k[i], v[i]);
    }
    ASSERT_EQUAL(map.size, size, "Size is %lu instead of %lu.", map.size, size);
    ASSERT_TRUE(map.capacity > cap, "Capacity is %lu, did not increase from %lu.", map.capacity, cap);

    CALL_TEST(check_map, &map, size, k, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(multiple_resize) {
    size_t cap = 5, increased_cap, size, expected_offset;
    s2i_hsm_t map;
    char k[] = {7 + 1, 21 + 1, 5, 10, 15};
    int v[] = {0, 1, 2, 3, 4};

    expected_offset = 2;
    size = sizeof(k) / sizeof(*k);
    ASSERT_EQUAL(size, sizeof(v) / sizeof(*v), "Unequal amount of keys and values.");
    ASSERT_TRUE(expected_offset < size, "Expected more than %lu elements.", expected_offset);

    CALL_TEST(make_map, &map, cap);
    ASSERT_EQUAL(
        map.max_offset, expected_offset,
        "Max offset is %lu, not less than %lu.", map.max_offset, size
    );

    for (size_t i = size - expected_offset - 1; i < size; i++) {
        result_ok(s2i_hsm_insert(&map, &k[i], v[i]), "%lu: Could not insert pair {\"%c\": %d}.", i, k[i], v[i]);
    }
    ASSERT_TRUE(map.capacity > cap, "Capacity is %lu, did not increase from %lu.", map.capacity, cap);
    ASSERT_EQUAL(map.size, expected_offset + 1, "Size is %lu instead of 3.", map.size);

    increased_cap = map.capacity;
    s2i_hsm_free(&map);
    CALL_TEST(make_map, &map, cap);

    for (size_t i = 0; i < size - 1; i++) {
        result_ok(s2i_hsm_insert(&map, &k[i], v[i]), "%lu: Could not insert pair {\"%c\": %d}.", i, k[i], v[i]);
    }
    ASSERT_EQUAL(map.capacity, cap, "Capacity is %lu instead of %lu. Bug in test or implementation.", map.capacity, cap);

    result_ok(s2i_hsm_insert(&map, &k[size-1], v[size-1]), "%lu: Could not insert pair {\"%c\": %d}.", size - 1, k[size - 1], v[size - 1]);
    ASSERT_TRUE(map.capacity > cap, "Capacity is %lu, did not increase from %lu.", map.capacity, cap);
    ASSERT_TRUE(map.capacity > increased_cap, "Capacity is %lu, did not increase a second time from %lu.", map.capacity, increased_cap);
    ASSERT_EQUAL(map.size, size, "Size is %lu instead of %lu.", map.size, size);

    CALL_TEST(check_map, &map, size, k, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(chain_to_resize) {
    size_t cap = 5, size;
    s2i_hsm_t map;
    char k[] = {10, 21, 32, 43, 53, 60};
    int v[] = {0, 1, 2, 3, 4, 5};

    size = sizeof(k) / sizeof(*k);
    ASSERT_EQUAL(size, sizeof(v) / sizeof(*v), "Unequal amount of keys and values.");

    CALL_TEST(make_map, &map, cap);

    for (size_t i = 0; i < size; i++) {
        result_ok(s2i_hsm_insert(&map, &k[i], v[i]), "%lu: Could not insert pair {\"%c\": %d}.", i, k[i], v[i]);
    }

    ASSERT_TRUE(map.capacity > cap, "Capacity is %lu, did not increase from %lu.", map.capacity, cap);
    ASSERT_EQUAL(map.size, size, "Size is %lu instead of %lu.", map.size, size);

    CALL_TEST(check_map, &map, size, k, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(resize_in_set_size) {
    size_t cap = 5, size;
    s2i_hsm_t map;
    char k[] = {17, 52, 38, 2};
    int v[] = {0, 1, 2, 3};

    size = sizeof(k) / sizeof(*k);
    ASSERT_EQUAL(size, sizeof(v) / sizeof(*v), "Unequal amount of keys and values.");

    CALL_TEST(make_map, &map, cap);

    for (size_t i = 0; i < size; i++) {
        result_ok(s2i_hsm_insert(&map, &k[i], v[i]), "%lu: Could not insert pair {\"%c\": %d}.", i, k[i], v[i]);
    }

    ASSERT_TRUE(map.capacity > cap, "Capacity is %lu, did not increase from %lu.", map.capacity, cap);
    ASSERT_TRUE(map.capacity > 7, "Capacity is %lu, not larger than 7.", map.capacity);
    ASSERT_EQUAL(map.size, size, "Size is %lu instead of %lu.", map.size, size);

    CALL_TEST(check_map, &map, size, k, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(insert_to_empty) {
    s2i_hsm_t map;
    char* k = (char*) key;
    int v = 11;

    s2i_hsm_new(&map);

    result_ok(s2i_hsm_insert(&map, k, v), "Could not insert into empty map.");

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(delete_from_empty) {
    hsm_error_t error;
    s2i_hsm_t map;
    char* k = (char*) key;
    int v;

    s2i_hsm_new(&map);

    error = s2i_hsm_delete(&map, k, &v);
    ASSERT_EQUAL(error, HSM_NOT_A_KEY, "Got nonsense value %d.", v);

    s2i_hsm_free(&map);
    TEST_END;
}

LIST_TESTS(
    insert,
    delete,
    insert_twice,
    delete_missing,
    insert_collision,
    verify_delete_move,
    insert_resize,
    multiple_resize,
    chain_to_resize,
    resize_in_set_size,
    insert_to_empty,
    delete_from_empty,
)

