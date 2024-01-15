#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


UNIT_TEST(new_array) {
    size_t cap = 5;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 0);

    ASSERT_TRUE(arr.capacity == cap, "Capacity is %lu instead of %lu.", arr.capacity, cap);
    ASSERT_TRUE(arr.size == 0, "Size is not zero, got %lu.", arr.size);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(fit_array) {
    size_t cap = 2, size = 10;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);
    ASSERT_EQUAL(int_gar_fit_capacity(&arr), GAR_OK, "Could not fit array size.");

    ASSERT_EQUAL(arr.capacity, arr.size, "Capacity is %lu instead of %lu.", arr.capacity, arr.size);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(copy_array) {
    size_t cap = 20, size = 5;
    int_gar_t arr, copy;

    CALL_TEST(make_array, &arr, cap, size);

    ASSERT_EQUAL(int_gar_copy(&arr, &copy), GAR_OK, "Could not copy array.");
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

    string_gar_new(&arr);
    ASSERT_EQUAL(string_gar_set_capacity(&arr, cap), GAR_OK, "Could not set capacity.");

    ASSERT_EQUAL(string_gar_push(&arr, str_duplicate("A string")), GAR_OK, "Could not push value.");
    ASSERT_EQUAL(string_gar_push(&arr, str_duplicate("B string")), GAR_OK, "Could not push value.");
    ASSERT_EQUAL(string_gar_push(&arr, str_duplicate("C string")), GAR_OK, "Could not push value.");

    ASSERT_EQUAL(string_gar_deepcopy(&arr, &copy), GAR_OK, "Could not copy array.");

    ASSERT_EQUAL(copy.size, arr.size, "Size of copy is %lu instead of %lu.", copy.size, arr.size);
    for (size_t i = 0; i < 3; i++) {
        ASSERT_NOT_EQUAL(arr.values[i], copy.values[i], "Strings were not deepcopied.");
        ASSERT_TRUE(strcmp(arr.values[i], copy.values[i]) == 0, "Copy contains \"%s\" instead of \"%s\".", copy.values[i], arr.values[i]);
    }

    string_gar_free_values(&arr);
    string_gar_free(&arr);
    string_gar_free_values(&copy);
    string_gar_free(&copy);
    TEST_END;
}

UNIT_TEST(free_values) {
    size_t cap = 10;
    string_gar_t arr;

    string_gar_new(&arr);
    ASSERT_EQUAL(string_gar_set_capacity(&arr, cap), GAR_OK, "Could not set capacity.");

    ASSERT_EQUAL(string_gar_push(&arr, str_duplicate("A string")), GAR_OK, "Could not push value.");
    ASSERT_EQUAL(string_gar_push(&arr, str_duplicate("B string")), GAR_OK, "Could not push value.");
    ASSERT_EQUAL(string_gar_push(&arr, str_duplicate("C string")), GAR_OK, "Could not push value.");

    string_gar_free_values(&arr);

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

