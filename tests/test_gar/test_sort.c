#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


int int_ord(int a, int b) {
    return a - b;
}


UNIT_TEST(sort_empty) {
    size_t cap = 20;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 0);
    int_gar_sort(&arr, int_ord);

    ASSERT_EQUAL(arr.capacity, cap, "Capacity is %lu instead of %lu.", arr.capacity, cap);
    ASSERT_EQUAL(arr.size, 0, "Size is %lu instead of 0.", arr.size);

    int_gar_free(&arr);
    TEST_END;
}


UNIT_TEST(sort_singleton) {
    size_t cap = 20;
    int val = 5;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 1);
    arr.values[0] = val;
    int_gar_sort(&arr, int_ord);

    ASSERT_EQUAL(arr.capacity, cap, "Capacity is %lu instead of %lu.", arr.capacity, cap);
    ASSERT_EQUAL(arr.size, 1, "Size is %lu instead of 1.", arr.size);
    ASSERT_EQUAL(arr.values[0], val, "Value is %d instead of %d.", arr.values[0], val);

    int_gar_free(&arr);
    TEST_END;
}


UNIT_TEST(sort_simple) {
    size_t cap = 20, v_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 0);
    int vals[] = {0, 9, 1, 8, 2, 7};
    v_size = sizeof(vals) / sizeof(int);
    for (size_t i = 0; i < v_size; i++) {int_gar_push(&arr, vals[i]);}

    int_gar_sort(&arr, int_ord);

    int res[] = {0, 1, 2, 7, 8, 9};
    v_size = sizeof(res) / sizeof(int);
    CALL_TEST(array_eq, &arr, v_size, res);

    int_gar_free(&arr);
    TEST_END;
}


UNIT_TEST(sort_duplicates) {
    size_t cap = 20, v_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 0);
    int vals[] = {0, 3, 4, 3, 6, 7, 0};
    v_size = sizeof(vals) / sizeof(int);
    for (size_t i = 0; i < v_size; i++) {int_gar_push(&arr, vals[i]);}
    
    int_gar_sort(&arr, int_ord);

    int res[] = {0, 0, 3, 3, 4, 6, 7};
    v_size = sizeof(res) / sizeof(int);
    CALL_TEST(array_eq, &arr, v_size, res);

    int_gar_free(&arr);
    TEST_END;
}


typedef struct test_sort {
    int data;
    int index;
} test_sort_t;

int test_ord(test_sort_t a, test_sort_t b) {
    return a.data - b.data;
}


LIST_TESTS(
    sort_empty,
    sort_singleton,
    sort_simple,
    sort_duplicates,
)

