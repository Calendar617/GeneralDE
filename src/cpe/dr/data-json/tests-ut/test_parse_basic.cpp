#include "ParseTest.hpp"

TEST_F(ParseTest, metalib_basic) {
    installMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='1'>"
        "    <struct name='S' version='1'>"
        "	     <entry name='a1' type='smallint'/>"
        "    </struct>"
        "</metalib>"
        );

    struct {
        int16_t a1;
    } expect = { 12 };

    ASSERT_EQ(0, read("{ \"a1\" : 12}", "S", 1));

    ASSERT_JSON_READ_RESULT(expect);
}
