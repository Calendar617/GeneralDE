#include "ReadTest.hpp"

#define DEF_CFG_YAML_READ_TYPE_TESTCASE(__type, __value)    \
TEST_F(ReadTest, struct_data_ ## __type) {                  \
    EXPECT_EQ(                                              \
        0, read(                                            \
            "a: !" #__type " " #__value "\n"                \
            ));                                             \
                                                            \
    EXPECT_STREQ(                                           \
        "---\n"                                             \
        "a: !" #__type " " #__value "\n"                    \
        "...\n"                                             \
        , result());                                        \
}                                                           \
TEST_F(ReadTest, seq_data_ ## __type) {                     \
    EXPECT_EQ(                                              \
        0, read(                                            \
            "- !" #__type " " #__value "\n"                 \
            ));                                             \
                                                            \
    EXPECT_STREQ(                                           \
        "---\n"                                             \
        "'':\n"                                             \
        "- !" #__type " " #__value "\n"                     \
        "...\n"                                             \
        , result());                                        \
}

DEF_CFG_YAML_READ_TYPE_TESTCASE(int8, -123)
DEF_CFG_YAML_READ_TYPE_TESTCASE(uint8, 123)
DEF_CFG_YAML_READ_TYPE_TESTCASE(int16, -123)
DEF_CFG_YAML_READ_TYPE_TESTCASE(uint16, 123)
DEF_CFG_YAML_READ_TYPE_TESTCASE(int32, -123)
DEF_CFG_YAML_READ_TYPE_TESTCASE(uint32, 123)
DEF_CFG_YAML_READ_TYPE_TESTCASE(int64, -123)
DEF_CFG_YAML_READ_TYPE_TESTCASE(uint64, 123)
