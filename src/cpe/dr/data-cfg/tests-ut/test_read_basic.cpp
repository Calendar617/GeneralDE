#include "cpe/dr/dr_ctypes_op.h"
#include "ReadTest.hpp"

TEST_F(ReadTest, struct_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "</metalib>");

    EXPECT_EQ(
        4,
        read(
            "a1: 12\n"
            "a2: 14\n",
            "S"));

    EXPECT_EQ(12, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(14, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
}

TEST_F(ReadTest, seq_size_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16' count='12'/>"
        "	     <entry name='a3' type='int16'/>"
        "    </struct>"
        "</metalib>");

    EXPECT_EQ(
        26,
        read(
            "a1: [12, 14]\n"
            "a3: 34",
            "S"));

    EXPECT_EQ(12, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(14, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
    EXPECT_EQ(0, dr_ctype_read_int16(result(4), CPE_DR_TYPE_INT16));
    EXPECT_EQ(34, dr_ctype_read_int16(result(24), CPE_DR_TYPE_INT16));
}

TEST_F(ReadTest, seq_size_with_refer) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='count' type='int16'/>"
        "	     <entry name='a1' type='int16' count='12' refer='count'/>"
        "	     <entry name='a3' type='int16'/>"
        "    </struct>"
        "</metalib>");

    t_em_set_print();

    EXPECT_EQ(
        28,
        read(
            "a1: [12, 14]\n"
            "a3: 34",
            "S"));

    EXPECT_EQ(2, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(12, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
    EXPECT_EQ(14, dr_ctype_read_int16(result(4), CPE_DR_TYPE_INT16));
    EXPECT_EQ(34, dr_ctype_read_int16(result(26), CPE_DR_TYPE_INT16));
}

TEST_F(ReadTest, seq_size_dft_no_refer) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16' count='12' defaultvalue='23'/>"
        "	     <entry name='a3' type='int16'/>"
        "    </struct>"
        "</metalib>");

    EXPECT_EQ(
        26,
        read(
            "a1: [12, 14]\n"
            "a3: 34",
            "S"));

    EXPECT_EQ(12, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(14, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));

    for(int i = 0; i < 10; ++i) {
        EXPECT_EQ(23, dr_ctype_read_int16(result(4 + i * 2), CPE_DR_TYPE_INT16))
            << "default at " << (i + 2) << " error!";
    }

    EXPECT_EQ(34, dr_ctype_read_int16(result(24), CPE_DR_TYPE_INT16));
}

