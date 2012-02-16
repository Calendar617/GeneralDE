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
