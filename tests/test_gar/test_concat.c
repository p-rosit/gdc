#include "../../cut/cut.h"

#include "../../gar.h"
#include "utils.c"


UNIT_TEST(concat) {
    int_gar_t a, b;

    CALL_TEST(make_array, &a, 10, 5);
    CALL_TEST(make_array, &b, 30, 3);

    ASSERT_EQUAL(int_gar_concat(&a, &b), GAR_OK, "Could not append arrays.");

    int res[] = {0, 1, 2, 3, 4, 0, 1, 2};
    size_t v_size = sizeof res / sizeof(int);
    CALL_TEST(array_eq, &a, v_size, res);

    TEST_END;
}

UNIT_TEST(concat_empty) {
    int_gar_t a, b;

    int_gar_new(&a);
    int_gar_new(&b);

    ASSERT_EQUAL(int_gar_concat(&a, &b), GAR_OK, "Could not append empty arrays.");

    CALL_TEST(array_eq, &a, 0, NULL);

    TEST_END;
}

UNIT_TEST(concat_on_empty) {
    int_gar_t a, b;

    int_gar_new(&a);
    CALL_TEST(make_array, &b, 30, 3);

    ASSERT_EQUAL(int_gar_concat(&a, &b), GAR_OK, "Could not append arrays.");

    int res[] = {0, 1, 2};
    size_t v_size = sizeof res / sizeof(int);
    CALL_TEST(array_eq, &a, v_size, res);

    TEST_END;
}

UNIT_TEST(concat_with_empty) {
    int_gar_t a, b;

    CALL_TEST(make_array, &a, 30, 5);
    int_gar_new(&b);

    ASSERT_EQUAL(int_gar_concat(&a, &b), GAR_OK, "Could not append arrays.");

    int res[] = {0, 1, 2, 3, 4};
    size_t v_size = sizeof res / sizeof(int);
    CALL_TEST(array_eq, &a, v_size, res);

    TEST_END;
}

LIST_TESTS(
    concat,
    concat_empty,
    concat_on_empty,
    concat_with_empty,
)

