#include "IdGeneratorTest.hpp"

TEST_F(IdGeneratorTest, generate_first) {
    usf_id_t r;

    EXPECT_EQ(0, usf_id_generator_generate(&r, id_generator()));
    EXPECT_EQ((usf_id_t)1, r);
}

TEST_F(IdGeneratorTest, generate_to_next_range) {
    usf_id_t r;

    usf_id_generator_set_once_alloc_size(id_generator(), 12);

    for(int i = 0; i < 100; ++i) {
        EXPECT_EQ(0, usf_id_generator_generate(&r, id_generator()));
        EXPECT_EQ((usf_id_t)(i + 1), r);
    }
}

TEST_F(IdGeneratorTest, with_file_generate_first) {
    usf_id_t r;

    set_load_from_file();

    EXPECT_EQ(0, usf_id_generator_generate(&r, id_generator()));
    EXPECT_EQ((usf_id_t)1, r);
}

TEST_F(IdGeneratorTest, with_file_generate_to_next_range) {
    usf_id_t r;

    set_load_from_file();
    usf_id_generator_set_once_alloc_size(id_generator(), 12);

    for(int i = 0; i < 100; ++i) {
        EXPECT_EQ(0, usf_id_generator_generate(&r, id_generator()));
        EXPECT_EQ((usf_id_t)(i + 1), r);
    }
}
