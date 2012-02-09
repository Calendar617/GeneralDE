#include "cpe/dr/dr_ctypes_op.h"
#include "SetDefaultsTest.hpp"

TEST_F(SetDefaultsTest, numeric_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16' defaultvalue='23'/>"
        "    </struct>"
        "</metalib>");

    set_defaults("S");

    EXPECT_EQ(23, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
}

TEST_F(SetDefaultsTest, numeric_no_default) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "</metalib>");

    set_defaults("S");

    EXPECT_EQ(0, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
}

TEST_F(SetDefaultsTest, string_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='string' size='6' defaultvalue='23'/>"
        "    </struct>"
        "</metalib>");

    set_defaults("S");

    EXPECT_STREQ("23", (const char *)result());
}

TEST_F(SetDefaultsTest, string_no_default) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='string' size='6'/>"
        "    </struct>"
        "</metalib>");

    set_defaults("S");

    EXPECT_STREQ("", (const char *)result());
}
