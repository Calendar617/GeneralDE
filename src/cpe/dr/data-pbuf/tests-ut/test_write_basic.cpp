#include "cpe/dr/dr_metalib_manage.h"
#include "WriteTest.hpp"

TEST_F(WriteTest, type_string) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='int32' id='1'/>"
        "    </struct>"
        "</metalib>"
        );

    write("S", "a1: 1");

    EXPECT_STREQ(
        "0x08 0x96 0x01", result());
}
