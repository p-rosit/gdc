#include "../../../cut/cut.h"
#include "../../parse.h"
#include "../utils.c"

#define TEST_STRING_PARSE(name, input, expected_result) \
    UNIT_TEST(name) {                                                           \
        char *value, *temp;                                                     \
        char str[] = input;                                                     \
        char res[] = expected_result;                                           \
                                                                                \
        temp = str;                                                             \
        result_ok(parse_string(&value, &temp), "Could not parse string: \"%s\".", str);\
        ASSERT_TRUE(strcmp(value, res) == 0, "Result is \"%*s\" instead of \"%s\".", sizeof(str) - 3, value, res);\
        ASSERT_EQUAL(*temp, '\0', "Pointer has not moved past input, pointing at \"%c\".", *temp); \
                                                                                \
        free(value);                                                            \
        TEST_END;                                                               \
    }

#define TEST_INVALID_STRING(name, input) \
    UNIT_TEST(name) {                                                           \
        char *value, *temp;                                                     \
        char str[] = input;                                                     \
                                                                                \
        temp = str;                                                             \
        ASSERT_EQUAL(parse_string(&value, &temp), PARSE_ERROR, "Accidentally parsed string: \"%s\".", str);\
                                                                                \
        TEST_END;                                                               \
    }

TEST_STRING_PARSE(parse_value,
    "\"Here is a string\"",
    "Here is a string"
)

TEST_STRING_PARSE(parse_escaped,
    "\"Quoth the raven: \\\"Nevermore!\\\"\"",
    "Quoth the raven: \"Nevermore!\""
)

TEST_STRING_PARSE(not_escaped,
    "\"A string: \\\\\"",
    "A string: \\"
)

TEST_STRING_PARSE(parse_special,
    "\"Whitespace: \\t\\r\\n\\f\"",
    "Whitespace: \t\r\n\f"
)

UNIT_TEST(parse_hex) {
    char expected[] = "\"\\uf450\\u00FF\"";
    char res[] = "\uf450\u00ff";
    TEST_BROKEN;
}

TEST_INVALID_STRING(missing_start, "A string.\"")
TEST_INVALID_STRING(missing_end, "\"A string that does not end.")
TEST_INVALID_STRING(invalid_escape, "\"Here: \\y\"")
TEST_INVALID_STRING(non_hex, "\"Not hex: \\u3y1a\"")

LIST_TESTS(
    parse_value,
    parse_escaped,
    not_escaped,
    parse_special,
    parse_hex,
    missing_start,
    missing_end,
    invalid_escape,
    non_hex,
)

