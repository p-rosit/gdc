#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


UNIT_TEST(push_items) {
    size_t cap = 20, size = 5;
    int_gar_t arr;

    CALL_TEST(make_array, &arr, cap, 0);

    for (int i = 0; i < size; i++) {
        ASSERT_EQUAL(int_gar_push(&arr, i), GAR_OK, "Could not push value: %d.", i);
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

    ASSERT_EQUAL(int_gar_pop(&arr, &u), GAR_OK, "Could not pop value.");
    ASSERT_EQUAL(u, size - 1, "Popped value was %d instead of 4.", u);

    ASSERT_EQUAL(int_gar_pop(&arr, NULL), GAR_OK, "Could not pop value.");

    ASSERT_EQUAL(int_gar_pop(&arr, &u), GAR_OK, "Could not pop value.");
    ASSERT_EQUAL(u, size - 3, "Popped value was %d instead of 2.", u);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(pop_empty) {
    int_gar_t arr;

    CALL_TEST(make_array, &arr, 5, 0);
    ASSERT_EQUAL(int_gar_pop(&arr, NULL), GAR_IDX_OOB, "Array should be empty.");

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(value_pushes) {
    size_t cap = 2, size = 10;
    int_gar_t arr;
    int iters;

    int vals[size];
    for (int i = 0; i < size; i++) vals[i] = i;

    CALL_TEST(make_array, &arr, cap, 0);
    ASSERT_EQUAL(int_gar_pushes(&arr, size, vals), GAR_OK, "Could not push %d values.", size);

    iters = 0;
    for_each_gar(&arr, int val) {
        ASSERT_EQUAL(val, iters, "Got %d instead of %d.", val, iters);
        iters++;
    }
    ASSERT_EQUAL(iters, size, "Iterated over %d values instead of %d.", iters, size);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(valid_value_pops) {
    size_t cap = 2, size = 10, pops = 5, v_size;
    int_gar_t arr;
    int iters;

    int vals[size];
    for (int i = 0; i < size; i++) vals[i] = i;

    CALL_TEST(make_array, &arr, cap, 0);
    ASSERT_EQUAL(int_gar_pushes(&arr, size, vals), GAR_OK, "Could not push %d values.", size);

    ASSERT_EQUAL(int_gar_pops(&arr, pops, vals), GAR_OK, "Could not pop %d values.", size);

    for (int i = 0; i < pops; i++) {
        ASSERT_EQUAL(vals[i], size - i - 1, "Got %d instead of %d.", vals[i], size - i - 1);
    }

    int res[5] = {0, 1, 2, 3, 4};
    v_size = sizeof res / sizeof(int);
    CALL_TEST(array_eq, &arr, v_size, res);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(empty_value_pops) {
    size_t cap = 2, size = 10, pops = 5, v_size;
    int_gar_t arr;
    int iters;

    int vals[size];
    for (int i = 0; i < size; i++) vals[i] = i;

    CALL_TEST(make_array, &arr, cap, 0);
    ASSERT_EQUAL(int_gar_pushes(&arr, size, vals), GAR_OK, "Could not push %d values.", size);

    ASSERT_EQUAL(int_gar_pops(&arr, pops, NULL), GAR_OK, "Could not pop %d values.", size);

    int res[5] = {0, 1, 2, 3, 4};
    v_size = sizeof res / sizeof(int);
    CALL_TEST(array_eq, &arr, v_size, res);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(invalid_value_pops) {
    size_t cap = 2, size = 10, pops = 20;
    int_gar_t arr;
    int iters;

    int vals[pops];
    for (int i = 0; i < size; i++) vals[i] = i;

    CALL_TEST(make_array, &arr, cap, 0);
    ASSERT_EQUAL(int_gar_pushes(&arr, size, vals), GAR_OK, "Could not push %d values.", size);

    ASSERT_EQUAL(int_gar_pops(&arr, pops, vals), GAR_IDX_OOB, "Could pop %d values in array of size %d.", pops, size);

    int_gar_free(&arr);
    TEST_END;
}

LIST_TESTS(
    push_items,
    change_capacity,
    push_and_pop,
    pop_empty,
    value_pushes,
    valid_value_pops,
    empty_value_pops,
    invalid_value_pops,
)

