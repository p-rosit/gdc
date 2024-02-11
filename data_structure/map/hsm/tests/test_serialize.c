#include "../../../../cut/cut.h"
#include "../hsm.h"
#include "../hsm_serialize.h"
#include "utils.c"

UNIT_TEST(serialize_single) {
    s2i_hsm_t map;
    char* keys[] = {"B"};
    int values[] = {  2};
    char* json;
    char* result = "[[\"B\", 2]]";

    s2i_hsm_new(&map);
    for (size_t i = 0; i < sizeof(values) / sizeof(*values); i++) {
        s2i_hsm_insert(&map, keys[i], values[i]);
    }

    result_ok(s2i_hsm_to_json(&map, &json), "Could not serialize map.");
    ASSERT_TRUE(strcmp(json, result) == 0, "Json is \"%s\" instead of \"%s\".", json, result);

    free(json);
    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(serialize_map) {
    s2i_hsm_t map;
    char* keys[] = {"B", "C", ":)"};
    int values[] = {  2,   0,   -3};
    char* json;
    /* TODO: remove hardcoding of string? */
    char* result = "[[\"B\", 2], [\":)\", -3], [\"C\", 0]]";

    s2i_hsm_new(&map);
    for (size_t i = 0; i < sizeof(values) / sizeof(*values); i++) {
        s2i_hsm_insert(&map, keys[i], values[i]);
    }

    result_ok(s2i_hsm_to_json(&map, &json), "Could not serialize map.");
    ASSERT_TRUE(strcmp(json, result) == 0, "Json is \"%s\" instead of \"%s\".", json, result);

    free(json);
    s2i_hsm_free(&map);
    TEST_END;
}

UNIT_TEST(serialize_empty) {
    s2i_hsm_t map;
    char* json;
    char* result = "[]";

    s2i_hsm_new(&map);

    result_ok(s2i_hsm_to_json(&map, &json), "Could not serialize map.");
    ASSERT_TRUE(strcmp(json, result) == 0, "Json is \"%s\" instead of \"%s\".", json, result);

    free(json);
    s2i_hsm_free(&map);
    TEST_END;
}


LIST_TESTS(
    serialize_single,
    serialize_map,
    serialize_empty
)
