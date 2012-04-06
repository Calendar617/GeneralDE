#include "RangeBitarryTest.hpp"

TEST_F(RangeBitarryTest, free_basic) {
    const char * str_ba = "0111101010"; 
    size_t capacity = strlen(str_ba);

    EXPECT_EQ(
        0,
        cpe_range_free_from_bitarray(
            &m_ra,
            create_ba(str_ba),
            0,
            capacity));

    EXPECT_STREQ("[1~5),[6~7),[8~9)", dump());
}

TEST_F(RangeBitarryTest, free_with_start) {
    const char * str_ba = "0111101010"; 
    size_t capacity = strlen(str_ba);

    EXPECT_EQ(
        0,
        cpe_range_free_from_bitarray(
            &m_ra,
            create_ba(str_ba),
            12,
            capacity));

    EXPECT_STREQ("[13~17),[18~19),[20~21)", dump());
}
