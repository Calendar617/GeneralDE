#include "cpe/dr/dr_ctypes_info.h"
#include "BuildFromXmlTest.hpp"

class BuildFromXmlStructAlign : public BuildFromXmlTest {
};

TEST_F(BuildFromXmlStructAlign, align2_basic) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='S1' version='1' id='33'>"
        "	     <entry name='a1' type='int8'/>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(4, dr_get_meta_size(get_meta("S1")));
}
