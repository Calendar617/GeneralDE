#include "cpe/dr/dr_ctypes_op.h"
#include "CopySameEntryTest.hpp"

TEST_F(CopySameEntryTest, multi_entry_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='a2' type='int16' defaultvalue='67'/>"
        "	     <entry name='a1' type='int16' defaultvalue='23'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    EXPECT_EQ(23, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(67, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
}

TEST_F(CopySameEntryTest, numeric_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='a1' type='int16' defaultvalue='23'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    EXPECT_EQ(23, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
}

TEST_F(CopySameEntryTest, string_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des' version='1'>"
        "	     <entry name='a1' type='string' size='6'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='a1' type='string' size='12' defaultvalue='aaaaaaaaaaa'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    EXPECT_STREQ("aaaaa", (const char *)result());
}

TEST_F(CopySameEntryTest, struct_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des' version='1'>"
        "	     <entry name='a1' type='int16' defaultvalue='33'/>"
        "	     <entry name='a2' type='int16' defaultvalue='34'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='a1' type='int16' defaultvalue='12'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    EXPECT_EQ(12, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(34, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
}

TEST_F(CopySameEntryTest, struct_struct_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des-i' version='1'>"
        "	     <entry name='b1' type='int16'/>"
        "    </struct>"
        "    <struct name='des' version='1'>"
        "	     <entry name='a1' type='des-i'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "    <struct name='src-i' version='1'>"
        "	     <entry name='b1' type='int16' defaultvalue='23'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='a2' type='int16' defaultvalue='67'/>"
        "	     <entry name='a1' type='src-i'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    EXPECT_EQ(23, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(67, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
}

TEST_F(CopySameEntryTest, struct_array_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des' version='1'>"
        "	     <entry name='a1' type='int16' count='8'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='a2' type='int16' defaultvalue='67'/>"
        "	     <entry name='a1' type='int16' defaultvalue='23' count='12'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    for(int i = 0; i < 8; ++i) {
        EXPECT_EQ(23, dr_ctype_read_int16(result(i * 2), CPE_DR_TYPE_INT16));
    }
    EXPECT_EQ(67, dr_ctype_read_int16(result(8 * 2), CPE_DR_TYPE_INT16));
}

TEST_F(CopySameEntryTest, struct_array_des_refer) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des' version='1'>"
        "	     <entry name='count' type='int16'/>"
        "	     <entry name='a1' type='int16' count='8' refer='count'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='a2' type='int16' defaultvalue='67'/>"
        "	     <entry name='a1' type='int16' defaultvalue='23' count='12'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    EXPECT_EQ(8, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    for(int i = 1; i < 9; ++i) {
        EXPECT_EQ(23, dr_ctype_read_int16(result(i * 2), CPE_DR_TYPE_INT16));
    }
    EXPECT_EQ(67, dr_ctype_read_int16(result(9 * 2), CPE_DR_TYPE_INT16));
}

TEST_F(CopySameEntryTest, struct_array_src_refer) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des' version='1'>"
        "	     <entry name='a1' type='int16' count='8' defaultvalue='55'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='count' type='int16' defaultvalue='2'/>"
        "	     <entry name='a2' type='int16' defaultvalue='67'/>"
        "	     <entry name='a1' type='int16' defaultvalue='23' count='12' refer='count'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    for(int i = 0; i < 2; ++i) {
        EXPECT_EQ(23, dr_ctype_read_int16(result(i * 2), CPE_DR_TYPE_INT16));
    }
    for(int i = 2; i < 8; ++i) {
        EXPECT_EQ(55, dr_ctype_read_int16(result(i * 2), CPE_DR_TYPE_INT16));
    }
    EXPECT_EQ(67, dr_ctype_read_int16(result(8 * 2), CPE_DR_TYPE_INT16));
}

TEST_F(CopySameEntryTest, struct_array_both_refer) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des' version='1'>"
        "	     <entry name='count' type='int16' defaultvalue='3'/>"
        "	     <entry name='a1' type='int16' count='8' defaultvalue='55'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='count' type='int16' defaultvalue='2'/>"
        "	     <entry name='a2' type='int16' defaultvalue='67'/>"
        "	     <entry name='a1' type='int16' defaultvalue='23' count='12' refer='count'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    EXPECT_EQ(2, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    for(int i = 1; i < 3; ++i) {
        EXPECT_EQ(23, dr_ctype_read_int16(result(i * 2), CPE_DR_TYPE_INT16));
    }
    for(int i = 3; i < 9; ++i) {
        EXPECT_EQ(55, dr_ctype_read_int16(result(i * 2), CPE_DR_TYPE_INT16));
    }
    EXPECT_EQ(67, dr_ctype_read_int16(result(9 * 2), CPE_DR_TYPE_INT16));
}

TEST_F(CopySameEntryTest, struct_array_struct_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net' version='1'>"
        "    <struct name='des-i' version='1'>"
        "	     <entry name='b1' type='int16'/>"
        "    </struct>"
        "    <struct name='des' version='1'>"
        "	     <entry name='a1' type='des-i' count='8'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "    <struct name='src-i' version='1'>"
        "	     <entry name='b1' type='int16' defaultvalue='23'/>"
        "    </struct>"
        "    <struct name='src' version='1'>"
        "	     <entry name='a2' type='int16' defaultvalue='67'/>"
        "	     <entry name='a1' type='src-i' count='12'/>"
        "    </struct>"
        "</metalib>");

    copy("des", "src");

    for(int i = 0; i < 8; ++i) {
        EXPECT_EQ(23, dr_ctype_read_int16(result(i * 2), CPE_DR_TYPE_INT16));
    }
    EXPECT_EQ(67, dr_ctype_read_int16(result(8 * 2), CPE_DR_TYPE_INT16));
}

TEST_F(CopySameEntryTest, union_with_selector) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='A-i' version='1'>"
        "	     <entry name='a1' type='int32' defaultvalue='9'/>"
        "	     <entry name='a2' type='int16' defaultvalue='10'/>"
        "    </struct>"
        "    <struct name='B-i' version='1'>"
        "	     <entry name='b1' type='int16' defaultvalue='11'/>"
        "	     <entry name='b2' type='int16' defaultvalue='12'/>"
        "    </struct>"
        "    <union name='U-i' version='1'>"
        "	     <entry name='u1' type='A-i' id='2'/>"
        "	     <entry name='u2' type='B-i' id='3'/>"
        "    </union>"
        "    <struct name='S-i' version='1'>"
        "	     <entry name='type' type='int16' defaultvalue='2'/>"
        "        <entry name='data' type='U-i' select='type'/>"
        "    </struct>"
        "    <struct name='A-d' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int32'/>"
        "    </struct>"
        "    <struct name='B-d' version='1'>"
        "	     <entry name='b1' type='int32'/>"
        "	     <entry name='b2' type='int32'/>"
        "    </struct>"
        "    <union name='U-d' version='1'>"
        "	     <entry name='u1' type='A-d' id='20'/>"
        "	     <entry name='u2' type='B-d' id='30'/>"
        "    </union>"
        "    <struct name='S-d' version='1'>"
        "	     <entry name='type' type='int32'/>"
        "        <entry name='data' type='U-d' select='type'/>"
        "    </struct>"
        "</metalib>");

    copy("S-d", "S-i");

    EXPECT_EQ(20, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT32));
    EXPECT_EQ(9, dr_ctype_read_int16(result(4), CPE_DR_TYPE_INT16));
    EXPECT_EQ(10, dr_ctype_read_int16(result(6), CPE_DR_TYPE_INT16));
}

