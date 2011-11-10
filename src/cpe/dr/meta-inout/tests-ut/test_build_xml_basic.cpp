#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "BuildFromXmlTest.hpp"

TEST_F(BuildFromXmlTest, metalib_basic) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'/>");

    ASSERT_EQ(0, errorCount());

    ASSERT_STREQ("net", dr_get_metalib_name(m_metaLib));
    ASSERT_EQ(10, dr_get_metalib_version(m_metaLib));
    ASSERT_EQ(0, dr_get_metalib_macro_num(m_metaLib));
}

TEST_F(BuildFromXmlTest, metalib_version_not_exist) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'/>");

    ASSERT_TRUE(haveError(CPE_DR_ERROR_NO_VERSION));
}

TEST_F(BuildFromXmlTest, metalib_name_not_exist) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' version='1'/>");

    ASSERT_TRUE(haveError(CPE_DR_ERROR_METALIB_ROOT_NO_NAME));
}


TEST_F(BuildFromXmlTest, xml_format_error) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
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
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='%s' version='20'/>",
        name);

    EXPECT_EQ(
        CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT,
        parseMeta(buf));

    ASSERT_TRUE(haveError(CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT));
}
