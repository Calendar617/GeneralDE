#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "BuildFromXmlTest.hpp"

TEST_F(BuildFromXmlTest, metalib_basic) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'/>");

    ASSERT_EQ(0, errorCount());

    ASSERT_STREQ("net", dr_get_metalib_name(m_metaLib));
    ASSERT_EQ(10, dr_get_metalib_version(m_metaLib));
    ASSERT_EQ(0, dr_get_metalib_macro_num(m_metaLib));
}

TEST_F(BuildFromXmlTest, metalib_tagsetversion_overflow) {
    EXPECT_EQ(
        CPE_DR_ERROR_INVALID_TAGSET_VERSION,
        parseMeta(
        "<metalib tagsetversion='1000000000' name='net' version='1'/>"));
}

TEST_F(BuildFromXmlTest, metalib_tagsetversion_unknown) {
    EXPECT_EQ(
        CPE_DR_ERROR_INVALID_TAGSET_VERSION,
        parseMeta(
        "<metalib tagsetversion='0' name='net' version='1'/>"));

    EXPECT_EQ(
        CPE_DR_ERROR_INVALID_TAGSET_VERSION,
        parseMeta(
        "<metalib tagsetversion='2' name='net' version='1'/>"));
}

TEST_F(BuildFromXmlTest, metalib_version_not_exist) {
    EXPECT_EQ(
        CPE_DR_ERROR_NO_VERSION,
        parseMeta(
            "<metalib tagsetversion='1' name='net'/>"));
}

TEST_F(BuildFromXmlTest, metalib_version_overflow) {
    EXPECT_EQ(
        CPE_DR_ERROR_INVALID_VERSION,
        parseMeta(
        "<metalib tagsetversion='1' name='net' version='2000000000'/>"));
}

TEST_F(BuildFromXmlTest, metalib_name_not_exist) {
    parseMeta(
        "<metalib tagsetversion='1' version='1'/>");

    ASSERT_TRUE(haveError(CPE_DR_ERROR_METALIB_ROOT_NO_NAME));
}


TEST_F(BuildFromXmlTest, xml_format_error) {
    parseMeta(
        "<metalib tagsetversion='1'");

    ASSERT_TRUE(haveError(CPE_DR_ERROR_XML_PARSE));
}

TEST_F(BuildFromXmlTest, metalib_name_overflow) {
    char name[CPE_DR_NAME_LEN + 1];
    for(int i = 0; i < CPE_DR_NAME_LEN; ++i) {
        name[i] = 'a';
    }
    name[CPE_DR_NAME_LEN] = 0;

    char buf[1024];
    snprintf(
        buf, 1024,
        "<metalib tagsetversion='1' name='%s' version='1'/>",
        name);

    EXPECT_EQ(
        CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT,
        parseMeta(buf));

    ASSERT_TRUE(haveError(CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT));
}
