#include "gtest/gtest.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "with_InputMetaLibTest.hpp"

struct MicroData {
    int index;
    const char * name;
    int value;
    const char * desc;
};

class MetaLibManagerMicroTest : public ::testing::TestWithParam<MicroData>, public WithInputMetaLibTest {
    virtual void SetUp() {
        loadLib();
    }

    virtual void TearDown() {
        freeLib();
    }
};

TEST_P(MetaLibManagerMicroTest, CheckData) {
    MicroData macro = GetParam();

    LPDRMACRO pMacro = dr_get_metalib_macro_by_index(m_lib, macro.index);
    ASSERT_TRUE(pMacro) << "get macro at " << macro.index << " fail!";

    ASSERT_STREQ(macro.name, dr_get_macro_name_by_ptr(m_lib, pMacro));

    int checkValue = -1;
    ASSERT_EQ(0, dr_get_macro_value_by_ptr(&checkValue, pMacro)) << "call dr_get_macro_value_by_ptr fail!";

    ASSERT_EQ(macro.value, checkValue);

    ASSERT_STREQ(macro.desc, dr_get_macro_desc_by_ptr(m_lib, pMacro));
}

MicroData macroDataCases[] = {
    {0, "VERSION", 100, "VERSION.desc"}
    , {1, "MAX_BODY_LEN", 32000, ""}
    , {2, "CMD_LOGIN", 0, ""}
    , {3, "CMD_LOGOUT", 1, ""}
    , {4, "MAX_NAME_LEN", 12, ""}
    , {5, "MAX_PASS_LEN", 32, ""}
    , {6, "MAX_ATTR_SIZE", 128, ""}
};

INSTANTIATE_TEST_CASE_P(
    CheckAllMacroData,
    MetaLibManagerMicroTest,
    testing::ValuesIn(macroDataCases));
