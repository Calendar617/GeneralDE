#include "ParseTest.hpp"

TEST_F(ParseTest, metalib_basic) {
    installMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "</metalib>"
        );

    
}
