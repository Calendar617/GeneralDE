#include "cpe/dr/dr_metalib_manage.h"
#include "WriteTest.hpp"

TEST_F(WriteTest, type_uin32) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='hello' type='uint32'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(0x10, write("S", "hello: 123"));

    EXPECT_STREQ(
        "0x10 0x00 0x00 0x00 0x10 0x68 0x65 0x6C 0x6C 0x6F 0x00"
        " 0x7B 0x00 0x00 0x00 0x00"
        , result());
}

TEST_F(WriteTest, type_in32) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='hello' type='int32'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(0x10, write("S", "hello: -123"));

    EXPECT_STREQ(
        "0x10 0x00 0x00 0x00 0x10 0x68 0x65 0x6C 0x6C 0x6F 0x00"
        " 0x85 0xFF 0xFF 0xFF 0x00"
        , result());
}

TEST_F(WriteTest, type_string) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='hello' type='string' id='1' size='30'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(0x16, write("S", "hello: world"));

    EXPECT_STREQ(
        "0x16 0x00 0x00 0x00 0x02 0x68 0x65 0x6C 0x6C 0x6F 0x00"
        " 0x06 0x00 0x00 0x00 0x77 0x6F 0x72 0x6C 0x64 0x00 0x00"
        , result());
}

TEST_F(WriteTest, type_array_uint32_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='uint32' count='3'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(35, write("S", "a1: [1, 2, 3]"));

    EXPECT_STREQ(
        "0x23 0x00 0x00 0x00"
        " 0x04"
        " 0x61 0x31 0x00"
        " 0x1A 0x00 0x00 0x00"
        " 0x10 0x30 0x00 0x01 0x00 0x00 0x00" /* '1': 1*/
        " 0x10 0x31 0x00 0x02 0x00 0x00 0x00" /* '2': 2*/
        " 0x10 0x32 0x00 0x03 0x00 0x00 0x00" /* '3': 3*/
        " 0x00"
        " 0x00"
        , result());
}

TEST_F(WriteTest, type_array_refer) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='count' type='uint32'/>"
        "	     <entry name='a1' type='uint32' count='0' refer='count'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(46, write("S", "a1: [1, 2, 3]"));

    EXPECT_STREQ(
        "0x2E 0x00 0x00 0x00"
        " 0x10 0x63 0x6F 0x75 0x6E 0x74 0x00 0x03 0x00 0x00 0x00"
        " 0x04"
        " 0x61 0x31 0x00"
        " 0x1A 0x00 0x00 0x00"
        " 0x10 0x30 0x00 0x01 0x00 0x00 0x00" /* '1': 1*/
        " 0x10 0x31 0x00 0x02 0x00 0x00 0x00" /* '2': 2*/
        " 0x10 0x32 0x00 0x03 0x00 0x00 0x00" /* '3': 3*/
        " 0x00"
        " 0x00"
        , result());
}

// TEST_F(WriteTest, type_array_string_basic) {
//     installMeta(
//         "<metalib tagsetversion='1' name='net'  version='1'>"
//         "    <struct name='S' version='1'>"
//         "	     <entry name='a1' type='string' id='2' size='128' count='3'/>"
//         "    </struct>"
//         "</metalib>"
//         );

//     EXPECT_EQ(30, write("S", "a1: [testing1, testing2, testing3]"));
    
//     EXPECT_STREQ(
//         "0x12 0x08 0x74 0x65 0x73 0x74 0x69 0x6E 0x67 0x31"
//         " 0x12 0x08 0x74 0x65 0x73 0x74 0x69 0x6E 0x67 0x32"
//         " 0x12 0x08 0x74 0x65 0x73 0x74 0x69 0x6E 0x67 0x33"
//         ,
//         result());
// }

// TEST_F(WriteTest, type_array_struct_basic) {
//     installMeta(
//         "<metalib tagsetversion='1' name='net'  version='1'>"
//         "    <struct name='S1' version='1'>"
//         "	     <entry name='a1' type='uint32' id='1'/>"
//         "    </struct>"
//         "    <struct name='S2' version='1'>"
//         "	     <entry name='b1' type='S1' id='3' count='3'/>"
//         "    </struct>"
//         "</metalib>"
//         );

//     EXPECT_EQ(15, write("S2", "b1: [{ a1: 150 }, { a1: 151 }, { a1: 152 }]"));
    
//     EXPECT_STREQ(
//         "0x1A 0x03 0x08 0x96 0x01"
//         " 0x1A 0x03 0x08 0x97 0x01"
//         " 0x1A 0x03 0x08 0x98 0x01"
//         , result());
// }
