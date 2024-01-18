#include "../../cut/cut.h"

#include "../../hsm.h"

#include "utils.c"


UNIT_TEST(for_each) {
    size_t cap = 50, size, i, iters;
    s2i_hsm_t map;
    char key[] = {'a', 'b', 'c', 'd', 'e', 'f'};

    size = sizeof(key) / sizeof(*key);
    int seen[size];

    CALL_TEST(make_map, &map, cap);

    for (i = 0; i < size; i++) {
        result_ok(s2i_hsm_insert(&map, &key[i], i), "%d: Could not insert kvp.", i);
    }
    ASSERT_EQUAL(map.size, size, "Size is %lu instead of %lu.", map.size, size);

    iters = 0;
    for (i = 0; i < size; i++) seen[i] = 0;
    for_each_hsm(map, char* k, int v) {
        ASSERT_TRUE((0 <= v) && (v < size), "Value is %d, not in range [0, %d).", v, size);
        ASSERT_NOT_NULL(k, "Key is null.");
        ASSERT_EQUAL(*k, key[v], "Key is %c instead of %c.", *k, key[v]);
        ASSERT_EQUAL(seen[v], 0, "Key value pair {\"%c\": %d} has been seen before.", *k, v);
        seen[v] = 1;
        iters += 1;
    }

    ASSERT_EQUAL(iters, size, "Iterated over %lu elements instead of %lu.", iters, size);
    for (i = 0; i < size; i++) {
        ASSERT_EQUAL(seen[i], 1, "Key value pair {\"%c\": %d} not seen.", key[i], i);
    }

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(for_each_ptr) {
    size_t cap = 50, size, i, iters;
    s2i_hsm_t map;
    char key[] = {'a', 'b', 'c', 'd', 'e', 'f'};

    size = sizeof(key) / sizeof(*key);
    int seen[size];

    CALL_TEST(make_map, &map, cap);

    for (i = 0; i < size; i++) {
        result_ok(s2i_hsm_insert(&map, &key[i], i), "%d: Could not insert kvp.", i);
    }
    ASSERT_EQUAL(map.size, size, "Size is %lu instead of %lu.", map.size, size);

    iters = 0;
    for (i = 0; i < size; i++) seen[i] = 0;
    for_each_ptr_hsm(map, char* k, int *v) {
        ASSERT_TRUE((0 <= *v) && (*v < size), "Value is %d, not in range [0, %d).", *v, size);
        ASSERT_NOT_NULL(k, "Key is null.");
        ASSERT_EQUAL(*k, key[*v], "Key is %c instead of %c.", *k, key[*v]);
        ASSERT_EQUAL(seen[*v], 0, "Key value pair {\"%c\": %d} has been seen before.", *k, v);
        seen[*v] = 1;
        iters += 1;
    }

    ASSERT_EQUAL(iters, size, "Iterated over %lu elements instead of %lu.", iters, size);
    for (i = 0; i < size; i++) {
        ASSERT_EQUAL(seen[i], 1, "Key value pair {\"%c\": %d} not seen.", key[i], i);
    }

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(for_each_empty) {
    size_t cap = 50;
    s2i_hsm_t map;

    CALL_TEST(make_map, &map, cap);

    for_each_hsm(map, char* k, int v) {
        TEST_FAIL("Empty map but found {\"%c\": %d}.", *k, v);
    }

    for_each_ptr_hsm(map, char* k, int *v) {
        TEST_FAIL("Empty map but found {\"%c\": %d}.", *k, *v);
    }

    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(for_each_persist) {
    size_t cap = 10;
    s2i_hsm_t map;
    char *temp_k, k = 'a';
    int temp_v, *temp_pv, v = 2;

    CALL_TEST(make_map, &map, cap);
    result_ok(s2i_hsm_insert(&map, &k, v), "Could not insert pair.");

    for_each_hsm(map, temp_k, temp_v) {
        /* pass */
    }
    ASSERT_EQUAL(temp_k, &k, "Key is \"%c\" instead of \"%c\".", *temp_k, k);
    ASSERT_EQUAL(temp_v, v, "Value is %d instead of %d.", temp_v, v);
    temp_k = NULL;

    for_each_ptr_hsm(map, temp_k, temp_pv) {
        /* pass */
    }
    ASSERT_EQUAL(temp_k, &k, "Key is \"%c\" instead of \"%c\".", *temp_k, k);
    ASSERT_EQUAL(*temp_pv, v, "Value is %d instead of %d.", *temp_pv, v);

    TEST_END;
}

UNIT_TEST(for_each_statement) {
    size_t cap = 10;
    s2i_hsm_t map;
    char k = 'a';
    int v = 2;

    CALL_TEST(make_map, &map, cap);
    result_ok(s2i_hsm_insert(&map, &k, v), "Could not insert kvp.");

    if (0)
        for_each_hsm(map, char* key, int val)
            TEST_FAIL("Unreachable statement executed {\"%c\": %d}.", *key, val);

    if (0)
        for_each_ptr_hsm(map, char* key, int *val)
            TEST_FAIL("Unreachable statement executed. {\"%c\": %d}", *key, *val);

    s2i_hsm_free(&map);
    TEST_END;
}


LIST_TESTS(
    for_each,
    for_each_ptr,
    for_each_empty,
    for_each_persist,
    for_each_statement,
)
