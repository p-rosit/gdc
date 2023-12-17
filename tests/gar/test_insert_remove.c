#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


UNIT_TEST(insert_value) {
    size_t cap = 20, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_insert(&arr, 2, -1), GAR_OK, "Could not insert value.");

    ASSERT_EQUAL(arr.size, size + 1, "Size is %lu instead of %lu.", arr.size, size + 1);
    ASSERT_EQUAL(arr.values[0], 0, "%d inserted at index 0 instead of 0.", arr.values[0]);
    ASSERT_EQUAL(arr.values[1], 1, "%d inserted at index 1 instead of 1.", arr.values[1]);
    ASSERT_EQUAL(arr.values[2], -1, "%d inserted at index 2 instead of -1.", arr.values[2]);
    ASSERT_EQUAL(arr.values[3], 2, "%d inserted at index 3 instead of 2.", arr.values[3]);
    ASSERT_EQUAL(arr.values[4], 3, "%d inserted at index 4 instead of 3.", arr.values[4]);

    ASSERT_EQUAL(int_gar_insert(&arr, size + 1, -1), GAR_OK, "Could not insert value.");
    ASSERT_EQUAL(arr.values[size + 1], -1, "Value at last index is %d insteadd of -1.", arr.values[size + 1]);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(remove_value) {
    size_t cap = 20, size = 4, iters = 0;
    int_gar_t arr;
    int v;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_remove(&arr, 1, &v), GAR_OK, "Could not remove value.");

    ASSERT_EQUAL(arr.size, size - 1, "Size is %lu instead of %lu.", arr.size, size - 1);
    ASSERT_EQUAL(arr.values[0], 0, "%d inserted at index 0 instead of 0.", arr.values[0]);
    ASSERT_EQUAL(arr.values[1], 2, "%d inserted at index 1 instead of 2.", arr.values[1]);
    ASSERT_EQUAL(arr.values[2], 3, "%d inserted at index 2 instead of 3.", arr.values[2]);
    ASSERT_EQUAL(v, 1, "Remove value is %d instead of 1.", v);

    ASSERT_EQUAL(int_gar_remove(&arr, size - 1, NULL), GAR_OK, "Could not remove value.");
    ASSERT_EQUAL(arr.size, size - 2, "Size is %lu instead of %lu.", arr.size, size - 2);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(invalid_insert) {
    size_t cap = 20, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_insert(&arr, 2 * size, -1), GAR_IDX_OOB, "Allowed to insert value at index %lu.", 2 * size);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(invalid_remove) {
    size_t cap = 20, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_remove(&arr, 2 * size, NULL), GAR_IDX_OOB, "Allowed to remove value at index %lu.", 2 * size);

    int_gar_free(&arr);
    TEST_END;
}


LIST_TESTS(
    insert_value,
    remove_value,
    invalid_insert,
    invalid_remove,
)

