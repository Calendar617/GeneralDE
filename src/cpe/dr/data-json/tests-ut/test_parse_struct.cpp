#include <sstream>
#include "cpe/dr/dr_metalib_manage.h"
#include "ParseTest.hpp"

TEST_F(ParseTest, struct_basic) {
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

TEST_F(ParseTest, struct_ignore_unknown_key) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

#pragma pack(push,1)
    struct {
        int16_t a1;
        int16_t a2;
    } expect = { 12, 14  };
#pragma pack(pop)

    ASSERT_EQ(0, read("{ \"a1\" : 12, \"not-exist\": 15, \"a2\" : 14 }", "S2"));

    ASSERT_JSON_READ_RESULT(expect);
}

TEST_F(ParseTest, struct_ignore_unknown_key_with_nest) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

#pragma pack(push,1)
    struct {
        int16_t a1;
        int16_t a2;
    } expect = { 12, 14  };
#pragma pack(pop)

    ASSERT_EQ(0, read("{ \"a1\" : 12,"
                      " \"not-exist\": { a2: 15 },"
                      " \"a2\" : 14 }", "S2"));

    ASSERT_JSON_READ_RESULT(expect);
}

TEST_F(ParseTest, struct_ignore_nest_not_struct) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

#pragma pack(push,1)
    struct {
        int16_t a1;
        int16_t a2;
    } expect = { 23, 14  };
#pragma pack(pop)

    ASSERT_EQ(0, read("{ \"a1\" : { a2: 15 },"
                      " \"a2\" : 14 }", "S2"));

    EXPECT_EQ(14, expect.a2);
}

TEST_F(ParseTest, struct_ignore_nest_level_2) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

#pragma pack(push,1)
    struct {
        int16_t a1;
        int16_t a2;
    } expect = { 23, 14  };
#pragma pack(pop)

    ASSERT_EQ(0, read("{ \"a1\" : { a2: { a2 : 15 } },"
                      " \"a2\" : 14 }", "S2"));

    EXPECT_EQ(14, expect.a2);
}
