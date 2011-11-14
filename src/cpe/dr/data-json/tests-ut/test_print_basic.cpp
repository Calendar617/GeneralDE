#include "PrintTest.hpp"

TEST_F(PrintTest, metalib_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    struct {
        int16_t a1;
    } input = { 12 };

    EXPECT_EQ(0, print(&input, "S"));
    EXPECT_STREQ("{\"a1\":12}", result());
}

TEST_F(PrintTest, metalib_nest) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='m_s' type='S'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

#pragma pack(push,1)
    struct {
        struct {
            int16_t a1;
        } m_s;
        int16_t a2;
    } input = { { 12 }, 14  };
#pragma pack(pop)

    EXPECT_EQ(0, print(&input, "S2"));
    EXPECT_STREQ("{\"m_s\":{\"a1\":12},\"a2\":14}", result());
}
