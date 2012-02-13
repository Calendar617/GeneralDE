#include "cpe/dr/dr_ctypes_op.h"
#include "SetDefaultsTest.hpp"

TEST_F(SetDefaultsTest, multi_entry_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16' defaultvalue='23'/>"
        "	     <entry name='a2' type='int16' defaultvalue='45'/>"
        "    </struct>"
        "</metalib>");

    set_defaults("S");

    EXPECT_EQ(23, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(45, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
}

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

TEST_F(SetDefaultsTest, numeric_no_default_ignore) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "</metalib>");

    set_defaults("S", DR_SET_DEFAULTS_POLICY_NO_DEFAULT_IGNORE);

    EXPECT_NE(0, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
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

TEST_F(SetDefaultsTest, sequence_value_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16' count='4' defaultvalue='23'/>"
        "    </struct>"
        "</metalib>");

    set_defaults("S");

    EXPECT_EQ(23, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(23, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
    EXPECT_EQ(23, dr_ctype_read_int16(result(4), CPE_DR_TYPE_INT16));
    EXPECT_EQ(23, dr_ctype_read_int16(result(6), CPE_DR_TYPE_INT16));
}

TEST_F(SetDefaultsTest, sequence_struct_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='S1' version='1'>"
        "	     <entry name='b1' type='int16' defaultvalue='23'/>"
        "    </struct>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='S1' count='4'/>"
        "    </struct>"
        "</metalib>");

    set_defaults("S");

    for(int i = 0; i < 4; ++i) {
        EXPECT_EQ(23, dr_ctype_read_int16(result(i * 2), CPE_DR_TYPE_INT16));
    }
}

TEST_F(SetDefaultsTest, sequence_struct_sequence) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='S1' version='1'>"
        "	     <entry name='b1' type='int16' count='5' defaultvalue='23'/>"
        "    </struct>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='S1' count='4'/>"
        "    </struct>"
        "</metalib>");

    set_defaults("S");

    for(int i = 0; i < 20; ++i) {
        EXPECT_EQ(23, dr_ctype_read_int16(result(i * 2), CPE_DR_TYPE_INT16));
    }
}
