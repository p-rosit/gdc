#include "../../cut/cut.h"

#include "../../hsm.h"

#include "utils.c"

const char key[] = "key";


UNIT_TEST(set) {
    size_t cap = 10;
    s2i_hsm_t map;
    char* k = (char*) key;
    int v = 4;
    HSM_STRUCT(meta_data) md;

    CALL_TEST(make_map, &map, cap);

    result_ok(s2i_hsm_set(&map, k, v), "Could not insert pair {\"%s\": %d}.", key, v);
    ASSERT_EQUAL(map.size, 1, "Size is %lu instead of 1.", map.size);

    md = (HSM_STRUCT(meta_data)) {.offset = 0, .hash = str_hash(k)};
    CALL_TEST(check_data, &map, md, k, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(set_twice) {
    size_t cap = 10;
    s2i_hsm_t map;
    char* k = (char*) key;
    int v = 4;
    HSM_STRUCT(meta_data) md;

    CALL_TEST(make_map, &map, cap);

    result_ok(s2i_hsm_set(&map, k, v), "Could not insert pair {\"%s\": %d}.", key, v);
    ASSERT_EQUAL(map.size, 1, "Size is %lu instead of 1.", map.size);

    md = (HSM_STRUCT(meta_data)) {.offset = 0, .hash = str_hash(k)};
    CALL_TEST(check_data, &map, md, k, v);

    result_ok(s2i_hsm_set(&map, k, v + 1), "Could not insert pair {\"%s\": %d}.", key, v);
    ASSERT_EQUAL(map.size, 1, "Size is %lu instead of 1.", map.size);

    md = (HSM_STRUCT(meta_data)) {.offset = 0, .hash = str_hash(k)};
    CALL_TEST(check_data, &map, md, k, v + 1);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(set_to_empty) {
    s2i_hsm_t map;
    char* k = (char*) key;
    int v = 4;
    HSM_STRUCT(meta_data) md;

    s2i_hsm_new(&map);

    result_ok(s2i_hsm_set(&map, k, v), "Could not insert pair {\"%s\": %d}.", key, v);
    ASSERT_EQUAL(map.size, 1, "Size is %lu instead of 1.", map.size);
    ASSERT_TRUE(map.capacity > 0, "Capacity is %lu, not larger than 0.", map.capacity);

    md = (HSM_STRUCT(meta_data)) {.offset = 0, .hash = str_hash(k)};
    CALL_TEST(check_data, &map, md, k, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(get) {
    size_t cap = 10;
    s2i_hsm_t map;
    char* k = (char*) key;
    int v = 4, w = 5;

    CALL_TEST(make_map, &map, cap);

    result_ok(s2i_hsm_set(&map, k, v), "Could not insert pair {\"%s\": %d}.", key, v);
    ASSERT_EQUAL(map.size, 1, "Size is %lu instead of 1.", map.size);

    result_ok(s2i_hsm_get(&map, k, &w), "Could not get value from key \"%s\".", key);
    ASSERT_EQUAL(w, v, "Fetched value is %d instead of %d.", w, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(get_empty) {
    gdc_error_t error;
    size_t cap = 10;
    s2i_hsm_t map;
    char* k = (char*) key;
    int v = 0;

    CALL_TEST(make_map, &map, cap);

    error = s2i_hsm_get(&map, k, &v);
    ASSERT_EQUAL(error, GDC_NOT_PRESENT, "Got nonsene value %d.", v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(get_from_empty) {
    gdc_error_t error;
    s2i_hsm_t map;
    char *k = (char*) key;
    int v;

    CALL_TEST(make_map, &map, 0);
    error = s2i_hsm_get(&map, k, &v);

    ASSERT_EQUAL(error, GDC_NOT_PRESENT, "Got nonsense value %d.", v);

    s2i_hsm_free(&map);
    TEST_END;
}


LIST_TESTS(
    set,
    set_twice,
    set_to_empty,
    get,
    get_empty,
    get_from_empty,
)

