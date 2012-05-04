#include "ReadTest.hpp"

TEST_F(ReadTest, struct_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='I' version='1'>"
        "	     <entry name='a1' type='uint32' id='1'/>"
        "    </struct>"
        "    <struct name='S' version='1'>"
        "	     <entry name='b1' type='I' id='1'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(4, read("S", "b1: { a1: 150 }"));

    EXPECT_CFG_EQ(
        "b1: { a1: 150 }"
        ,
        result());
}

