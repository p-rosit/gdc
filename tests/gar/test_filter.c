#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


int is_3(int val) {
    return val == 3;
}

int is_11(int val) {
    return val == 11;
}

int more_than_5(int val) {
    return val > 5;
}

UNIT_TEST(find_unique) {
    size_t cap = 20, size = 10;
    int_gar_t arr;
    int u;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_find(&arr, is_3, &u), GAR_OK, "Could not find value.");
    ASSERT_EQUAL(u, 3, "Value is %d instead of 3.", u);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(find_duplicate) {
    size_t cap = 20, size = 10;
    int_gar_t arr;
    int u;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_find(&arr, more_than_5, &u), GAR_OK, "Could not find value.");
    ASSERT_EQUAL(u, 6, "Value is %d instead of 6.", u);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(find_no_out) {
    size_t cap = 20, size = 10;
    int_gar_t arr;
    int u;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_find(&arr, is_3, NULL), GAR_OK, "Could not find value.");

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(find_non_existing) {
    size_t cap = 10, size = 5;
    int_gar_t arr;
    int u;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_find(&arr, is_11, &u), GAR_IDX_OOB, "Found %d when searching for 11.\n", u);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(filter_array) {
    size_t cap = 20, v_size;
    int_gar_t arr, filter;

    CALL_TEST(make_array, &arr, cap, 0);

    int values[8] = {1, 9, 2, 8, 3, 7, 4, 6};
    v_size = sizeof values / sizeof(int);
    ASSERT_EQUAL(int_gar_pushes(&arr, v_size, values), GAR_OK, "Could not push values.");
    ASSERT_EQUAL(int_gar_filter(&arr, more_than_5, &filter), GAR_OK, "Could not filter array.");

    int res[4] = {9, 8, 7, 6};
    v_size = sizeof res / sizeof(int);
    CALL_TEST(array_eq, &filter, v_size, res);
    ASSERT_EQUAL(filter.capacity, arr.size, "Capacity is %lu instead of %lu.", filter.capacity, arr.size);

    int_gar_free(&arr);
    int_gar_free(&filter);
    TEST_END;
}

LIST_TESTS(
    find_unique,
    find_duplicate,
    find_no_out,
    find_non_existing,
    filter_array,
)

