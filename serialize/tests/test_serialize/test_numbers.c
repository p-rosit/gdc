#include <limits.h>
#include "../../../cut/cut.h"
#include "../../serialize.h"
#include "../utils.c"

#define TESTS (1000)

#define BASE_TESTS(name, type, upper_limit, is_signed) \
    UNIT_TEST(JOIN_TOKENS(name, _test_n_numbers)) {                             \
        json_str_t json = json_new();                                           \
        char expected[500];                                                     \
        char* result;                                                           \
        long long seed = 134, new_num;                                          \
        type value;                                                             \
                                                                                \
        for (int i = 0; i < TESTS; i++) {                                       \
            seed = rand_num(seed);                                              \
            value = seed;                                                       \
            new_num = value;                                                    \
                                                                                \
            SERIALIZE_NUMBER(expected, new_num, is_signed);                     \
                                                                                \
            result_ok(JOIN_TOKENS(serialize_, name)(&json, value), "Could not serialize number: %s.", expected); \
            result_ok(json_to_str(&json, &result), "Could not produce string."); \
            ASSERT_TRUE(strcmp(result, expected) == 0, "Result is %s instead of %s.", result, expected); \
                                                                                \
            free(result);                                                       \
        }                                                                       \
                                                                                \
        TEST_END;                                                               \
    }

#define UNSIGNED_TESTS(name, type, upper_limit) \
    BASE_TESTS(name, type, upper_limit, 0)

#define SIGNED_TESTS(name, type, upper_limit, lower_limit) \
    BASE_TESTS(name, type, upper_limit, 1)

#define TEST_FUNCS(name) \
    JOIN_TOKENS(name, _test_n_numbers)

SIGNED_TESTS(short,     short,      SHRT_MAX,   SHRT_MIN    )
SIGNED_TESTS(int,       int,        INT_MAX,    INT_MIN     )
SIGNED_TESTS(long,      long,       LONG_MAX,   LONG_MIN    )
SIGNED_TESTS(long_long, long long,  LLONG_MAX,  LLONG_MIN   )

LIST_TESTS(
    TEST_FUNCS(short),
    TEST_FUNCS(int),
    TEST_FUNCS(long),
    TEST_FUNCS(long_long)
)
