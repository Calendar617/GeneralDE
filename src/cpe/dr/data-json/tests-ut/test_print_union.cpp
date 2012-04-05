#include "PrintTest.hpp"

TEST_F(PrintTest, print_union_no_selector) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <union name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int32'/>"
        "    </union>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='m_s' type='S'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

#pragma pack(push,1)
    struct {
        union {
            int16_t a1;
            int32_t a2;
        } m_s;
        int16_t a2;
    } input = { { 13 }, 14  };
#pragma pack(pop)

    EXPECT_EQ(0, print(&input, "S2"));
    EXPECT_STREQ("{\"m_s\":{\"a1\":13},\"a2\":14}", result());
}
