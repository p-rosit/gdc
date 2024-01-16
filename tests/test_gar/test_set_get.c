#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


UNIT_TEST(set) {
    size_t cap = 10, size = 5, v_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_set(&arr, 3, 9), GAR_OK, "Could not set value.");

    int res[] = {0, 1, 2, 9, 4};
    v_size = sizeof(res) / sizeof(int);
    CALL_TEST(array_eq, &arr, v_size, res);

    TEST_END;
}

UNIT_TEST(get) {
    size_t cap = 10, size = 5;
    int_gar_t arr;
    int u;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_get(&arr, 3, &u), GAR_OK, "Could not get value.");
    ASSERT_EQUAL(u, 3, "Value is %d instead of 3.", u);

    TEST_END;
}

UNIT_TEST(invalid_set) {
    size_t cap = 10, size = 5, v_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_set(&arr, size + 2, size - 1), GAR_IDX_OOB, "Could set index %lu in array of size %lu.", size + 2, arr.size);

    int res[] = {0, 1, 2, 3, 4};
    v_size = sizeof(res) / sizeof(int);
    CALL_TEST(array_eq, &arr, v_size, res);

    TEST_END;
    TEST_END;
}

UNIT_TEST(invalid_get) {
    size_t cap = 10, size = 5;
    int_gar_t arr;
    int u = 0;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_get(&arr, size + 2, &u), GAR_IDX_OOB, "Could get index %lu in array of size %lu. Value: %d", size + 2, arr.size);

    TEST_END;
}


LIST_TESTS(
    set,
    get,
    invalid_set,
    invalid_get,
)

