#include "../../../../cut/cut.h"
#include "../gar.h"
#include "utils.c"


UNIT_TEST(push_items) {
    size_t cap = 20, size = 5;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 0);

    for (int i = 0; i < size; i++) {
        result_ok(int_gar_push(&arr, i), "Could not push value: %d.", i);
        ASSERT_EQUAL(arr.size, i + 1, "Size is %d instead of %d.", arr.size, i + 1);
    }

    for (int i = 0; i < size; i++) {
        ASSERT_EQUAL(arr.values[i], i, "%d inserted at index %d instead of %d.", arr.values[i], i, i);
    }

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(change_capacity) {
    size_t cap = 2, size = 10;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);
    ASSERT_TRUE(arr.capacity > cap, "Capacity is %lu, did not increase from %lu.", arr.capacity, cap);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(push_and_pop) {
    int u, cap = 10, size = 5;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, size);

    result_ok(int_gar_pop(&arr, &u), "Could not pop value.");
    ASSERT_EQUAL(u, size - 1, "Popped value was %d instead of 4.", u);

    result_ok(int_gar_pop(&arr, NULL), "Could not pop value.");

    result_ok(int_gar_pop(&arr, &u), "Could not pop value.");
    ASSERT_EQUAL(u, size - 3, "Popped value was %d instead of 2.", u);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(pop_empty) {
    int_gar_t arr;

    CALL_TEST(make_array, &arr, 5, 0);
    ASSERT_EQUAL(int_gar_pop(&arr, NULL), INDEX_OOB, "Array should be empty.");

    int_gar_free(&arr);
    TEST_END;
}

LIST_TESTS(
    push_items,
    change_capacity,
    push_and_pop,
    pop_empty,
)

