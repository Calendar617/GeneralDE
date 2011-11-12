#include "gtest/gtest.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_ctypes_info.h"
#include "with_InputMetaLibTest.hpp"

struct Pos2PathCase {
    int size;
    int pos;
    const char * path;
};

class MetaLibManagerPathTest : public ::testing::TestWithParam<Pos2PathCase>, public WithInputMetaLibTest {
public:
    MetaLibManagerPathTest() : m_pMeta(0) {
    }

    virtual void SetUp() {
        loadLib();

        m_pMeta = dr_lib_find_meta_by_name(m_lib, "Pkg");
        ASSERT_TRUE(m_pMeta) << "get meta Pkg fail!";
    }

    virtual void TearDown() {
        m_pMeta = 0;
        freeLib();
    }

    LPDRMETA m_pMeta;
};

TEST_P(MetaLibManagerPathTest, CheckPath) {
    Pos2PathCase caseData = GetParam();

    char buf[1024];
    memset(buf, 0xcc, 1024);

    char * checkPath = dr_meta_off_to_path(m_pMeta, caseData.pos, buf, caseData.size < 0 ? 1024 : caseData.size);

    ASSERT_STREQ(caseData.path, checkPath) << "get path at " << caseData.pos << " error!";
}

Pos2PathCase checkPathCases[] = {
    /*  size pos path*/
    {   -1,  0,  "head.magic"}
    , { 0,   0,  0}
    , { 1,   0,  ""}
    , { 3,   0,  "he"}
    , { 11,   0,  "head.magic"}
    , { -1,   1,  "head.magic"}
    , { -1,   4,  "head.time"}
    , { -1,   11111111,  0}
    , { -1,   123,  "body.login.zone"}
};

INSTANTIATE_TEST_CASE_P(
    CheckGetPathByPos,
    MetaLibManagerPathTest,
    testing::ValuesIn(checkPathCases));
