#include "gtest/gtest.h"
#include "../../dr_ctype_ops.h"

struct CtypeNameIdInfo {
    int m_id;
    const char * name;
};

class CtypeOpsFindByNameTest : public ::testing::TestWithParam<CtypeNameIdInfo> {
};

TEST_P(CtypeOpsFindByNameTest, FindByName) {
    CtypeNameIdInfo caseInfo = GetParam();

    const struct tagDRCTypeInfo * ctypeInfo = dr_find_ctype_info_by_name(caseInfo.name);
    ASSERT_TRUE(dr_find_ctype_info_by_name(caseInfo.name)) << "get ctype by name " << caseInfo.name << " fail!";

    EXPECT_EQ(caseInfo.m_id, ctypeInfo->m_id);
}

CtypeNameIdInfo ctypeCasees[] = {
    {CPE_DR_TYPE_UNION, "union"}
    , {CPE_DR_TYPE_STRUCT, "struct"}
    , {CPE_DR_TYPE_CHAR, "char"}
    , {CPE_DR_TYPE_UCHAR, "uchar"}
    , {CPE_DR_TYPE_BYTE, "byte"}
    , {CPE_DR_TYPE_SMALLINT, "smallint"}
    , {CPE_DR_TYPE_SHORT, "short"}
    , {CPE_DR_TYPE_SMALLUINT, "smalluint"}
    , {CPE_DR_TYPE_USHORT, "ushort"}
    , {CPE_DR_TYPE_INT, "int"}
    , {CPE_DR_TYPE_UINT, "uint"}
    , {CPE_DR_TYPE_LONG, "long"}
    , {CPE_DR_TYPE_ULONG, "ulong"}
    , {CPE_DR_TYPE_LONGLONG, "longlong"}
    , {CPE_DR_TYPE_ULONGLONG, "ulonglong"}
    , {CPE_DR_TYPE_DATE, "date"}
    , {CPE_DR_TYPE_TIME, "time"}
    , {CPE_DR_TYPE_DATETIME, "datetime"}
    , {CPE_DR_TYPE_MONEY, "money"}
    , {CPE_DR_TYPE_FLOAT, "float"}
    , {CPE_DR_TYPE_DOUBLE, "double"}
    , {CPE_DR_TYPE_IP, "ip"}
    , {CPE_DR_TYPE_WCHAR, "wchar"}
    , {CPE_DR_TYPE_STRING, "string"}
    , {CPE_DR_TYPE_WSTRING, "wstring"}
    , {CPE_DR_TYPE_VOID, "void"}
};

INSTANTIATE_TEST_CASE_P(
    CheckName,
    CtypeOpsFindByNameTest,
    testing::ValuesIn(ctypeCasees));
