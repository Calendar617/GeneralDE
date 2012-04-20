#include "IdGeneratorTest.hpp"

TEST_F(IdGeneratorTest, generate_first) {
    gd_id_t r;

    EXPECT_EQ(0, gd_id_generator_generate(&r, id_generator()));
    EXPECT_EQ((gd_id_t)1, r);
}

TEST_F(IdGeneratorTest, generate_to_next_range) {
    gd_id_t r;

    gd_id_generator_set_once_alloc_size(id_generator(), 12);

    for(int i = 0; i < 100; ++i) {
        EXPECT_EQ(0, gd_id_generator_generate(&r, id_generator()));
        EXPECT_EQ((gd_id_t)(i + 1), r);
    }
}

TEST_F(IdGeneratorTest, with_file_generate_first) {
    gd_id_t r;

    set_load_from_file();

    EXPECT_EQ(0, gd_id_generator_generate(&r, id_generator()));
    EXPECT_EQ((gd_id_t)1, r);
}

TEST_F(IdGeneratorTest, with_file_generate_to_next_range) {
    gd_id_t r;

    set_load_from_file();
    gd_id_generator_set_once_alloc_size(id_generator(), 12);

    for(int i = 0; i < 100; ++i) {
        EXPECT_EQ(0, gd_id_generator_generate(&r, id_generator()));
        EXPECT_EQ((gd_id_t)(i + 1), r);
    }
}
