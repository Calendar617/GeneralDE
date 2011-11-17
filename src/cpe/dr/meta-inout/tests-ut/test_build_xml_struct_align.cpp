#include "cpe/dr/dr_ctypes_info.h"
#include "BuildFromXmlTest.hpp"

class BuildFromXmlStructAlign : public BuildFromXmlTest {
};

TEST_F(BuildFromXmlStructAlign, align1_basic) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='S1' version='1' id='33'>"
        "	     <entry name='a1' type='int8'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(1, dr_meta_align(meta("S1")));
    EXPECT_EQ(3, dr_meta_size(meta("S1")));
}

TEST_F(BuildFromXmlStructAlign, align2_basic) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='S1' version='1' id='33' align='2'>"
        "	     <entry name='a1' type='int8'/>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(4, dr_meta_size(meta("S1")));
}


TEST_F(BuildFromXmlStructAlign, align_composite_2_1) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='S1' version='1' id='33' align='2'>"
        "	     <entry name='a1' type='int8'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "    <struct name='S2' version='1' id='34' align='1'>"
        "	     <entry name='a1' type='int8'/>"
        "	     <entry name='a2' type='S1'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(4, dr_meta_size(meta("S1")));
    EXPECT_EQ(5, dr_meta_size(meta("S2")));
}

TEST_F(BuildFromXmlStructAlign, align_composite_1_2) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='S1' version='1' id='33' align='1'>"
        "	     <entry name='a1' type='int8'/>"
        "	     <entry name='a2' type='int16'/>"
        "    </struct>"
        "    <struct name='S2' version='1' id='34' align='2'>"
        "	     <entry name='a1' type='int8'/>"
        "	     <entry name='a2' type='S1'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(3, dr_meta_size(meta("S1")));
    EXPECT_EQ(4, dr_meta_size(meta("S2")));
}
