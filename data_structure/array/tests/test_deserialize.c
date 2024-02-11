#include "../../../../cut/cut.h"
#include "../gar/gar.h"
#include "../gar/gar_parse.h"
#include "utils.c"


UNIT_TEST(deserialize_single) {
    TEST_BROKEN;
    int_gar_t arr;
    int values[] = {3};
    char *temp, json[] = "  [\n 3  \t  ]";

    temp = json;
    result_ok(int_gar_from_json(&arr, &temp), "Could not parse Json.");
    ASSERT_EQUAL(*temp, '\0', "Pointer moved %lu steps.", temp - json);
    CALL_TEST(array_eq, &arr, 1, values);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(deserialize_array) {
    TEST_BROKEN;
    size_t size;
    int_gar_t arr;
    int values[] = {3, 4, -5, 2, -3};
    char *temp, json[] = "[3,\n 4,\t -5, +2, -3]";

    temp = json;
    result_ok(int_gar_from_json(&arr, &temp), "Could not parse Json.");
    ASSERT_EQUAL(*temp, '\0', "Pointer moved %lu steps.", temp - json);

    size = sizeof(values) / sizeof(*values);
    CALL_TEST(array_eq, &arr, size, values);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(deserialize_empty) {
    int_gar_t arr;
    char *temp, json[] = "[]";

    temp = json;
    result_ok(int_gar_from_json(&arr, &temp), "Could not parse Json.");
    ASSERT_EQUAL(*temp, '\0', "Pointer moved %lu steps.", temp - json);
    CALL_TEST(array_eq, &arr, 0, NULL);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(only_whitespace) {
    int_gar_t arr;
    char *temp, json[] = "   \t\n   ";

    temp = json;
    ASSERT_EQUAL(int_gar_from_json(&arr, &temp), PARSE_ERROR, "Invalid json was parsed.");
    ASSERT_EQUAL(temp, json, "Pointer moved %lu steps.", temp - json);
    CALL_TEST(array_eq, &arr, 0, NULL);
    
    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(missing_start) {
    int_gar_t arr;
    char *temp, json[] = "  5, 7, 2]";

    temp = json;
    ASSERT_EQUAL(int_gar_from_json(&arr, &temp), PARSE_ERROR, "Invalid json was parsed.");
    ASSERT_EQUAL(temp, json, "Pointer moved %lu steps.", temp - json);
    CALL_TEST(array_eq, &arr, 0, NULL);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(missing_end) {
    int_gar_t arr;
    char *temp, json[] = "[1, 2, 3";

    temp = json;
    ASSERT_EQUAL(int_gar_from_json(&arr, &temp), PARSE_ERROR, "Invalid json was parsed.");
    ASSERT_EQUAL(temp, json, "Pointer moved %lu steps.", temp - json);
    CALL_TEST(array_eq, &arr, 0, NULL);

    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(missing_value) {
    int_gar_t arr;
    char *temp, json[] = "[4, 5, , 7, 8]";

    temp = json;
    ASSERT_EQUAL(int_gar_from_json(&arr, &temp), PARSE_ERROR, "Invalid json was parsed.");
    ASSERT_EQUAL(temp, json, "Pointer moved %lu steps.", temp - json);
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
