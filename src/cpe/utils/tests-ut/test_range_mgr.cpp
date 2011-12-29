#include "RangeMgrTest.hpp"

TEST_F(RangeMgrTest, get_one_from_empty) {
    EXPECT_EQ(-1, cpe_range_get_one(&m_ra));
}

TEST_F(RangeMgrTest, get_one_left) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 8));

    EXPECT_EQ(6, cpe_range_get_one(&m_ra));

    EXPECT_STREQ("[7~8),[10~12)", dump());
}

TEST_F(RangeMgrTest, get_one_empty) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 7));

    EXPECT_EQ(6, cpe_range_get_one(&m_ra));

    EXPECT_STREQ("[10~12)", dump());
}

TEST_F(RangeMgrTest, get_range_from_empty) {
    struct cpe_range r = cpe_range_get_range(&m_ra, 10);
    EXPECT_EQ(-1, r.m_start);
    EXPECT_EQ(-1, r.m_end);
}

TEST_F(RangeMgrTest, get_range_left) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 12));

    struct cpe_range r = cpe_range_get_range(&m_ra, 2);
    EXPECT_EQ(6, r.m_start);
    EXPECT_EQ(8, r.m_end);

    EXPECT_STREQ("[8~12),[18~20)", dump());
}

TEST_F(RangeMgrTest, get_range_full) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 12));

    struct cpe_range r = cpe_range_get_range(&m_ra, 6);
    EXPECT_EQ(6, r.m_start);
    EXPECT_EQ(12, r.m_end);

    EXPECT_STREQ("[18~20)", dump());
}

TEST_F(RangeMgrTest, get_range_not_enouth) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 12));

    struct cpe_range r = cpe_range_get_range(&m_ra, 8);
    EXPECT_EQ(6, r.m_start);
    EXPECT_EQ(12, r.m_end);

    EXPECT_STREQ("[18~20)", dump());
}

TEST_F(RangeMgrTest, put_one_to_empty) {
    EXPECT_EQ(0, cpe_range_put_one(&m_ra, 10));
    EXPECT_STREQ("[10~11)", dump());
}

TEST_F(RangeMgrTest, put_one_to_begin_not_connect) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_one(&m_ra, 8));

    EXPECT_STREQ("[8~9),[10~12)", dump());
}

TEST_F(RangeMgrTest, put_one_to_begin_connect) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_one(&m_ra, 9));

    EXPECT_STREQ("[9~12)", dump());
}

TEST_F(RangeMgrTest, put_one_to_end_not_connect) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_one(&m_ra, 13));

    EXPECT_STREQ("[10~12),[13~14)", dump());
}

TEST_F(RangeMgrTest, put_one_to_end_connect) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_one(&m_ra, 12));

    EXPECT_STREQ("[10~13)", dump());
}

TEST_F(RangeMgrTest, put_one_to_middle) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_one(&m_ra, 11));

    EXPECT_STREQ("[10~12)", dump());
}

TEST_F(RangeMgrTest, put_range_to_empty) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_STREQ("[10~12)", dump());
}

TEST_F(RangeMgrTest, put_range_start_invalid) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));

    EXPECT_EQ(-1, cpe_range_put_range(&m_ra, -3, 10));

    EXPECT_STREQ("[10~12)", dump());
}

TEST_F(RangeMgrTest, put_range_end_invalid) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));

    EXPECT_EQ(-1, cpe_range_put_range(&m_ra, 8, -1));

    EXPECT_STREQ("[10~12)", dump());
}

TEST_F(RangeMgrTest, put_range_end_lt_start) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));

    EXPECT_EQ(-1, cpe_range_put_range(&m_ra, 8, 7));

    EXPECT_STREQ("[10~12)", dump());
}

TEST_F(RangeMgrTest, put_range_empty) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 10));

    EXPECT_STREQ("[10~12)", dump());
}

TEST_F(RangeMgrTest, put_range_to_begin_not_connect) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 8));

    EXPECT_STREQ("[6~8),[10~12)", dump());
}

TEST_F(RangeMgrTest, put_range_to_begin_connect) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 10));

    EXPECT_STREQ("[6~12)", dump());
}

TEST_F(RangeMgrTest, put_range_to_begin_connect_in_middle) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 11));

    EXPECT_STREQ("[6~12)", dump());
}

TEST_F(RangeMgrTest, put_range_to_begin_connect_pass_to_end) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 13));

    EXPECT_STREQ("[6~13)", dump());
}

TEST_F(RangeMgrTest, put_range_to_begin_connect_pass_to_next) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 15, 16));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 17, 18));
    EXPECT_STREQ("[10~12),[15~16),[17~18)", dump());

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 6, 17));

    EXPECT_STREQ("[6~18)", dump());
}

TEST_F(RangeMgrTest, put_range_to_end_not_connect) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 14, 16));

    EXPECT_STREQ("[10~12),[14~16)", dump());
}

TEST_F(RangeMgrTest, put_range_to_end_connect) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 12, 16));

    EXPECT_STREQ("[10~16)", dump());
}

TEST_F(RangeMgrTest, put_range_to_end_connect_in_middle) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 11, 16));

    EXPECT_STREQ("[10~16)", dump());
}

TEST_F(RangeMgrTest, put_range_to_end_connect_pass_to_begin) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 16));

    EXPECT_STREQ("[10~16)", dump());
}

TEST_F(RangeMgrTest, put_range_to_end_connect_pass_to_prev) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 9, 16));

    EXPECT_STREQ("[9~16)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_not_connect) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));
    EXPECT_STREQ("[10~12),[18~20)", dump());

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 13, 17));
    EXPECT_STREQ("[10~12),[13~17),[18~20)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_pre) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));
    EXPECT_STREQ("[10~12),[18~20)", dump());

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 12, 17));
    EXPECT_STREQ("[10~17),[18~20)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_pre_to_middle) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));
    EXPECT_STREQ("[10~12),[18~20)", dump());

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 11, 17));
    EXPECT_STREQ("[10~17),[18~20)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_pre_to_begin) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));
    EXPECT_STREQ("[10~12),[18~20)", dump());

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 17));
    EXPECT_STREQ("[10~17),[18~20)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_pre_pass_begin) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));
    EXPECT_STREQ("[10~12),[18~20)", dump());

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 9, 17));
    EXPECT_STREQ("[9~17),[18~20)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_pre_to_pre) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 8, 9));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 9, 17));
    EXPECT_STREQ("[8~17),[18~20)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_next) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 13, 18));
    EXPECT_STREQ("[10~12),[13~20)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_next_to_middle) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 13, 19));
    EXPECT_STREQ("[10~12),[13~20)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_next_to_end) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 13, 20));
    EXPECT_STREQ("[10~12),[13~20)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_next_pass_end) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 13, 21));
    EXPECT_STREQ("[10~12),[13~21)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_next_to_next) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 23, 27));

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 13, 23));
    EXPECT_STREQ("[10~12),[13~27)", dump());
}

TEST_F(RangeMgrTest, put_range_to_middle_connect_both) {
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 10, 12));
    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 18, 20));

    EXPECT_EQ(0, cpe_range_put_range(&m_ra, 12, 18));
    EXPECT_STREQ("[10~20)", dump());
}
