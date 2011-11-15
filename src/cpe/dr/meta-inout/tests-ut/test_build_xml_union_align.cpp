#include "cpe/dr/dr_ctypes_info.h"
#include "../../dr_internal_types.h"
#include "BuildFromXmlTest.hpp"

class BuildFromXmlUnionAlign : public BuildFromXmlTest {
};

TEST_F(BuildFromXmlUnionAlign, align1_basic) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <union name='S1' version='1' id='33'>"
        "	     <entry name='b1' type='int8'/>"
        "	     <entry name='b2' type='int16'/>"
        "    </union>"
        "    <struct name='S2' version='1' id='34'>"
        "	     <entry name='a1' type='int8'/>"
        "	     <entry name='m_u' type='S1'/>"
        "    </struct>"
        "</metalib>"
        );

#pragma pack(push,1)
    struct {
        char a1;
        union {
            int8_t b1;
            int16_t b2;
        } m_u;
    } data;
#pragma pack(pop)

    EXPECT_EQ(1, dr_meta_align(get_meta("S1")));
    EXPECT_EQ(2, dr_meta_size(get_meta("S1")));

    EXPECT_EQ(0, get_entry("S1", "b1")->m_data_start_pos);
    EXPECT_EQ(0, get_entry("S1", "b2")->m_data_start_pos);

    EXPECT_EQ(1, dr_meta_align(get_meta("S2")));
    EXPECT_EQ(3, dr_meta_size(get_meta("S2")));

    EXPECT_EQ(0, get_entry("S2", "a1")->m_data_start_pos);
    EXPECT_EQ(1, get_entry("S2", "m_u")->m_data_start_pos);
}
