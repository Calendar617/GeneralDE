#include <sstream>
#include "cpe/dr/dr_ctypes_op.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "ParseTest.hpp"

TEST_F(ParseTest, array_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='count' type='int16'/>"
        "	     <entry name='data' type='int16' count='16' refer='count'/>"
        "	     <entry name='last' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_TRUE(t_em_no_error());

    EXPECT_EQ(
        metaSize("S2"), read("{ \"count\": 2, \"data\" : [ 12 , 14 ], \"last\": 33 }", "S2"));

    EXPECT_EQ(2, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(12, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
    EXPECT_EQ(14, dr_ctype_read_int16(result(4), CPE_DR_TYPE_INT16));
    EXPECT_EQ(33, dr_ctype_read_int16(result(34), CPE_DR_TYPE_INT16));
}

TEST_F(ParseTest, array_auto_count) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='count' type='int16'/>"
        "	     <entry name='data' type='int16' count='16' refer='count'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_TRUE(t_em_no_error());

    EXPECT_EQ(6, read("{ \"data\" : [ 12, 14 ] }", "S2"));

    EXPECT_EQ(2, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
}

TEST_F(ParseTest, array_overflow) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='count' type='int16'/>"
        "	     <entry name='data' type='int16' count='2' refer='count'/>"
        "	     <entry name='last' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_TRUE(t_em_no_error());

    EXPECT_EQ(-1, read("{ \"data\" : [ 12 , 14, 16 ], \"last\": 33 }", "S2"));

    EXPECT_EQ(2, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(12, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
    EXPECT_EQ(14, dr_ctype_read_int16(result(4), CPE_DR_TYPE_INT16));
    EXPECT_EQ(33, dr_ctype_read_int16(result(6), CPE_DR_TYPE_INT16));
}

TEST_F(ParseTest, array_struct_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='count' type='int16'/>"
        "	     <entry name='data' type='S' count='16' refer='count'/>"
        "	     <entry name='last' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_TRUE(t_em_no_error());

    ASSERT_EQ(metaSize("S2"), read("{ \"count\": 2, \"data\" : [ { \"a1\" : 12 }, {\"a1\" : 14 } ], \"last\": 33 }" , "S2"));

    EXPECT_EQ(2, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
    EXPECT_EQ(12, dr_ctype_read_int16(result(2), CPE_DR_TYPE_INT16));
    EXPECT_EQ(14, dr_ctype_read_int16(result(4), CPE_DR_TYPE_INT16));
    EXPECT_EQ(33, dr_ctype_read_int16(result(34), CPE_DR_TYPE_INT16));
}

TEST_F(ParseTest, array_struct_auto_count) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='count' type='int16'/>"
        "	     <entry name='data' type='S' count='16' refer='count'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_TRUE(t_em_no_error());

    ASSERT_EQ(6, read("{ \"data\" : [ { \"a1\" : 12 }, {\"a1\" : 14 } ], \"last\": 33 }" , "S2"));

    EXPECT_EQ(2, dr_ctype_read_int16(result(), CPE_DR_TYPE_INT16));
}

