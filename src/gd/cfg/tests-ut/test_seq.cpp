#include "CfgTest.hpp"
#include "../cfg_internal_types.h"

class SeqTest : public CfgTest {
public:
    SeqTest() : m_seq(NULL) {}

    virtual void SetUp() {
        CfgTest::SetUp();

        m_seq = gd_cfg_struct_add_seq(m_root, "a");
        ASSERT_TRUE(m_seq);
    }

    void fill(int n) {
        while(n > 0) {
            gd_cfg_seq_add_struct(m_seq);
            --n;
        }
    }

    gd_cfg_t m_seq;
};

TEST_F(SeqTest, count) {
    EXPECT_EQ(0, gd_cfg_seq_count(m_seq));
}

TEST_F(SeqTest, add_basic) {
    gd_cfg_t s = gd_cfg_seq_add_struct(m_seq);
    ASSERT_TRUE(s);
    EXPECT_STREQ("a", gd_cfg_name(s));

    EXPECT_EQ(GD_CFG_TYPE_STRUCT, gd_cfg_type(s));
    EXPECT_TRUE(m_seq == gd_cfg_parent(s));

    EXPECT_EQ(1, gd_cfg_seq_count(m_seq));
}

TEST_F(SeqTest, add_order) {
    gd_cfg_t s1 = gd_cfg_seq_add_struct(m_seq);
    ASSERT_TRUE(s1);

    gd_cfg_t s2 = gd_cfg_seq_add_struct(m_seq);
    ASSERT_TRUE(s2);

    EXPECT_EQ(2, gd_cfg_seq_count(m_seq));

    EXPECT_TRUE(s1 == gd_cfg_seq_at(m_seq, 0));
    EXPECT_TRUE(s2 == gd_cfg_seq_at(m_seq, 1));
}

TEST_F(SeqTest, add_to_next_trunk) {
    fill(GD_CFG_SEQ_BLOCK_ITEM_COUNT);
    gd_cfg_t s1 = gd_cfg_seq_add_struct(m_seq);
    ASSERT_TRUE(s1);

    gd_cfg_t s2 = gd_cfg_seq_add_struct(m_seq);
    ASSERT_TRUE(s2);

    EXPECT_EQ(GD_CFG_SEQ_BLOCK_ITEM_COUNT + 2, gd_cfg_seq_count(m_seq));

    EXPECT_TRUE(s1 == gd_cfg_seq_at(m_seq, GD_CFG_SEQ_BLOCK_ITEM_COUNT + 0));
    EXPECT_TRUE(s2 == gd_cfg_seq_at(m_seq, GD_CFG_SEQ_BLOCK_ITEM_COUNT + 1));
}

TEST_F(SeqTest, add_not_from_seq) {
    gd_cfg_t not_seq = gd_cfg_struct_add_int8(m_root, "aa", 4);
    EXPECT_FALSE(gd_cfg_seq_add_struct(not_seq));
}


TEST_F(SeqTest, at_from_empty) {
    EXPECT_TRUE(NULL == gd_cfg_seq_at(m_seq, 0));
}

TEST_F(SeqTest, at_pos_negative) {
    gd_cfg_seq_add_struct(m_seq);

    EXPECT_FALSE(gd_cfg_seq_at(m_seq, -1));
}

TEST_F(SeqTest, at_pos_overflow) {
    gd_cfg_seq_add_struct(m_seq);

    EXPECT_FALSE(gd_cfg_seq_at(m_seq, 1));
}

TEST_F(SeqTest, at_basic) {
    gd_cfg_t s = gd_cfg_seq_add_struct(m_seq);

    EXPECT_TRUE(s == gd_cfg_seq_at(m_seq, 0));
}

TEST_F(SeqTest, remove_last) {
    gd_cfg_t i0 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t i1 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t i2 = gd_cfg_seq_add_struct(m_seq);

    gd_cfg_free(i2);

    EXPECT_EQ(2, gd_cfg_seq_count(m_seq));
    EXPECT_EQ(i0, gd_cfg_seq_at(m_seq, 0));
    EXPECT_EQ(i1, gd_cfg_seq_at(m_seq, 1));
}

TEST_F(SeqTest, remove_middle) {
    gd_cfg_t i0 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t i1 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t i2 = gd_cfg_seq_add_struct(m_seq);

    gd_cfg_free(i1);

    EXPECT_EQ(2, gd_cfg_seq_count(m_seq));
    EXPECT_EQ(i0, gd_cfg_seq_at(m_seq, 0));
    EXPECT_EQ(i2, gd_cfg_seq_at(m_seq, 1));
}

TEST_F(SeqTest, remove_first) {
    gd_cfg_t i0 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t i1 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t i2 = gd_cfg_seq_add_struct(m_seq);

    gd_cfg_free(i0);

    EXPECT_EQ(2, gd_cfg_seq_count(m_seq));
    EXPECT_EQ(i1, gd_cfg_seq_at(m_seq, 0));
    EXPECT_EQ(i2, gd_cfg_seq_at(m_seq, 1));
}

TEST_F(SeqTest, remove_trunk_last_no_left) {
    fill(GD_CFG_SEQ_BLOCK_ITEM_COUNT - 2);
    gd_cfg_t l0 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t l1 = gd_cfg_seq_add_struct(m_seq);

    gd_cfg_free(l1);

    EXPECT_EQ(GD_CFG_SEQ_BLOCK_ITEM_COUNT - 1, gd_cfg_seq_count(m_seq));
    EXPECT_EQ(l0, gd_cfg_seq_at(m_seq, GD_CFG_SEQ_BLOCK_ITEM_COUNT - 2));
}

TEST_F(SeqTest, remove_trunk_last_with_one_left) {
    fill(GD_CFG_SEQ_BLOCK_ITEM_COUNT - 1);
    gd_cfg_t l0 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t l1 = gd_cfg_seq_add_struct(m_seq);

    gd_cfg_free(l0);

    EXPECT_EQ(GD_CFG_SEQ_BLOCK_ITEM_COUNT, gd_cfg_seq_count(m_seq));
    EXPECT_EQ(l1, gd_cfg_seq_at(m_seq, GD_CFG_SEQ_BLOCK_ITEM_COUNT - 1));
}

TEST_F(SeqTest, remove_trunk_last_with_more_left) {
    fill(GD_CFG_SEQ_BLOCK_ITEM_COUNT - 1);
    gd_cfg_t l0 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t l1 = gd_cfg_seq_add_struct(m_seq);
    gd_cfg_t l2 = gd_cfg_seq_add_struct(m_seq);

    gd_cfg_free(l0);

    EXPECT_EQ(GD_CFG_SEQ_BLOCK_ITEM_COUNT + 1, gd_cfg_seq_count(m_seq));
    EXPECT_EQ(l1, gd_cfg_seq_at(m_seq, GD_CFG_SEQ_BLOCK_ITEM_COUNT - 1));
    EXPECT_EQ(l2, gd_cfg_seq_at(m_seq, GD_CFG_SEQ_BLOCK_ITEM_COUNT));
}

