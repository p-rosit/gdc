#include "../../../cut/cut.h"
#include "../../macro.h"
#include "../serialize.h"
#include "../parse.h"
#include "utils.c"

#define TESTS (5000)

#define BASE_TEST(name, type, is_signed) \
UNIT_TEST(JOIN_TOKENS(test_, name)) {                                           \
    error_t error;                                                              \
    long long seed = 123;                                                       \
    json_str_t json = json_new();                                               \
    char *str, *temp;                                                           \
    type value, parsed;                                                         \
                                                                                \
    for (size_t i = 0; i < TESTS; i++) {                                        \
        seed = rand_num(seed);                                                  \
        value = seed;                                                           \
                                                                                \
        error = JOIN_TOKENS(serialize_, name)(&json, value);                    \
        if (is_signed) {                                                        \
            result_ok(error, "Could not serialize: %lld.", (long long) value);  \
        } else {                                                                \
            result_ok(error, "Could not serialize: %llu.", (unsigned long long) value); \
        }                                                                       \
                                                                                \
        json_to_str(&json, &str); temp = str;                                   \
        error = JOIN_TOKENS(parse_, name)(&parsed, &temp);                      \
        result_ok(error, "Could not parse number: \"%s\".", str);               \
                                                                                \
        if (is_signed) {                                                        \
            ASSERT_EQUAL(parsed, value, "Parsed value is %lld instead of %lld.", (long long) parsed, (long long) value); \
        } else {                                                                \
            ASSERT_EQUAL(parsed, value, "Parsed value is %llu instead of %llu.", (unsigned long long) parsed, (unsigned long long) value); \
        }                                                                       \
        free(str);                                                              \
    }                                                                           \
                                                                                \
    TEST_END;                                                                   \
}

#define TEST_UNSIGNED(name, type) \
    BASE_TEST(name, type, 0)

#define TEST_SIGNED(name, type) \
    BASE_TEST(name, type, 1)

TEST_SIGNED(short, short)
TEST_SIGNED(int, int)
TEST_SIGNED(long, long)
TEST_SIGNED(long_long, long long)
TEST_UNSIGNED(ushort, unsigned short)
TEST_UNSIGNED(uint, unsigned int)
TEST_UNSIGNED(ulong, unsigned long)
TEST_UNSIGNED(ulong_long, unsigned long long)

LIST_TESTS(
    test_short,
    test_int,
    test_long,
    test_long_long,
    test_ushort,
    test_uint,
    test_ulong,
    test_ulong_long
)
