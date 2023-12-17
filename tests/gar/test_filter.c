#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


int less_than(int val) {
    return val < 5;
}

UNIT_TEST(filter_array) {
    size_t cap = 20, size = 4;
    int_gar_t arr, filter;

    CALL_TEST(make_array, &arr, 20, 0);

    ASSERT_EQUAL(int_gar_push(&arr, 1), GAR_OK, "Could not push value");
    ASSERT_EQUAL(int_gar_push(&arr, 9), GAR_OK, "Could not push value");
    ASSERT_EQUAL(int_gar_push(&arr, 2), GAR_OK, "Could not push value");
    ASSERT_EQUAL(int_gar_push(&arr, 8), GAR_OK, "Could not push value");
    ASSERT_EQUAL(int_gar_push(&arr, 3), GAR_OK, "Could not push value");
    ASSERT_EQUAL(int_gar_push(&arr, 7), GAR_OK, "Could not push value");
    ASSERT_EQUAL(int_gar_push(&arr, 4), GAR_OK, "Could not push value");
    ASSERT_EQUAL(int_gar_push(&arr, 6), GAR_OK, "Could not push value");

    ASSERT_EQUAL(int_gar_filter(&arr, less_than, &filter), GAR_OK, "Could not filter array.");

    ASSERT_EQUAL(filter.capacity, arr.size, "Capacity is %lu instead of %lu.", filter.capacity, arr.size);
    ASSERT_EQUAL(filter.size, size, "Size is %lu instead of %lu.", filter.size, size);
    for (int i = 0; i < size; i++) {
        ASSERT_EQUAL(filter.values[i], i + 1, "Index %d has %d instead of %d.", i, filter.values[i], i + 1);
    }

    int_gar_free(&arr);
    int_gar_free(&filter);
    TEST_END;
}


LIST_TESTS(
    filter_array,
)

