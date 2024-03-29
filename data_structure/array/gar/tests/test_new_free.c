#include "../../../../cut/cut.h"
#include "../gar.h"
#include "utils.c"


UNIT_TEST(new_array) {
    size_t cap = 5;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 0);

    ASSERT_TRUE(arr.capacity == cap, "Capacity is %lu instead of %lu.", arr.capacity, cap);
    ASSERT_TRUE(arr.size == 0, "Size is not zero, got %lu.", arr.size);

    int_gar_free(&arr);
    ASSERT_EQUAL(arr.size, 0, "Size is %lu instead of 0.", arr.size);
    TEST_END;
}

UNIT_TEST(fit_array) {
    size_t cap = 2, size = 10;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);
    result_ok(int_gar_fit_capacity(&arr), "Could not fit array size.");

    ASSERT_EQUAL(arr.capacity, arr.size, "Capacity is %lu instead of %lu.", arr.capacity, arr.size);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(copy_array) {
    size_t cap = 20, size = 5;
    int_gar_t arr, copy;

    CALL_TEST(make_array, &arr, cap, size);

    result_ok(int_gar_copy(&arr, &copy), "Could not copy array.");
    ASSERT_EQUAL(copy.capacity, arr.size, "Capacity if %lu instead of %lu.", copy.capacity, arr.capacity);
    ASSERT_EQUAL(copy.size, arr.size, "Size is %lu instead of %lu.", copy.size, arr.size);

    for (int i = 0; i < size; i++) {
        ASSERT_EQUAL(arr.values[i], copy.values[i], "%d inserted at index %d instead of %d.", copy.values[i], i, arr.values[i]);
    }

    int_gar_free(&arr);
    int_gar_free(&copy);
    TEST_END;
}

UNIT_TEST(deepcopy_str) {
    size_t cap = 10;
    string_gar_t arr, copy;

    arr = string_gar_new();
    result_ok(string_gar_set_capacity(&arr, cap), "Could not set capacity.");

    result_ok(string_gar_push(&arr, str_duplicate("A string")), "Could not push value.");
    result_ok(string_gar_push(&arr, str_duplicate("B string")), "Could not push value.");
    result_ok(string_gar_push(&arr, str_duplicate("C string")), "Could not push value.");

    result_ok(string_gar_deepcopy(&arr, &copy), "Could not copy array.");

    ASSERT_EQUAL(copy.size, arr.size, "Size of copy is %lu instead of %lu.", copy.size, arr.size);
    for (size_t i = 0; i < 3; i++) {
        ASSERT_NOT_EQUAL(arr.values[i], copy.values[i], "Strings were not deepcopied.");
        ASSERT_TRUE(strcmp(arr.values[i], copy.values[i]) == 0, "Copy contains \"%s\" instead of \"%s\".", copy.values[i], arr.values[i]);
    }

    string_gar_free(&arr);
    string_gar_free(&copy);
    TEST_END;
}

UNIT_TEST(free_values) {
    size_t cap = 10;
    string_gar_t arr;

    arr = string_gar_new();
    result_ok(string_gar_set_capacity(&arr, cap), "Could not set capacity.");

    result_ok(string_gar_push(&arr, str_duplicate("A string")), "Could not push value.");
    result_ok(string_gar_push(&arr, str_duplicate("B string")), "Could not push value.");
    result_ok(string_gar_push(&arr, str_duplicate("C string")), "Could not push value.");

    for (size_t i = 0; i < arr.size; i++) {
        free(arr.values[i]);
    }
    string_gar_clear(&arr);

    ASSERT_EQUAL(arr.capacity, cap, "Capacity is %lu instead of %lu.", arr.capacity, cap);
    ASSERT_EQUAL(arr.size, 0, "Size is %lu instead of 0.", arr.size);

    string_gar_free(&arr);
    TEST_END;
}

LIST_TESTS(
    new_array,
    fit_array,
    copy_array,
    deepcopy_str,
    free_values,
)

