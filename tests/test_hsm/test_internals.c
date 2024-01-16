#include "../../cut/cut.h"
#include "../../hsm.h"

#include "utils.c"

const char key[] = "key";


UNIT_TEST(find_kvp_empty_map) {
    s2i_hsm_t map;
    HSM_STRUCT(meta_data) md;

    CALL_TEST(make_map, &map, 0);
    md.hash = str_hash(key);

    hsmp_find_kvp_s2i(&map, &md);

    ASSERT_EQUAL(
        md.offset, map.max_offset,
        "Expected offset to be maximal in empty map, got %lu insted of %lu.", md.offset, map.max_offset
    );

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(find_kvp_no_collision) {
    s2i_hsm_t map;
    HSM_STRUCT(meta_data) md;

    CALL_TEST(make_map, &map, 10);
    md.hash = str_hash(key);

    hsmp_find_kvp_s2i(&map, &md);

    ASSERT_EQUAL(md.offset, 0, "Expected offset to be zero, got %lu.", md.offset);

    s2i_hsm_free(&map);
    TEST_END;
}


UNIT_TEST(find_kvp_with_collision) {
    size_t target;
    s2i_hsm_t map;
    HSM_STRUCT(meta_data) md;

    CALL_TEST(make_map, &map, 20);
    ASSERT_TRUE(4 <= map.max_offset, "Expected max offset to be at least 4, got %lu.", map.max_offset);
    md.hash = str_hash(key);
    target = md.hash % map.capacity;

    map.meta_data[target + 0] = (HSM_STRUCT(meta_data)) {.hash = md.hash + 1, .offset = 0};
    map.meta_data[target + 1] = (HSM_STRUCT(meta_data)) {.hash = md.hash + 2, .offset = 1};

    hsmp_find_kvp_s2i(&map, &md);

    ASSERT_EQUAL(md.offset, 2, "Expected offset to be 2, got %lu.", md.offset);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(find_kvp_with_replace) {
    size_t target;
    s2i_hsm_t map;
    HSM_STRUCT(meta_data) md;

    CALL_TEST(make_map, &map, 20);
    ASSERT_TRUE(4 <= map.max_offset, "Expected max offset to be at least 4, got %lu.", map.max_offset);
    md.hash = str_hash(key);
    target = md.hash % map.capacity;

    map.meta_data[target + 0] = (HSM_STRUCT(meta_data)) {.hash = md.hash + 1, .offset = 0};
    map.meta_data[target + 1] = (HSM_STRUCT(meta_data)) {.hash = md.hash + 2, .offset = 0};

    hsmp_find_kvp_s2i(&map, &md);

    ASSERT_EQUAL(md.offset, 1, "Expected offset to be 1, got %lu.", md.offset);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(find_kvp_existing) {
    size_t target;
    s2i_hsm_t map;
    HSM_STRUCT(meta_data) md;

    CALL_TEST(make_map, &map, 20);
    ASSERT_TRUE(4 <= map.max_offset, "Expected max offset to be at least 4, got %lu.", map.max_offset);
    md.hash = str_hash(key);
    target = md.hash % map.capacity;

    map.meta_data[target + 0] = (HSM_STRUCT(meta_data)) {.hash = md.hash + 1, .offset = 0};
    map.meta_data[target + 1] = (HSM_STRUCT(meta_data)) {.hash = md.hash, .offset = 1};

    hsmp_find_kvp_s2i(&map, &md);

    ASSERT_EQUAL(md.offset, 1, "Expected offset to be 1, got %lu.", md.offset);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(swap_kvp) {
    size_t target;
    s2i_hsm_t map;
    char stri[] = "value", strj[] = "another", *key;
    int i = 5, j = 6, val;
    HSM_STRUCT(meta_data) mdi, mdj, md;

    CALL_TEST(make_map, &map, 10);

    mdi.hash = str_hash(stri);
    mdi.offset = 0;
    mdj.hash = str_hash(strj);
    mdj.offset = 0;
    target = mdi.hash % map.capacity;

    map.meta_data[target] = mdj;
    map.keys[target] = strj;
    map.values[target] = j;

    md = mdi;
    key = stri;
    val = i;
    hsmp_swap_kvp_s2i(&map, (md.hash % map.capacity) + md.offset, &md, &key, &val);

    /* j has been taken out and its target moved one step forward */
    ASSERT_EQUAL(md.offset, mdj.offset, "Offset is %lu instead of %lu.", md.offset, mdj.offset);
    ASSERT_EQUAL(md.hash, mdj.hash, "Hash is %lu instead of %lu.", md.hash, mdj.hash);
    ASSERT_EQUAL(key, strj, "String pointer is %p instead of %p.", key, strj);
    ASSERT_EQUAL(val, j, "Value is %d instead of %d.", val, j);

    /* i has been placed into the map */
    ASSERT_EQUAL(map.meta_data[target].offset, mdi.offset, "Offset is %lu instead of %lu.", md.offset, mdj.offset);
    ASSERT_EQUAL(map.meta_data[target].hash, mdi.hash, "Hash is %lu instead of %lu.");
    ASSERT_EQUAL(map.keys[target], stri, "String pointer is %p instead of %p.");
    ASSERT_EQUAL(map.values[target], i, "Value is %d instead of %d.");

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(add_kvp) {
    size_t target;
    s2i_hsm_t map;
    HSM_STRUCT(meta_data) md, expected_md;
    char *k, *expected_k;
    int v, expected_v;

    CALL_TEST(make_map, &map, 10);

    k = expected_k = (char*) key;
    v = expected_v = 1;
    md = expected_md = (HSM_STRUCT(meta_data)) {.hash = str_hash(key), .offset = 0};

    target = hsmp_target_s2i(&map, md);
    ASSERT_EQUAL(
        hsmp_add_kvp_s2i(&map, target, &md, &k, &v), GDC_OK,
        "Could not insert key value pair."
    );

    CALL_TEST(check_data, &map, md, k, v);

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(add_empty_kvp) {
    size_t target;
    s2i_hsm_t map;
    HSM_STRUCT(meta_data) md, expected_md;
    char *k, *expected_k;
    int v, expected_v;

    CALL_TEST(make_map, &map, 10);

    k = expected_k = (char*) key;
    v = expected_v = 1;
    md = (HSM_STRUCT(meta_data)) {.hash = str_hash(key) + 2, .offset = map.max_offset};
    expected_md = (HSM_STRUCT(meta_data)) {.hash = str_hash(key), .offset = 0};

    target = md.hash % map.capacity;
    map.meta_data[target] = (HSM_STRUCT(meta_data)) {.hash = str_hash(key), .offset = 0};
    map.keys[target] = expected_k;
    map.values[target] = expected_v;
    ASSERT_EQUAL(hsmp_add_kvp_s2i(&map, target, &md, &k, &v), GDC_OK,
        "Could not insert empty key value pair.");

    ASSERT_EQUAL(
        map.meta_data[target].hash, expected_md.hash,
        "Hash %lu was inserted over %lu.", map.meta_data[target].hash, expected_md.hash
    );
    ASSERT_EQUAL(
        map.meta_data[target].offset, expected_md.offset,
        "Offset %lu was inserted over %lu.", map.meta_data[target].offset, expected_md.offset
    );
    ASSERT_EQUAL(
        map.keys[target], expected_k,
        "Key %p was inserted over %p.", map.keys[target], expected_k
    );
    ASSERT_EQUAL(
        map.values[target], expected_v,
        "Value %d was inserted over %d.", map.values[target], expected_v
    );

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(add_colliding_kvp) {
    size_t target, size = 2;
    s2i_hsm_t map;
    HSM_STRUCT(meta_data) md, expected_md;
    char *k, *expected_k;
    int v, expected_v;

    CALL_TEST(make_map, &map, 20);
    ASSERT_TRUE(size + 1 <= map.max_offset, "Expected max offset to be at least %lu, got %lu.", size, map.max_offset);

    k = expected_k = (char*) key;
    v = expected_v = 1;
    md = expected_md = (HSM_STRUCT(meta_data)) {.hash = str_hash(key), .offset = 0};

    target = hsmp_target_s2i(&map, md);

    for (size_t i = 0; i < size; i++) {
        map.meta_data[target + i] = (HSM_STRUCT(meta_data)) {.hash = str_hash(key) + i + 1, .offset = i};
        map.keys[target + i] = expected_k + i + 1;
        map.values[target + i] = expected_v + i + 1;
    }

    ASSERT_EQUAL(
        hsmp_add_kvp_s2i(&map, target, &md, &k, &v), GDC_OK,
        "Could not insert key value pair."
    );

    for (size_t i = 0; i < size + 1; i++) {
        ASSERT_EQUAL(
            map.meta_data[target + i].hash, expected_md.hash + i,
            "%lu: Hash %lu was inserted instead of %lu.", i, map.meta_data[target + i].hash, expected_md.hash + i
        );
        ASSERT_EQUAL(
            map.meta_data[target + i].offset, expected_md.offset + i,
            "%lu: Offset %lu was inserted instead of %lu.", i, map.meta_data[target + i].offset, expected_md.offset + i
        );
        ASSERT_EQUAL(
            map.keys[target + i], expected_k + i,
            "%lu: Key %p was inserted instead of %p.", i, map.keys[target + i], expected_k + i
        );
        ASSERT_EQUAL(
            map.values[target + i], expected_v + i, 
            "%lu: Value %d was inserted instead of %d.", i, map.values[target + i], expected_v + i
        );
    }

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(add_kvp_after_insert) {
    size_t target, size = 5;
    s2i_hsm_t map;
    HSM_STRUCT(meta_data) md, expected_md;
    char *k, *expected_k;
    int v, expected_v;

    CALL_TEST(make_map, &map, 30);
    ASSERT_TRUE(size <= map.max_offset, "Expected max offset to be at least %lu, got %lu.", size, map.max_offset);

    k = expected_k = (char*) key;
    v = expected_v = 1;
    md = expected_md = (HSM_STRUCT(meta_data)) {.hash = str_hash(key), .offset = 0};

    target = md.hash % map.capacity;

    for (size_t i = 0; i < size; i++) {
        if (i == 2) continue;
        map.meta_data[target + i] = (HSM_STRUCT(meta_data)) {.hash = str_hash(key) + i + 1, .offset = 0};
        map.keys[target + i] = expected_k + i + 1;
        map.values[target + i] = expected_v + i + 1;
    }

    ASSERT_EQUAL(
        hsmp_add_kvp_s2i(&map, target, &md, &k, &v), GDC_OK,
        "Could not insert key value pair."
    );

    for (size_t i = 0; i < size; i++) {
        ASSERT_EQUAL(
            map.meta_data[target + i].hash, expected_md.hash + i + (i > 2),
            "%lu: Hash %lu was inserted instead of %lu.", i, map.meta_data[target + i].hash, expected_md.hash + i + (i > 2)
        );
        ASSERT_EQUAL(
            map.meta_data[target + i].offset, (i > 0) - (i > 2),
            "%lu: Offset %lu was inserted instead of %lu.", i, map.meta_data[target + i].offset, (i > 0) - (i > 2)
        );
        ASSERT_EQUAL(
            map.keys[target + i], expected_k + i + (i > 2),
            "%lu: Key %p was inserted instead of %p.", i, map.keys[target + i], expected_k + i + (i > 2)
        );
        ASSERT_EQUAL(
            map.values[target + i], expected_v + i + (i > 2), 
            "%lu: Value %d was inserted instead of %d.", i, map.values[target + i], expected_v + i + (i > 2)
        );
    }

    s2i_hsm_free(&map);
    TEST_END;
}

LIST_TESTS(
    find_kvp_empty_map,
    find_kvp_no_collision,
    find_kvp_with_collision,
    find_kvp_with_replace,
    find_kvp_existing,
    swap_kvp,
    add_kvp,
    add_empty_kvp,
    add_colliding_kvp,
    add_kvp_after_insert,
)

