#include "../generate_ops.h"
#include "GenerateTest.hpp"

TEST_F(GenerateTest, basic) {
    add_buffer(
        "a",
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' desc='PkgHead.desc' version='1' id='33'>"
        "	     <entry name='a1' type='int8'/>"
        "    </struct>"
        "</metalib>");

    EXPECT_STREQ(
        "#ifndef DR_GENERATED_H_NET_A_INCLEDED\n"
        "#define DR_GENERATED_H_NET_A_INCLEDED\n"
        "\n"
        "#ifdef __cplusplus\n"
        "extern \"C\" {\n"
        "#endif\n"
        "\n"
        "#ifdef __cplusplus\n"
        "}\n"
        "#endif\n"
        "\n"
        "#endif\n"
        ,
        generate_h("a"));
}
