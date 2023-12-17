#include "../../cut/cut.h"

#include "../../gar.h"


SUB_TEST(make_array, int_gar_t* arr, size_t cap, size_t size) {
    int_gar_new(arr);
    ASSERT_EQUAL(int_gar_set_capacity(arr, cap), GAR_OK, "Could not set capacity to %lu.", cap);

    for (size_t i = 0; i < size; i++) {
        ASSERT_EQUAL(int_gar_push(arr, i), GAR_OK, "Could not push value: %lu.", i);
        ASSERT_EQUAL(arr->values[i], i, "%d inserted at index %lu instead of %lu.", arr->values[i], i, i);
        ASSERT_EQUAL(arr->size, i + 1, "Size is %d instead of %d.", arr->size, i + 1);
    }

    TEST_END;
}

