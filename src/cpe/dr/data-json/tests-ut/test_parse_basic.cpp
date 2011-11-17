#include "cpe/dr/dr_metalib_manage.h"
#include "ParseTest.hpp"

#define DEF_PARSE_TYPED_TEST(__typeName, __type, __input, __expect)     \
    TEST_F(ParseTest, type_ ## __typeName) {                            \
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
    } expect = { __expect };                                            \
                                                                        \
    ASSERT_EQ(0, read("{ \"a1\" : " __input "}", "S"));                 \
    ASSERT_JSON_READ_RESULT(expect);                                    \
}

DEF_PARSE_TYPED_TEST(int8, int8_t, "12", 12)
DEF_PARSE_TYPED_TEST(uint8, uint8_t, "12", 12)
DEF_PARSE_TYPED_TEST(int16, int16_t, "12", 12)
DEF_PARSE_TYPED_TEST(uint16, uint16_t, "12", 12)
DEF_PARSE_TYPED_TEST(int32, int32_t, "12", 12)
DEF_PARSE_TYPED_TEST(uint32, uint32_t, "12", 12)
DEF_PARSE_TYPED_TEST(char, char, "\"a\"", 'a')
DEF_PARSE_TYPED_TEST(uchar, unsigned char, "\"a\"", 'a')

TEST_F(ParseTest, type_struct) {
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
    } expect = { { 12 }, 14  };
#pragma pack(pop)

    ASSERT_EQ(0, read("{ \"m_s\" : { \"a1\" : 12 }, \"a2\" : 14 }", "S2"));

    ASSERT_JSON_READ_RESULT(expect);
}

