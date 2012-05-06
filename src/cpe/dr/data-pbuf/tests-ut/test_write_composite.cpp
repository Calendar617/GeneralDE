#include "cpe/dr/dr_metalib_manage.h"
#include "WriteTest.hpp"

TEST_F(WriteTest, type_struct_basic) {
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

TEST_F(WriteTest, type_struct_array) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S1' version='1'>"
        "	     <entry name='a1' type='uint32' id='1' count='2'/>"
        "    </struct>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='b1' type='S1' id='3'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(8, write("S2", "b1: { a1: [150, 151] }"));
    
    EXPECT_STREQ(
        "0x1A 0x06 0x0A 0x04 0x96 0x01 0x97 0x01", result());
}

TEST_F(WriteTest, type_union_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <union name='U1' version='1'>"
        "	     <entry name='a1' type='uint32' id='1'/>"
        "	     <entry name='a2' type='uint32' id='2'/>"
        "    </union>"
        "    <struct name='S2' version='1'>"
        "        <entry name='s' type='uint32'/>"
        "	     <entry name='b1' type='U1' id='3' select='s'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(5, write("S2", "b1: { a1: 150 }"));
    
    EXPECT_STREQ(
        "0x1A 0x03 0x08 0x96 0x01", result());
}

TEST_F(WriteTest, type_union_no_select) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <union name='U1' version='1'>"
        "	     <entry name='a1' type='uint32' id='1'/>"
        "	     <entry name='a2' type='uint32' id='2'/>"
        "    </union>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='b1' type='U1' id='3'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(8, write("S2", "b1: { a1: 150 }"));
    
    EXPECT_STREQ(
        "0x1A 0x06 0x08 0x96 0x01 0x10 0x96 0x01"
        , result());
}
