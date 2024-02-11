#include "../../../../cut/cut.h"
#include "../hsm.h"
#include "utils.c"


UNIT_TEST(update) {
    size_t cap, size1, size2, i;
    s2i_hsm_t a, b;
    char k1[] = {'a', 'b', 'c'}, k2[] = {'g', 'h'};
    size1 = sizeof(k1) / sizeof(*k1);
    size2 = sizeof(k2) / sizeof(*k2);

    CALL_TEST(make_map, &a, 10);
    CALL_TEST(make_map, &b, 5);
    
    for (i = 0; i < size1; i++) {
        result_ok(s2i_hsm_insert(&a, &k1[i], i), "Could not insert kvp {\"%c\": %d}.", k1[i], i);
    }
    for (i = 0; i < size2; i++) {
        result_ok(s2i_hsm_insert(&b, &k2[i], i), "Could not insert kvp {\"%c\": %d}.", k2[i], i);
    }

    cap = b.capacity;
    result_ok(s2i_hsm_update(&a, &b), "Could not update map.");

    ASSERT_EQUAL(b.size, 0, "Size is %lu, map was not consumed.", b.size);
    ASSERT_EQUAL(b.capacity, cap, "Capacity is %lu, did not stay at %lu.", b.capacity, cap);
    
    char k[] = {'a', 'b', 'c', 'g', 'h'};
    int v[] =  {  0,   1,   2,   0,   1};
    size_t size = sizeof(k) / sizeof(*k);
    CALL_TEST(check_map, &a, size, k, v);

    s2i_hsm_free(&a);
    s2i_hsm_free(&b);
    TEST_END;
}

UNIT_TEST(update_empty) {
    size_t a_cap, b_cap;
    s2i_hsm_t a, b;

    a = s2i_hsm_new();
    b = s2i_hsm_new();

    a_cap = a.capacity;
    b_cap = b.capacity;
    result_ok(s2i_hsm_update(&a, &b), "Could not update map.");

    ASSERT_EQUAL(a.size, 0, "Size was changed to %lu.", a.size);
    ASSERT_EQUAL(a.capacity, a_cap, "Capacity is %lu, did not stay at %lu.", a.capacity, a_cap);
    ASSERT_EQUAL(b.size, 0, "Size is %lu, map was not consumed.", b.size);
    ASSERT_EQUAL(b.capacity, b_cap, "Capacity is %lu, did not stay at %lu.", b.capacity, b_cap);

    s2i_hsm_free(&a);
    s2i_hsm_free(&b);
    TEST_END;
}

UNIT_TEST(update_on_empty) {
    size_t cap, size2, i;
    s2i_hsm_t a, b;
    char k2[] = {'g', 'h'};
    size2 = sizeof(k2) / sizeof(*k2);

    a = s2i_hsm_new();
    CALL_TEST(make_map, &b, 5);
    
    for (i = 0; i < size2; i++) {
        result_ok(s2i_hsm_insert(&b, &k2[i], i), "Could not insert kvp {\"%c\": %d}.", k2[i], i);
    }

    cap = b.capacity;
    result_ok(s2i_hsm_update(&a, &b), "Could not update map.");

    ASSERT_EQUAL(b.size, 0, "Size is %lu, map was not consumed.", b.size);
    ASSERT_EQUAL(b.capacity, cap, "Capacity is %lu, did not stay at %lu.", b.capacity, cap);
    
    char k[] = {'g', 'h'};
    int v[] =  {  0,   1};
    size_t size = sizeof(k) / sizeof(*k);
    CALL_TEST(check_map, &a, size, k, v);

    s2i_hsm_free(&a);
    s2i_hsm_free(&b);
    TEST_END;
}

UNIT_TEST(update_with_empty) {
    size_t cap, size1, i;
    s2i_hsm_t a, b;
    char k1[] = {'a', 'b', 'c'};
    size1 = sizeof(k1) / sizeof(*k1);

    CALL_TEST(make_map, &a, 10);
    b = s2i_hsm_new();
    
    for (i = 0; i < size1; i++) {
        result_ok(s2i_hsm_insert(&a, &k1[i], i), "Could not insert kvp {\"%c\": %d}.", k1[i], i);
    }

    cap = b.capacity;
    result_ok(s2i_hsm_update(&a, &b), "Could not update map.");

    ASSERT_EQUAL(b.size, 0, "Size is %lu, map was not consumed.", b.size);
    ASSERT_EQUAL(b.capacity, cap, "Capacity is %lu, did not stay at %lu.", b.capacity, cap);
    
    char k[] = {'a', 'b', 'c'};
    int v[] =  {  0,   1,   2};
    size_t size = sizeof(k) / sizeof(*k);
    CALL_TEST(check_map, &a, size, k, v);

    s2i_hsm_free(&a);
    s2i_hsm_free(&b);
    TEST_END;
}


LIST_TESTS(
    update,
    update_empty,
    update_on_empty,
    update_with_empty,
)
