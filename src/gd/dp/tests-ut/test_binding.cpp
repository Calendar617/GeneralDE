#include "DpTest.hpp"

TEST_F(DpTest, rsp_create_basic) {
    gd_dp_rsp_t rsp = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp);
    EXPECT_STREQ("rsp1", gd_dp_rsp_name(rsp));
    EXPECT_TRUE(NULL == gd_dp_rsp_processor(rsp));
}

TEST_F(DpTest, rsp_create_duplicate) {
    gd_dp_rsp_t rsp = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp);
    EXPECT_STREQ("rsp1", gd_dp_rsp_name(rsp));

    EXPECT_TRUE(NULL == gd_dp_rsp_create(t_dp(), "rsp1"));
}

TEST_F(DpTest, find_byName_basic) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_by_name(t_dp(), "rsp1"));
    EXPECT_TRUE(rsp2 == gd_dp_rsp_find_by_name(t_dp(), "rsp2"));
}

TEST_F(DpTest, binding_numeric_create_basic) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 0, t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_numeric(t_dp(), 0));
}

TEST_F(DpTest, binding_numeric_create_multi) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 0, t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp2, 1, t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_numeric(t_dp(), 0));
    EXPECT_TRUE(rsp2 == gd_dp_rsp_find_first_by_numeric(t_dp(), 1));
}

TEST_F(DpTest, binding_numeric_create_multi_to_same) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 0, t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 1, t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_numeric(t_dp(), 0));
    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_numeric(t_dp(), 1));
}

TEST_F(DpTest, binding_numeric_create_duplicate) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 0, t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp2, 0, t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_numeric(t_dp(), 0));
}

TEST_F(DpTest, binding_string_create_basic) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd1", t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));
}

TEST_F(DpTest, binding_string_create_multi) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd1", t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp2, "cmd2", t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));
    EXPECT_TRUE(rsp2 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd2"));
}

TEST_F(DpTest, binding_string_create_multi_to_same) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd1", t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd2", t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));
    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd2"));
}

TEST_F(DpTest, binding_string_create_duplicate) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd1", t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp2, "cmd1", t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));
}

TEST_F(DpTest, rsp_free_with_binding) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd1", t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd2", t_em()));

    gd_dp_rsp_free(rsp1);


    EXPECT_TRUE(NULL == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));
    EXPECT_TRUE(NULL == gd_dp_rsp_find_first_by_string(t_dp(), "cmd2"));
}

TEST_F(DpTest, unbinding_string_basic) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd1", t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd2", t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));

    EXPECT_EQ(1, gd_dp_mgr_unbind_string(t_dp(), "cmd1"));
    EXPECT_TRUE(NULL == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));
    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd2"));
}

TEST_F(DpTest, unbinding_string_not_exist) {
    EXPECT_EQ(0, gd_dp_mgr_unbind_string(t_dp(), "cmd1"));
}

TEST_F(DpTest, unbinding_numeric_basic) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 0, t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 1, t_em()));

    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_numeric(t_dp(), 0));

    EXPECT_EQ(1, gd_dp_mgr_unbind_numeric(t_dp(), 0));
    EXPECT_TRUE(NULL == gd_dp_rsp_find_first_by_numeric(t_dp(), 0));
    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_numeric(t_dp(), 1));
}

TEST_F(DpTest, unbinding_numeric_not_exist) {
    EXPECT_EQ(0, gd_dp_mgr_unbind_numeric(t_dp(), 0));
}

TEST_F(DpTest, multi_binding_numeric_unbind_all) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 0, t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp2, 0, t_em()));

    EXPECT_EQ(2, gd_dp_mgr_unbind_numeric(t_dp(), 0));
    EXPECT_TRUE(NULL == gd_dp_rsp_find_first_by_numeric(t_dp(), 0));
}

TEST_F(DpTest, multi_binding_numeric_unbind_first) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 0, t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp2, 0, t_em()));

    EXPECT_EQ(1, gd_dp_rsp_unbind_numeric(rsp1, 0));
    EXPECT_TRUE(rsp2 == gd_dp_rsp_find_first_by_numeric(t_dp(), 0));
}

TEST_F(DpTest, multi_binding_numeric_unbind_second) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp1, 0, t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_numeric(rsp2, 0, t_em()));

    EXPECT_EQ(1, gd_dp_rsp_unbind_numeric(rsp2, 0));
    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_numeric(t_dp(), 0));
}

TEST_F(DpTest, multi_binding_string_unbind_all) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd1", t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp2, "cmd1", t_em()));

    EXPECT_EQ(2, gd_dp_mgr_unbind_string(t_dp(), "cmd1"));
    EXPECT_TRUE(NULL == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));
}

TEST_F(DpTest, multi_binding_string_unbind_first) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd1", t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp2, "cmd1", t_em()));

    EXPECT_EQ(1, gd_dp_rsp_unbind_string(rsp1, "cmd1"));
    EXPECT_TRUE(rsp2 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));
}

TEST_F(DpTest, multi_binding_string_unbind_second) {
    gd_dp_rsp_t rsp1 = gd_dp_rsp_create(t_dp(), "rsp1");
    ASSERT_TRUE(rsp1);

    gd_dp_rsp_t rsp2 = gd_dp_rsp_create(t_dp(), "rsp2");
    ASSERT_TRUE(rsp2);

    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp1, "cmd1", t_em()));
    EXPECT_EQ(0, gd_dp_rsp_bind_string(rsp2, "cmd1", t_em()));

    EXPECT_EQ(1, gd_dp_rsp_unbind_string(rsp2, "cmd1"));
    EXPECT_TRUE(rsp1 == gd_dp_rsp_find_first_by_string(t_dp(), "cmd1"));
}

