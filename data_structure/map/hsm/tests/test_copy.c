#include "../../../../cut/cut.h"
#include "../hsm.h"
#include "utils.c"


UNIT_TEST(copy_empty) {
    s2i_hsm_t src, dst;
    
    s2i_hsm_new(&src);
    result_ok(s2i_hsm_copy(&src, &dst), "Could not copy map.");

    ASSERT_EQUAL(dst.capacity, src.capacity, "Capacity is %lu instead of %lu.", dst.capacity, src.capacity);
    ASSERT_EQUAL(dst.size, 0, "Size is %lu instead of 0.", dst.size);
    ASSERT_EQUAL(dst.max_offset, src.max_offset, "Max offset is %lu instead of %lu.", dst.max_offset, src.max_offset);
    
    for (size_t i = 0; i < dst.capacity + dst.max_offset; i++) {
        ASSERT_EQUAL(dst.meta_data[i].offset, dst.max_offset, "Map is non-empty.");
    }

    s2i_hsm_free(&src);
    s2i_hsm_free(&dst);
    TEST_END;
}

UNIT_TEST(copy_map) {
    size_t i, size;
    s2i_hsm_t src, dst;
    char k[] = {10, 20, 30, 40, 50, 60};

    size = sizeof(k) / sizeof(char);
    int v[size];
    for (i = 0; i < size; i++) v[i] = i;

    s2i_hsm_new(&src);
    for (i = 0; i < size; i++) {
        result_ok(s2i_hsm_insert(&src, &k[i], v[i]), "Could not insert pair: {\"%c\": %d}.", k[i], v[i]);
    }

    result_ok(s2i_hsm_copy(&src, &dst), "Could not copy map.");

    for (i = 0; i < size; i++) {
        ASSERT_EQUAL(
            src.meta_data[i].offset, dst.meta_data[i].offset,
            "Offset at index %lu is %lu instead of %lu.",
            i, src.meta_data[i].offset, dst.meta_data[i].offset
        );
        if (src.meta_data[i].offset < src.max_offset) {
            CALL_TEST(check_data, &src, dst.meta_data[i], dst.keys[i], dst.values[i]);
        }
    }

    s2i_hsm_free(&src);
    s2i_hsm_free(&dst);
    TEST_END;
}

UNIT_TEST(deepcopy_empty) {
    s2i_hsm_t src, dst;
    
    s2i_hsm_new(&src);
    result_ok(s2i_hsm_deepcopy(&src, &dst), "Could not copy map.");

    ASSERT_EQUAL(dst.capacity, src.capacity, "Capacity is %lu instead of %lu.", dst.capacity, src.capacity);
    ASSERT_EQUAL(dst.size, 0, "Size is %lu instead of 0.", dst.size);
    ASSERT_EQUAL(dst.max_offset, src.max_offset, "Max offset is %lu instead of %lu.", dst.max_offset, src.max_offset);
    
    for (size_t i = 0; i < dst.capacity + dst.max_offset; i++) {
        ASSERT_EQUAL(dst.meta_data[i].offset, dst.max_offset, "Map is non-empty.");
    }

    s2i_hsm_free(&src);
    s2i_hsm_free(&dst);
    TEST_END;
}

UNIT_TEST(deepcopy_map) {
    size_t i, size;
    s2i_hsm_t src, dst;
    char k[][2] = {"A", "B", "C", "D"}, *temp;

    size = sizeof(k) / (sizeof(*k) * sizeof(**k));
    int v[size];
    for (i = 0; i < size; i++) v[i] = i;

    s2i_hsm_new(&src);
    for (i = 0; i < size; i++) {
        temp = str_duplicate(k[i]);
        result_ok(s2i_hsm_insert(&src, temp, v[i]), "Could not insert pair: {\"%c\": %d}.", k[i], v[i]);
    }

    result_ok(s2i_hsm_deepcopy(&src, &dst), "Could not copy map.");

    for (i = 0; i < size; i++) {
        ASSERT_EQUAL(
            src.meta_data[i].offset, dst.meta_data[i].offset,
            "Offset at index %lu is %lu instead of %lu.",
            i, src.meta_data[i].offset, dst.meta_data[i].offset
        );
        if (src.meta_data[i].offset < src.max_offset) {
            ASSERT_EQUAL(dst.meta_data[i].offset, src.meta_data[i].offset, "%lu: offset is %lu instead of %lu.", i, dst.meta_data[i].offset, src.meta_data[i].offset);
            ASSERT_EQUAL(dst.meta_data[i].hash, src.meta_data[i].hash, "%lu: hash is %lu instead of %lu.", i, dst.meta_data[i].hash, dst.meta_data[i].hash);
            
            ASSERT_NOT_EQUAL(dst.keys[i], src.keys[i], "%lu: pointers are equal: %p", i, src.keys[i]);
            ASSERT_TRUE(strcmp(dst.keys[i], src.keys[i]) == 0, "%lu: saved string is %s instead of %s", i, dst.keys[i], src.keys[i]);

            ASSERT_EQUAL(dst.values[i], src.values[i], "%lu value is %d instead of %d.", i, dst.values[i], src.values[i]);
        }
    }

    s2i_hsm_free_items(&src);
    s2i_hsm_free_items(&dst);
    s2i_hsm_free(&src);
    s2i_hsm_free(&dst);
    TEST_END;
}


LIST_TESTS(
    copy_empty,
    copy_map,
    deepcopy_empty,
    deepcopy_map,
)
