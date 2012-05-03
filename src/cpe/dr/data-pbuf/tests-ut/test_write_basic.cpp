#include "cpe/dr/dr_metalib_manage.h"
#include "WriteTest.hpp"

TEST_F(WriteTest, type_uin32) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='uint32' id='1'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(3, write("S", "a1: 150"));

    EXPECT_STREQ(
        "0x08 0x96 0x01", result());
}

TEST_F(WriteTest, type_string) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='string' id='2' size='128'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(9, write("S", "a1: testing"));
    
    EXPECT_STREQ(
        "0x12 0x07 0x74 0x65 0x73 0x74 0x69 0x6E 0x67", result());
}

TEST_F(WriteTest, type_struct) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S1' version='1'>"
        "	     <entry name='a1' type='uint32' id='1'/>"
        "    </struct>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='b1' type='S1' id='3'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(5, write("S2", "b1: { a1: 150 }"));
    
    EXPECT_STREQ(
        "0x1A 0x03 0x08 0x96 0x01", result());
}

TEST_F(WriteTest, type_array_uint32_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='uint32' id='4' count='3'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(8, write("S", "a1: [3, 270, 86942]"));

    EXPECT_STREQ(
        "0x22 0x06 0x03 0x8E 0x02 0x9E 0xA7 0x05", result());
}
