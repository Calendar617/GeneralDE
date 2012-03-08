#include "BuilderTest.hpp"

class BuilderBasicTest : public BuilderTest {
};

TEST_F(BuilderBasicTest, add_file_basic) {
    dr_metalib_source_t source = dr_metalib_builder_add_file(m_builder, "b/a.xml");
    ASSERT_TRUE(source);

    EXPECT_EQ(dr_metalib_source_type_file, dr_metalib_source_type(source));
    EXPECT_EQ(dr_metalib_source_format_xml, dr_metalib_source_format(source));
    EXPECT_EQ(dr_metalib_source_from_user, dr_metalib_source_from(source));
    EXPECT_EQ(dr_metalib_source_state_not_analize, dr_metalib_source_state(source));

    EXPECT_STREQ("a", dr_metalib_source_name(source));
    EXPECT_STREQ("b/a.xml", dr_metalib_source_file(source));
}

TEST_F(BuilderBasicTest, add_file_prefix_unknown) {
    dr_metalib_source_t source = dr_metalib_builder_add_file(m_builder, "b/a.aa");
    ASSERT_TRUE(source == NULL);
}

TEST_F(BuilderBasicTest, source_find_basic) {
    dr_metalib_source_t source = dr_metalib_builder_add_file(m_builder, "b/a.xml");
    ASSERT_TRUE(source == dr_metalib_source_find(m_builder, "a"));
}

TEST_F(BuilderBasicTest, source_find_not_exist) {
    ASSERT_TRUE(NULL == dr_metalib_source_find(m_builder, "a"));
}

