#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


int less_than(int val) {
    return val < 5;
}

UNIT_TEST(find) {
    size_t cap = 20, size = 10, v_size;
    int_gar_t arr, filter;
    int u;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_find(&arr, less_than, &u), GAR_OK, "Could not find value.");
    ASSERT_EQUAL(u, 0, "Value is %d instead of 0.", u);

    TEST_END;
}

UNIT_TEST(filter_array) {
    size_t cap = 20, v_size;
    int_gar_t arr, filter;

    CALL_TEST(make_array, &arr, cap, 0);

    int values[8] = {1, 9, 2, 8, 3, 7, 4, 6};
    v_size = sizeof values / sizeof(int);
    ASSERT_EQUAL(int_gar_pushes(&arr, v_size, values), GAR_OK, "Could not push values.");
    ASSERT_EQUAL(int_gar_filter(&arr, less_than, &filter), GAR_OK, "Could not filter array.");

    int res[4] = {1, 2, 3, 4};
    v_size = sizeof res / sizeof(int);
    CALL_TEST(array_eq, &filter, v_size, res);
    ASSERT_EQUAL(filter.capacity, arr.size, "Capacity is %lu instead of %lu.", filter.capacity, arr.size);

    int_gar_free(&arr);
    int_gar_free(&filter);
    TEST_END;
}

LIST_TESTS(
    find,
    filter_array,
)

