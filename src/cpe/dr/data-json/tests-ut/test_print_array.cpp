#include "PrintTest.hpp"

TEST_F(PrintTest, array_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='count' type='int16'/>"
        "	     <entry name='data' type='int16' count='16' refer='count'/>"
        "    </struct>"
        "</metalib>"
        );

#pragma pack(push,1)
    struct {
        int16_t count;
        int16_t data[16];
    } input = { 2, {12, 13, 14}  };
#pragma pack(pop)

    EXPECT_EQ(0, print(&input, "S2"));
    EXPECT_STREQ("{\"count\":2,\"data\":[12,13]}", result());
}

