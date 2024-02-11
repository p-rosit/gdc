#include "../../../../cut/cut.h"
#include "../hsm.h"
#include "../hsm_parse.h"
#include "utils.c"

#define INVALID_JSON(name, json_string) \
    UNIT_TEST(name) {                                                           \
        s2i_hsm_t map;                                                          \
        char *temp, json[] = json_string;                                       \
                                                                                \
        temp = json;                                                            \
        ASSERT_EQUAL(s2i_hsm_from_json(&map, &temp), PARSE_ERROR, "Invalid json was parsed."); \
        ASSERT_EQUAL(temp, json, "Pointer moved %lu steps.", temp - json);      \
                                                                                \
        TEST_END;                                                               \
    } 


UNIT_TEST(deserialize_single) {
    s2i_hsm_t map;
    char *keys[] = {":)"};
    int values[] = {   5};
    char *temp, json[] = " \t [  [\":)\", \n 5 ]\t\t ]";

    temp = json;
    result_ok(s2i_hsm_from_json(&map, &temp), "Could not parse Json.");
    ASSERT_EQUAL(*temp, '\0', "Pointer moved %lu steps.", temp - json);
    
    size_t size = sizeof(values) / sizeof(*values);
    CALL_TEST(map_eq, &map, size, (char**) keys, values);

    s2i_hsm_free_items(&map);
    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(deserialize_map) {
    s2i_hsm_t map;
    char *keys[] = {"A", "B", "C"};
    int values[] = {  5, -23,  67};
    char *temp, json[] = "[[\"A\", 5], [\"B\", -23], [\"C\", +67]]";

    temp = json;
    result_ok(s2i_hsm_from_json(&map, &temp), "Could not parse Json.");
    ASSERT_EQUAL(*temp, '\0', "Pointer moved %lu steps.", temp - json);
    
    size_t size = sizeof(values) / sizeof(*values);
    CALL_TEST(map_eq, &map, size, (char**) keys, values);

    s2i_hsm_free_items(&map);
    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(deserialize_empty) {
    s2i_hsm_t map;
    char *temp, json[] = "[]";

    temp = json;
    result_ok(s2i_hsm_from_json(&map, &temp), "Could not parse Json.");
    ASSERT_EQUAL(*temp, '\0', "Pointer moved %lu steps.", temp - json);
    CALL_TEST(map_eq, &map, 0, NULL, NULL);

    s2i_hsm_free(&map);
    TEST_END;
}

INVALID_JSON(only_whitespace, "   \t\f\n  ")
INVALID_JSON(missing_start, "  [\"A\", 5], [\"C\", 6]]")
INVALID_JSON(missing_end, "[[\"A\", 5], [\"C\", 6]")
INVALID_JSON(missing_kvp, "[[\"A\", 5], , [\"C\", 6]]")
INVALID_JSON(missing_value, "[[\"A\"], [\"C\", 6]]")
INVALID_JSON(large_kvp, "[[\"A\", 5], [\"C\", 6, 10]]")


LIST_TESTS(
    deserialize_single,
    deserialize_map,
    deserialize_empty,
    only_whitespace,
    missing_start,
    missing_end,
    missing_kvp,
    missing_value,
    large_kvp,
)
