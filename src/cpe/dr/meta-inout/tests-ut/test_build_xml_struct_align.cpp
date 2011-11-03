#include "cpe/dr/dr_ctypes_info.h"
#include "BuildFromXmlTest.hpp"

class BuildFromXmlStructAlign : public BuildFromXmlTest {
};

TEST_F(BuildFromXmlStructAlign, struct_data) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='S1' version='1' id='33'>"
        "	     <entry name='a1' type='char'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ(1, dr_get_meta_size(get_meta("S1")));
}
