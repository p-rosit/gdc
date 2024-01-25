#include "../../cut/cut.h"
#include "../../gar.h"
#include "utils.c"


UNIT_TEST(deserialize_single) {
    int_gar_t arr;
    int values[] = {3};
    char json[] = "  [\n 3  \t  ]";

    result_ok(int_gar_from_json(&arr, json), "Could not parse Json.");
    CALL_TEST(array_eq, &arr, 1, values);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(deserialize_array) {
    size_t size;
    int_gar_t arr;
    int values[] = {3, 4, -5, 2, -3};
    char json[] = "[3,\n 4,\t -5, +2, -3]";

    result_ok(int_gar_from_json(&arr, json), "Could not parse Json.");

    size = sizeof(values) / sizeof(*values);
    CALL_TEST(array_eq, &arr, size, values);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(deserialize_empty) {
    int_gar_t arr;
    char json[] = "[]";

    result_ok(int_gar_from_json(&arr, json), "Could not parse Json.");
    CALL_TEST(array_eq, &arr, 0, NULL);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(only_whitespace) {
    int_gar_t arr;
    char json[] = "   \t\n   ";

    ASSERT_EQUAL(int_gar_from_json(&arr, json), PARSE_ERROR, "Invalid json was parsed.");
    CALL_TEST(array_eq, &arr, 0, NULL);
    
    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(missing_start) {
    int_gar_t arr;
    char json[] = "  5, 7, 2]";

    ASSERT_EQUAL(int_gar_from_json(&arr, json), PARSE_ERROR, "Invalid json was parsed.");
    CALL_TEST(array_eq, &arr, 0, NULL);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(missing_end) {
    int_gar_t arr;
    char json[] = "[1, 2, 3";

    ASSERT_EQUAL(int_gar_from_json(&arr, json), PARSE_ERROR, "Invalid json was parsed.");
    CALL_TEST(array_eq, &arr, 0, NULL);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(missing_value) {
    int_gar_t arr;
    char json[] = "[4, 5, , 7, 8]";

    ASSERT_EQUAL(int_gar_from_json(&arr, json), PARSE_ERROR, "Invalid json was parsed.");
    CALL_TEST(array_eq, &arr, 0, NULL);

    int_gar_free(&arr);
    TEST_END;
}


LIST_TESTS(
    deserialize_single,
    deserialize_array,
    deserialize_empty,
    only_whitespace,
    missing_start,
    missing_end,
    missing_value,
)
