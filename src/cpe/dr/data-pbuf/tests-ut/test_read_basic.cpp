#include "ReadTest.hpp"

TEST_F(ReadTest, type_uin32) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='uint32' id='1'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(4, read("S", "a1: 150"));

    EXPECT_CFG_EQ(
        "count: 3\n"
        "a1: [12, 13, 14]"
        ,
        result());
}
