#include "../../../cut/cut.h"
#include "../../parse.h"
#include "../utils.c"

#define CORRECT_TOKEN(name, func, json, next_char) \
    UNIT_TEST(name) {                                                           \
        char str[] = json;                                                      \
        char* temp = str;                                                       \
                                                                                \
        result_ok(func(&temp), "Token not recognized.");                        \
        ASSERT_EQUAL(*temp, next_char, "Pointer not advanced.");                \
                                                                                \
        TEST_END;                                                               \
    }

#define INCORRECT_TOKEN(name, func, json) \
    UNIT_TEST(name) {                                                           \
        char str[] = json;                                                      \
        char* temp = str;                                                       \
                                                                                \
        ASSERT_EQUAL(func(&temp), PARSE_ERROR, "Incorrect token parsed.");      \
        ASSERT_EQUAL(temp, str, "Pointer moved %ld steps.", temp - str);        \
                                                                                \
        TEST_END;                                                               \
    }


UNIT_TEST(test_whitespace) {
    char str[] = "  \t\n \f \t  \r\nHello :)";
    char* temp = str;

    parse_skip_whitespace(&temp);
    ASSERT_EQUAL(*temp, 'H', "Not all whitespace was skipped.");

    TEST_END;
}

CORRECT_TOKEN(test_start_array, parse_start_array,          "[item1, ",         'i')
CORRECT_TOKEN(test_stop_array,  parse_end_array,            "], :)",            ',')
CORRECT_TOKEN(test_start_map,   parse_start_map,            "{item1: value1, ", 'i')
CORRECT_TOKEN(test_stop_map,    parse_end_map,              "}; :(",            ';')
CORRECT_TOKEN(test_entry,       parse_next_entry,           ", item2",          ' ')
CORRECT_TOKEN(test_key_value,   parse_key_value_divider,    ": value2",         ' ')
INCORRECT_TOKEN(no_start_array, parse_start_array, "item2")
INCORRECT_TOKEN(no_end_array, parse_end_array, "continue")
INCORRECT_TOKEN(no_start_map, parse_start_array, "item")
INCORRECT_TOKEN(no_end_map, parse_end_array, "yes more")
INCORRECT_TOKEN(no_next, parse_next_entry, "], :)")
INCORRECT_TOKEN(no_divider, parse_key_value_divider, ", :)")

LIST_TESTS(
    test_whitespace,
    test_start_array,
    test_stop_array,
    test_start_map,
    test_stop_map,
    no_start_array,
    no_end_array,
    no_start_map,
    no_end_map,
    no_next,
    no_divider,
)
