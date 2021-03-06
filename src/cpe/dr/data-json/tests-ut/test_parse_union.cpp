#include "cpe/dr/dr_metalib_manage.h"
#include "ParseTest.hpp"

TEST_F(ParseTest, type_union_no_select_use_large) {
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
    struct T {
        union {
            int16_t a1;
            int32_t a2;
        } m_s;
        int16_t a2;
    };
#pragma pack(pop)

    ASSERT_EQ(
        metaSize("S2"),
        read("{ \"m_s\" : { \"a2\" : 12 }, \"a2\" : 14 }", "S2"));

    struct T * r = (struct T*)result();
    ASSERT_TRUE(r);
    EXPECT_EQ(12, r->m_s.a1);
    EXPECT_EQ(14, r->a2);
}

TEST_F(ParseTest, type_union_no_select_use_small) {
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
    struct T {
        union {
            int16_t a1;
            int32_t a2;
        } m_s;
        int16_t a2;
    };
#pragma pack(pop)

    EXPECT_EQ(
        metaSize("S2"),
        read("{ \"m_s\" : { \"a1\" : 12 }, \"a2\" : 14 }", "S2"));

    struct T * r = (struct T*)result();
    ASSERT_TRUE(r);
    EXPECT_EQ(12, r->m_s.a1);
    EXPECT_EQ(14, r->a2);
}

TEST_F(ParseTest, type_union_no_select_multi_entry) {
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
    } expect = { { 13 }, 14  };
#pragma pack(pop)

    ASSERT_EQ(
        metaSize("S2"),
        read("{ \"m_s\" : { \"a1\" : 12, \"a2\" : 13 }, \"a2\" : 14 }", "S2"));

    ASSERT_JSON_READ_RESULT(expect);
}

TEST_F(ParseTest, type_union_root) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <union name='S' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a2' type='int32'/>"
        "    </union>"
        "</metalib>"
        );

#pragma pack(push,1)
    union {
        int16_t a1;
        int32_t a2;
    } expect = { 12 };
#pragma pack(pop)

    ASSERT_EQ(metaSize("S"), read("{ \"a2\" : 12}", "S"));

    ASSERT_JSON_READ_RESULT(expect);
}

TEST_F(ParseTest, type_union_selector_basic) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <union name='S' version='1'>"
        "	     <entry name='a1' type='int16' id='3'/>"
        "	     <entry name='a2' type='int32' id='4'/>"
        "    </union>"
        "    <struct name='S2' version='1'>"
        "	     <entry name='s' type='int16'/>"
        "	     <entry name='u' type='S' select='s'/>"
        "    </struct>"
        "</metalib>"
        );

#pragma pack(push,1)
    struct T {
        int16_t s;
        union {
            int16_t a1;
            int32_t a2;
        } u;
    };
#pragma pack(pop)

    ASSERT_EQ(
        4,
        read("{ \"s\" : 3, \"u\" : { \"a1\" : 12, \"a2\" : 13 } }", "S2"));

    struct T * r = (struct T*)result();
    ASSERT_TRUE(r);
    EXPECT_EQ(3, r->s);
    EXPECT_EQ(12, r->u.a1);

}

