#include <limits.h>
#include "../../../cut/cut.h"
#include "../../parse.h"
#include "../utils.c"


#define SERIALIZE_NUMBER(str, num, is_signed) \
    do {                                                                        \
        if (is_signed) {sprintf(str, "%lld", num);}                             \
        else {sprintf(str, "%llu", num);}                                       \
    } while (0)

#define BASE_TEST(name, type, upper_limit, is_signed) \
    UNIT_TEST(JOIN_TOKENS(name, _test_n_numbers)) {                             \
        error_t error;                                                          \
        char num[500], *temp;                                                   \
        long long seed = 85, new_num;                                           \
        type v;                                                                 \
                                                                                \
        for (int i = 0; i < TESTS; i++) {                                       \
            seed = rand_num(seed);                                              \
            v = seed;                                                           \
            new_num = v;                                                        \
                                                                                \
            SERIALIZE_NUMBER(num, new_num, is_signed);                          \
                                                                                \
            temp = num;                                                         \
            result_ok(JOIN_TOKENS(parse_, name)(&v, &temp), "Could not parse number: %s.", num); \
            ASSERT_EQUAL(*temp, '\0', "String does not point to character after number."); \
        }                                                                       \
                                                                                \
        TEST_END;                                                               \
    }                                                                           \
                                                                                \
    UNIT_TEST(JOIN_TOKENS(name, _test_nonsense)) {                              \
        error_t error;                                                          \
        char num[] = "A10_and_so_on", *temp;                                    \
        type v;                                                                 \
                                                                                \
        temp = num;                                                             \
        error = JOIN_TOKENS(parse_, name)(&v, &temp);                           \
        ASSERT_EQUAL(error, PARSE_ERROR, "Incorrectly parsed \"%s\".", num);    \
                                                                                \
        TEST_END;                                                               \
    }                                                                           \
                                                                                \
    UNIT_TEST(JOIN_TOKENS(name, _test_above_limit)) {                           \
        size_t len;                                                             \
        error_t error;                                                          \
        char num[500], *temp;                                                   \
        type v;                                                                 \
                                                                                \
        SERIALIZE_NUMBER(num, (long long) upper_limit, is_signed);              \
        len = strlen(num);                                                      \
                                                                                \
        for (int i = 0; i < LIMIT_TESTS && i < len; i++) {                      \
            SERIALIZE_NUMBER(num, (long long) upper_limit, is_signed);          \
            add_to_index(num, i);                                               \
                                                                                \
            temp = num;                                                         \
            error = JOIN_TOKENS(parse_, name)(&v, &temp);                       \
            ASSERT_EQUAL(error, PARSE_ERROR, "Incorrectly parsed %s.", num);    \
        }                                                                       \
                                                                                \
        TEST_END;                                                               \
    }

#define SIGNED_TEST(name, type, upper_limit, lower_limit) \
    BASE_TEST(name, type, upper_limit, 1)                                       \
                                                                                \
    UNIT_TEST(JOIN_TOKENS(name, _test_below_limit)) {                           \
        size_t len;                                                             \
        error_t error;                                                          \
        char num[500], *temp;                                                   \
        type v;                                                                 \
                                                                                \
        SERIALIZE_NUMBER(num, (long long) upper_limit, 1);                      \
        len = strlen(num);                                                      \
                                                                                \
        for (int i = 0; i < LIMIT_TESTS && i < len; i++) {                      \
            SERIALIZE_NUMBER(num, (long long) lower_limit, 1);                  \
            add_to_index(num, i);                                               \
            temp = num;                                                         \
            error = JOIN_TOKENS(parse_, name)(&v, &temp);                       \
            ASSERT_EQUAL(error, PARSE_ERROR, "Incorrectly parsed %s.", num);    \
        }                                                                       \
                                                                                \
        TEST_END;                                                               \
    }

#define UNSIGNED_TEST(name, type, upper_limit) \
    BASE_TEST(name, type, upper_limit, 0)                                       \
                                                                                \
    UNIT_TEST(JOIN_TOKENS(name, _test_below_limit)) {                           \
        error_t error;                                                          \
        char num[] = "-1", *temp;                                               \
        type v;                                                                 \
                                                                                \
        temp = num;                                                             \
        error = JOIN_TOKENS(parse_, name)(&v, &temp);                           \
        ASSERT_EQUAL(error, PARSE_ERROR, "Incorrectly parsed signed number.");  \
                                                                                \
        TEST_END;                                                               \
    }

#define TEST_FUNCS(name) \
    JOIN_TOKENS(name, _test_n_numbers),                                         \
    JOIN_TOKENS(name, _test_nonsense),                                          \
    JOIN_TOKENS(name, _test_above_limit),                                       \
    JOIN_TOKENS(name, _test_below_limit)

#define TESTS (1000)

#define LIMIT_TESTS (2)
SIGNED_TEST(    short,      short,              SHRT_MIN,   SHRT_MAX    )
SIGNED_TEST(    i8,         int8_t,             INT8_MIN,   INT8_MAX    )
SIGNED_TEST(    i16,        int16_t,            INT16_MIN,  INT16_MAX   )
UNSIGNED_TEST(  ushort,     unsigned short,     USHRT_MAX               )
UNSIGNED_TEST(  u8,         uint8_t,            UINT8_MAX               )
UNSIGNED_TEST(  u16,        uint16_t,           UINT16_MAX              )

#undef LIMIT_TESTS
#define LIMIT_TESTS (5)
SIGNED_TEST(    int,        int,                INT_MIN,    INT_MAX     )
SIGNED_TEST(    long,       long,               LONG_MIN,   LONG_MAX    )
SIGNED_TEST(    long_long,  long long,          LLONG_MIN,  LLONG_MAX   )
SIGNED_TEST(    i32,        int32_t,            INT32_MIN,  INT32_MAX   )
SIGNED_TEST(    i64,        int64_t,            INT64_MIN,  INT64_MAX   )
UNSIGNED_TEST(  uint,       unsigned int,       UINT_MAX                )
UNSIGNED_TEST(  ulong,      unsigned long,      ULONG_MAX               )
UNSIGNED_TEST(  ulong_long, unsigned long long, ULLONG_MAX              )
UNSIGNED_TEST(  u32,        uint32_t,           UINT32_MAX              )
UNSIGNED_TEST(  u64,        uint64_t,           UINT64_MAX              )

LIST_TESTS(
    TEST_FUNCS(short),
    TEST_FUNCS(int),
    TEST_FUNCS(long),
    TEST_FUNCS(long_long),
    TEST_FUNCS(ushort),
    TEST_FUNCS(uint),
    TEST_FUNCS(ulong),
    TEST_FUNCS(ulong_long),
    TEST_FUNCS(i8),
    TEST_FUNCS(i16),
    TEST_FUNCS(i32),
    TEST_FUNCS(i64),
    TEST_FUNCS(u8),
    TEST_FUNCS(u16),
    TEST_FUNCS(u32),
    TEST_FUNCS(u64),
)
