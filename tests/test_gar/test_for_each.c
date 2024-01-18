#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


UNIT_TEST(for_each) {
    size_t cap = 5, size = 5;
    int iters, value;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    iters = 0;
    for_each_gar(arr, int temp) {
        ASSERT_EQUAL(temp, iters, "Got %d instead of %d.", temp, iters);
        iters++;
    }
    ASSERT_EQUAL(iters, size, "Iterated over %d values instead of %d.", iters, size);

    iters = 0;
    for_each_gar(arr, value) {
        ASSERT_EQUAL(value, iters, "Got %d instead of %d.", value, iters);
        iters++;
    }
    ASSERT_EQUAL(iters, size, "Iterated over %d values instead of %d.", iters, size);
    ASSERT_EQUAL(value, size - 1, "Last value is %d instead of %d.", value, size - 1);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(for_each_ptr) {
    size_t cap = 5, size = 5;
    int iters, *value;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    iters = 0;
    for_each_ptr_gar(arr, int* temp) {
        ASSERT_EQUAL(*temp, iters, "Got %d instead of %d.", temp, iters);
        iters++;
    }
    ASSERT_EQUAL(iters, size, "Iterated over %d values instead of %d.", iters, size);

    iters = 0;
    for_each_ptr_gar(arr, value) {
        ASSERT_EQUAL(*value, iters, "Got %d instead of %d.", value, iters);
        iters++;
    }
    ASSERT_EQUAL(iters, size, "Iterated over %d values instead of %d.", iters, size);
    ASSERT_EQUAL(*value, size - 1, "Last value is %d instead of %d.", value, size - 1);

    for_each_ptr_gar(arr, int* val) {*val = 0;}
    for_each_ptr_gar(arr, int* val) {
        ASSERT_EQUAL(*val, 0, "Value is %d, not mutable through loop.", *val);
    }

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(for_each_statement) {
    int_gar_t arr;

    CALL_TEST(make_array, &arr, 2, 1);

    if (0)
        for_each_gar(arr, int temp)
            TEST_FAIL("Unreachable statement executed (%d).", temp);

    if (0)
        for_each_ptr_gar(arr, int* temp)
            TEST_FAIL("Unreachable statement executed. (%p)", temp);

    int_gar_free(&arr);
    TEST_END;
}


LIST_TESTS(
    for_each,
    for_each_ptr,
    for_each_statement,
)

