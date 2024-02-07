#include <limits.h>
#include "../../../cut/cut.h"
#include "../../serialize.h"

#define TESTS (1000)

#define NUMBER_TEST_CASE(test_name, name, print_option, type, value, expected_result) \
    UNIT_TEST(test_name) { \
        json_str_t json = json_new(); \
        char expected[500] = expected_result; \
        char* result; \
        type number = value; \
        \
        JOIN_TOKENS(serialize_, name)(&json, number); \
        json_to_str(&json, &result); \
        \
        ASSERT_TRUE(strcmp(result, expected) == 0, "Result is %s instead of %s.", result, expected); \
        \
        free(result);\
        TEST_END;\
    }

#define UNSIGNED_TESTS(name, type, print_option, upper_limit) \
    NUMBER_TEST_CASE(JOIN_TOKENS(test_, name), name, type, 123, "123") \
    NUMBER_TEST_CASE(JOIN_TOKENS(test_one_, name), name, type, 1, "1") \
    NUMBER_TEST_CASE(JOIN_TOKENS(test_zero_, name), name, type, 0, "0") \

#define SIGNED_TESTS(name, type, upper_limit, lower_limit) \
    UNSIGNED_TESTS(name, type, upper_limit) \
    \
    NUMBER_TEST_CASE(JOIN_TOKENS(test_negative_, name), name, type, -123, "-123")

#define UNSIGNED_TEST_FUNCS(name) \
    JOIN_TOKENS(test_, name), \
    JOIN_TOKENS(test_one_, name), \
    JOIN_TOKENS(test_zero_, name) \

#define SIGNED_TEST_FUNCS(name) \
    UNSIGNED_TEST_FUNCS(name), \
    JOIN_TOKENS(test_negative_, name)

SIGNED_TESTS(short, short, "%d", SHRT_MAX, SHRT_MIN)

LIST_TESTS(
    SIGNED_TEST_FUNCS(short)
)
