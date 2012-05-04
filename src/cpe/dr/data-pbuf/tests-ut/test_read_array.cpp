#include "ReadTest.hpp"

TEST_F(ReadTest, array_uin32) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='uint32' id='1' count='5'/>"
        "    </struct>"
        "</metalib>"
        );

    t_em_set_print();
    EXPECT_EQ(20, read("S", "a1: [1, 2, 3, 4, 5]"));

    EXPECT_CFG_EQ(
        "a1: [1, 2, 3, 4, 5]"
        ,
        result());
}

TEST_F(ReadTest, array_struct) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='I' version='1'>"
        "	     <entry name='a1' type='uint32' id='1'/>"
        "    </struct>"
        "    <struct name='S' version='1'>"
        "	     <entry name='b1' type='I' id='1' count='2'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(8, read("S", "b1: [ { a1: 150 }, { a1: 151 } ]"));

    EXPECT_CFG_EQ(
        "b1: [ { a1: 150 }, { a1: 151 } ]"
        ,
        result());
}

TEST_F(ReadTest, array_with_refer_struct) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='I' version='1'>"
        "	     <entry name='a1' type='uint32' id='1'/>"
        "    </struct>"
        "    <struct name='S' version='1'>"
        "        <entry name='count' type='int32'/>"
        "	     <entry name='b1' type='I' id='1' count='2' refer='count'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(12, read("S", "b1: [ { a1: 150 }, { a1: 151 } ]"));

    EXPECT_CFG_EQ(
        "count: 2\n"
        "b1: [ { a1: 150 }, { a1: 151 } ]"
        ,
        result());
}
