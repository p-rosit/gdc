#include "../../../../cut/cut.h"
#include "../gar.h"
#include "utils.c"


UNIT_TEST(concat) {
    size_t cap;
    int_gar_t a, b;

    CALL_TEST(make_array, &a, 10, 5);
    CALL_TEST(make_array, &b, 30, 3);

    cap = b.capacity;
    result_ok(int_gar_concat(&a, &b), "Could not concatenate arrays.");

    ASSERT_EQUAL(b.size, 0, "Size is %lu, array was not consumed.", b.size);
    ASSERT_EQUAL(b.capacity, cap, "Capacity is %lu instead of %lu.", b.capacity, cap);

    int res[] = {0, 1, 2, 3, 4, 0, 1, 2};
    size_t v_size = sizeof(res) / sizeof(int);
    CALL_TEST(array_eq, &a, v_size, res);

    int_gar_free(&a);
    int_gar_free(&b);
    TEST_END;
}

UNIT_TEST(concat_empty) {
    int_gar_t a, b;

    a = int_gar_new();
    b = int_gar_new();

    result_ok(int_gar_concat(&a, &b), "Could not concatenate arrays.");
    ASSERT_EQUAL(b.size, 0, "Size is %lu, array was not consumed.", b.size);

    CALL_TEST(array_eq, &a, 0, NULL);

    int_gar_free(&a);
    int_gar_free(&b);
    TEST_END;
}

UNIT_TEST(concat_on_empty) {
    int_gar_t a, b;

    a = int_gar_new();
    CALL_TEST(make_array, &b, 30, 3);

    result_ok(int_gar_concat(&a, &b), "Could not concatenate arrays.");
    ASSERT_EQUAL(b.size, 0, "Size is %lu, array was not consumed.", b.size);

    int res[] = {0, 1, 2};
    size_t v_size = sizeof(res) / sizeof(int);
    CALL_TEST(array_eq, &a, v_size, res);

    int_gar_free(&a);
    int_gar_free(&b);
    TEST_END;
}

UNIT_TEST(concat_with_empty) {
    int_gar_t a, b;

    CALL_TEST(make_array, &a, 30, 5);
    b = int_gar_new();

    result_ok(int_gar_concat(&a, &b), "Could not concatenate arrays.");
    ASSERT_EQUAL(b.size, 0, "Size is %lu, array was not consumed.", b.size);

    int res[] = {0, 1, 2, 3, 4};
    size_t v_size = sizeof(res) / sizeof(int);
    CALL_TEST(array_eq, &a, v_size, res);

    int_gar_free(&a);
    int_gar_free(&b);
    TEST_END;
}

LIST_TESTS(
    concat,
    concat_empty,
    concat_on_empty,
    concat_with_empty,
)

