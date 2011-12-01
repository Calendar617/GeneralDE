#include "ReadTest.hpp"

TEST_F(ReadTest, data_string) {

    EXPECT_STREQ(
        "---\n"
        "a: !<str> 123\n"
        "...\n"
        , result());
}
