#include "../../../../cut/cut.h"
#include "../gar/gar.h"
#include "utils.c"


UNIT_TEST(insert_value) {
    size_t cap = 10, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    result_ok(int_gar_insert(&arr, 2, -1), "Could not insert value.");

    int res[5] = {0, 1, -1, 2, 3};
    CALL_TEST(array_eq, &arr, sizeof(res) / sizeof(int), res);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(insert_final_value) {
    size_t cap = 10, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    result_ok(int_gar_insert(&arr, size, -1), "Could not insert value.");

    int res[5] = {0, 1, 2, 3, -1};
    CALL_TEST(array_eq, &arr, sizeof(res) / sizeof(int), res);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(remove_value) {
    size_t cap = 10, size = 4;
    int_gar_t arr;
    int v;

    CALL_TEST(make_array, &arr, cap, size);

    result_ok(int_gar_remove(&arr, 1, &v), "Could not remove value.");

    int res[3] = {0, 2, 3};
    CALL_TEST(array_eq, &arr, sizeof(res) / sizeof(int), res);
    ASSERT_EQUAL(v, 1, "Remove value is %d instead of 1.", v);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(remove_final_value) {
    size_t cap = 10, size = 4;
    int_gar_t arr;
    int v;

    CALL_TEST(make_array, &arr, cap, size);

    result_ok(int_gar_remove(&arr, 3, &v), "Could not remove value.");

    int res[3] = {0, 1, 2};
    CALL_TEST(array_eq, &arr, sizeof(res) / sizeof(int), res);
    ASSERT_EQUAL(v, 3, "Remove value is %d instead of 3.", v);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(invalid_insert) {
    size_t cap = 10, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_insert(&arr, 2 * size, -1), INDEX_OOB, "Allowed to insert value at index %lu.", 2 * size);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(invalid_remove) {
    size_t cap = 10, size = 4;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_remove(&arr, 2 * size, NULL), INDEX_OOB, "Allowed to remove value at index %lu.", 2 * size);

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
)

