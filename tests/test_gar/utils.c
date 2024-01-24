#include "../../cut/cut.h"
#include "../../gar.h"


#define result_ok(error, ...) \
    ASSERT_EQUAL(error, GDC_OK, __VA_ARGS__)


char* str_duplicate(const char* src) {
    size_t len = strlen(src);
    char* dst = malloc(len + 1);
    
    for (size_t i = 0; i < len + 1; i++) {
        dst[i] = src[i];
    }

    return dst;
}

SUB_TEST(make_array, int_gar_t* arr, size_t cap, size_t size) {
    int_gar_new(arr);
    ASSERT_EQUAL(int_gar_set_capacity(arr, cap), GDC_OK, "Could not set capacity to %lu.", cap);

    for (size_t i = 0; i < size; i++) {
        ASSERT_EQUAL(int_gar_push(arr, i), GDC_OK, "Could not push value: %lu.", i);
        ASSERT_EQUAL(arr->values[i], i, "%d inserted at index %lu instead of %lu.", arr->values[i], i, i);
        ASSERT_EQUAL(arr->size, i + 1, "Size is %d instead of %d.", arr->size, i + 1);
    }

    TEST_END;
}

SUB_TEST(array_eq, int_gar_t* arr, size_t size, int* values) {
    ASSERT_EQUAL(arr->size, size, "Size is %lu instead of %lu.", arr->size, size);
    for (size_t i = 0; i < size; i++) {
        ASSERT_EQUAL(arr->values[i], values[i], "index %lu has %d, expected: %d.", i, arr->values[i], values[i]);
    }
    TEST_END;
}

void print_array(int_gar_t* array) {
    size_t i = 0;
    printf("Array[capacity=%lu, size=%lu][", array->capacity, array->size);
    for_each_gar(*array, int value) {
        if (i++ > 0) {printf(", ");}
        printf("%d", value);
    }
    printf("\n");
}

