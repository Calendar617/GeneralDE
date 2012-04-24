#include "cpe/dr/dr_metalib_manage.h"
#include "WriteTest.hpp"

TEST_F(WriteTest, type_string) {
    installMeta(
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='string' size='5'/>"
        "    </struct>"
        "</metalib>"
        );
}
