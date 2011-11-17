#include "PrintTest.hpp"

#define DEF_PRINT_TYPED_TEST(__typeName, __type, __input, __expect)     \
    TEST_F(PrintTest, print_ ## __typeName ) {                        \
    installMeta(                                                        \
    "<metalib tagsetversion='1' name='net'  version='1'>"               \
    "    <struct name='S' version='1'>"                                 \
    "	     <entry name='a1' type='" #__typeName "'/>"                 \
    "    </struct>"                                                     \
    "</metalib>"                                                        \
        );                                                              \
                                                                        \
    struct {                                                            \
        __type a1;                                                      \
    } input = { __input };                                              \
                                                                        \
    EXPECT_EQ(0, print(&input, "S"));                                   \
    EXPECT_STREQ("{\"a1\":" __expect "}", result());                    \
}

DEF_PRINT_TYPED_TEST(int8, int8_t, 12, "12")
DEF_PRINT_TYPED_TEST(uint8, uint8_t, 12, "12")
DEF_PRINT_TYPED_TEST(int16, int16_t, 12, "12")
DEF_PRINT_TYPED_TEST(uint16, uint16_t, 12, "12")
DEF_PRINT_TYPED_TEST(int32, int32_t, 12, "12")
DEF_PRINT_TYPED_TEST(uint32, uint32_t, 12, "12")
DEF_PRINT_TYPED_TEST(char, char, 'a', "\"a\"")
DEF_PRINT_TYPED_TEST(uchar, unsigned char, 'a', "\"a\"")

TEST_F(PrintTest, print_struct) {
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
