#include "WriteTest.hpp"

TEST_F(WriteTest, basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "</metalib>");

    EXPECT_EQ(
        0,
        write(
            "a1: 12\n"
            "a2: 14\n",
            "S"));

}

TEST_F(WriteTest, seq_count_dynamic_with_refer) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='count' type='int16'/>"
        "	     <entry name='a1' type='int16' count='0' refer='count'/>"
        "    </struct>"
        "</metalib>");

    EXPECT_EQ(
        0,
        write(
            "a1: [12, 13, 14]\n",
            "S"));


    EXPECT_CFG_EQ(
        "count: 3\n"
        "a1: [12, 13, 14]"
        ,
        m_cfg);
}
