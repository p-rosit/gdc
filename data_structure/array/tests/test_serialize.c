#include "../../../../cut/cut.h"
#include "../gar.h"
#include "utils.c"

UNIT_TEST(serialize_single) {
    int_gar_t arr;
    char* json;
    char* result = "[0]";

    CALL_TEST(make_array, &arr, 20, 1);

    result_ok(int_gar_to_json(&arr, &json), "Could not serialize array.");
    ASSERT_TRUE(strcmp(json, result) == 0, "Json is \"%s\" instead of \"%s\".", json, result);

    free(json);
    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(serialize_array) {
    int_gar_t arr;
    char* json;
    char* result = "[0, 1, 2, 3, 4]";

    CALL_TEST(make_array, &arr, 20, 5);

    result_ok(int_gar_to_json(&arr, &json), "Could not serialize array.");
    ASSERT_TRUE(strcmp(json, result) == 0, "Json is \"%s\" instead of \"%s\".", json, result);

    free(json);
    int_gar_free(&arr);
    TEST_END;
}

UNIT_TEST(serialize_empty) {
    int_gar_t arr;
    char* json;
    char* result = "[]";

    CALL_TEST(make_array, &arr, 20, 0);

    result_ok(int_gar_to_json(&arr, &json), "Could not serialize array.");
    ASSERT_TRUE(strcmp(json, result) == 0, "Json is \"%s\" instead of \"%s\".", json, result);

    free(json);
    int_gar_free(&arr);
    TEST_END;
}


LIST_TESTS(
    serialize_single,
    serialize_array,
    serialize_empty,
)
