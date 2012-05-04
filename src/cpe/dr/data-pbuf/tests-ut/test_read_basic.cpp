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
        "a1: 150"
        ,
        result());
}

TEST_F(ReadTest, type_in32) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int32' id='1'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(4, read("S", "a1: 150"));

    EXPECT_CFG_EQ(
        "a1: 150"
        ,
        result());
}

