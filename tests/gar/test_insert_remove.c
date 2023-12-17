#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


UNIT_TEST(insert_value) {
    size_t cap = 10, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_insert(&arr, 2, -1), GAR_OK, "Could not insert value.");

    int res[5] = {0, 1, -1, 2, 3};
    CALL_TEST(array_eq, &arr, sizeof res / sizeof(int), res);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(insert_final_value) {
    size_t cap = 10, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_insert(&arr, size, -1), GAR_OK, "Could not insert value.");

    int res[5] = {0, 1, 2, 3, -1};
    CALL_TEST(array_eq, &arr, sizeof res / sizeof(int), res);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(remove_value) {
    size_t cap = 10, size = 4;
    int_gar_t arr;
    int v;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_remove(&arr, 1, &v), GAR_OK, "Could not remove value.");

    int res[3] = {0, 2, 3};
    CALL_TEST(array_eq, &arr, sizeof res / sizeof(int), res);
    ASSERT_EQUAL(v, 1, "Remove value is %d instead of 1.", v);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(remove_final_value) {
    size_t cap = 10, size = 4;
    int_gar_t arr;
    int v;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_remove(&arr, 3, &v), GAR_OK, "Could not remove value.");

    int res[3] = {0, 1, 2};
    CALL_TEST(array_eq, &arr, sizeof res / sizeof(int), res);
    ASSERT_EQUAL(v, 3, "Remove value is %d instead of 3.", v);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(invalid_insert) {
    size_t cap = 10, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_insert(&arr, 2 * size, -1), GAR_IDX_OOB, "Allowed to insert value at index %lu.", 2 * size);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(invalid_remove) {
    size_t cap = 10, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_remove(&arr, 2 * size, NULL), GAR_IDX_OOB, "Allowed to remove value at index %lu.", 2 * size);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(inserts) {
    size_t cap = 10, size = 6, val_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    int values[5] = {-1, -2, -3, -4, -5};
    val_size = sizeof values / sizeof(int);
    ASSERT_EQUAL(int_gar_inserts(&arr, 2, val_size, values), GAR_OK, "Could not insert values.");

    int res[11] = {0, 1, -1, -2, -3, -4, -5, 2, 3, 4, 5};
    CALL_TEST(array_eq, &arr, sizeof res / sizeof(int), res);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(removes) {
    size_t cap = 10, size = 6, index = 2, val_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    int values[3] = {0, 0, 0};
    val_size = sizeof values / sizeof(int);
    ASSERT_EQUAL(int_gar_removes(&arr, index, val_size, values), GAR_OK, "Could not remove values.");

    int res[3] = {0, 1, 5};
    CALL_TEST(array_eq, &arr, sizeof res / sizeof(int), res);
    for (int i = 0; i < val_size; i++) {
        ASSERT_EQUAL(values[i], i + index, "index %d has %d, expected: %d.", i, values[i], i + index);
    }

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(invalid_inserts) {
    size_t cap = 10, size = 6, val_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    int values[5] = {0, 0, 0, 0, 0};
    val_size = sizeof values / sizeof(int);
    ASSERT_EQUAL(int_gar_inserts(&arr, size + 1, val_size, values), GAR_IDX_OOB, "Out of bounds insert was allowed.");

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(invalid_removes) {
    size_t cap = 10, size = 6, val_size;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    int values[5] = {0, 0, 0, 0, 0};
    val_size = sizeof values / sizeof(int);
    ASSERT_EQUAL(int_gar_removes(&arr, size + 1, val_size, values), GAR_IDX_OOB, "Out of bounds remove was allowed.");
    ASSERT_EQUAL(int_gar_removes(&arr, size - 2, val_size, values), GAR_IDX_OOB, "Partially out of bounds remove was allowed.");

    int_gar_free(&arr);
    TEST_END;
}

LIST_TESTS(
    insert_value,
    insert_final_value,
    remove_value,
    remove_final_value,
    invalid_insert,
    invalid_remove,
    inserts,
    removes,
    invalid_inserts,
    invalid_removes,
)

