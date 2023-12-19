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

    TEST_END;
}


UNIT_TEST(sort_simple) {
    size_t cap = 20, v_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 0);
    int vals[] = {0, 9, 1, 8, 2, 7};
    v_size = sizeof vals / sizeof(int);
    for (size_t i = 0; i < v_size; i++) {int_gar_push(&arr, vals[i]);}

    int_gar_sort(&arr, int_ord);

    int res[] = {0, 1, 2, 7, 8, 9};
    v_size = sizeof res / sizeof(int);
    CALL_TEST(array_eq, &arr, v_size, res);

    TEST_END;
}


UNIT_TEST(sort_duplicates) {
    size_t cap = 20, v_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 0);
    int vals[] = {0, 3, 4, 3, 6, 7, 0};
    v_size = sizeof vals / sizeof(int);
    for (size_t i = 0; i < v_size; i++) {int_gar_push(&arr, vals[i]);}
    
    int_gar_sort(&arr, int_ord);

    int res[] = {0, 0, 3, 3, 4, 6, 7};
    v_size = sizeof res / sizeof(int);
    CALL_TEST(array_eq, &arr, v_size, res);

    TEST_END;
}


typedef struct test_sort {
    int data;
    int index;
} test_sort_t;

int test_ord(test_sort_t a, test_sort_t b) {
    return a.data - b.data;
}

gar_make_basic_h(test, test_sort_t);
gar_make_basic(test, test_sort_t)

UNIT_TEST(sort_stable) {
    TEST_BROKEN;
    size_t cap = 10, v_size;
    test_sort_t a, b;
    test_gar_t arr;

    test_gar_new(&arr);
    ASSERT_EQUAL(test_gar_set_capacity(&arr, cap), GAR_OK, "Could not initialize array with capacity %lu.", cap);

    test_sort_t vals[] = {
        {.data=0, .index=0},
        {.data=1, .index=2},
        {.data=3, .index=4},
        {.data=3, .index=5},
        {.data=2, .index=3},
        {.data=0, .index=1},
    }; v_size = sizeof vals / sizeof(test_sort_t);
    for (size_t i = 0; i < v_size; i++) {
        ASSERT_EQUAL(test_gar_push(&arr, vals[i]), GAR_OK, "Could not push data to index %lu.", i);
    }

    test_gar_sort(&arr, test_ord);

    test_sort_t res[] = {
        {.data=0, .index=0},
        {.data=0, .index=1},
        {.data=1, .index=2},
        {.data=2, .index=3},
        {.data=3, .index=4},
        {.data=3, .index=5},
    }; v_size = sizeof res / sizeof(test_sort_t);
    ASSERT_EQUAL(arr.size, v_size, "Size is %lu instead of %lu.", arr.size, v_size);
    ASSERT_EQUAL(arr.capacity, cap, "Capacity is %lu instead of %lu.", arr.capacity, cap);
    // printf("\n:");
    // for (size_t i = 0; i < v_size; i++) {
    //     a = arr.values[i]; b = res[i];
    //     printf("(%d, %d), ", a.data, a.index);
    //     // ASSERT_TRUE(a.data == b.data && a.index == b.index, "Data at index %lu is (%d, %d) instead of (%d, %d).", a.data, a.index, b.data, b.index);
    // }
    // printf("\n");

    TEST_END;
}


LIST_TESTS(
    sort_empty,
    sort_singleton,
    sort_simple,
    sort_duplicates,
    sort_stable,
)

