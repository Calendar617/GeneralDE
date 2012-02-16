#include "gtest/gtest.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_ctypes_info.h"
#include "with_InputMetaLibTest.hpp"

struct Pos2PathCase {
    int size;
    int pos;
    const char * path;
};

class MetaLibManagerPath : public ::testing::Test, public WithInputMetaLibTest {
public:
    virtual void SetUp() { loadLib(); }
    virtual void TearDown() { freeLib(); }

    char m_buf[128];
    const char * pos2path(const char * metaName, int pos, int bufSize = -1) {
        return dr_meta_off_to_path(meta(metaName), pos, m_buf, bufSize < 0 ? 128 : bufSize);
    }

    int path2pos(const char * metaName, const char * path) {
        return dr_meta_path_to_off(meta(metaName), path, NULL);
    }
};

TEST_F(MetaLibManagerPath, pos2path_basic) {
    EXPECT_STREQ("head.magic", pos2path("Pkg", 0));
}

TEST_F(MetaLibManagerPath, pos2path_buf_empty) {
    EXPECT_STREQ(NULL, pos2path("Pkg", 0, 0));
}

TEST_F(MetaLibManagerPath, pos2path_buf_size_1) {
    EXPECT_STREQ("", pos2path("Pkg", 0, 1));
}

TEST_F(MetaLibManagerPath, pos2path_buf_size_small) {
    EXPECT_STREQ("he", pos2path("Pkg", 0, 3));
}

TEST_F(MetaLibManagerPath, pos2path_buf_size_enough) {
    EXPECT_STREQ("head.magic", pos2path("Pkg", 0, 11));
}

TEST_F(MetaLibManagerPath, pos2path_pos_at_middle) {
    EXPECT_STREQ("head.magic", pos2path("Pkg", 1));
}

TEST_F(MetaLibManagerPath, pos2path_entry_ad_middle) {
    EXPECT_STREQ("head.time", pos2path("Pkg", 4));
}

TEST_F(MetaLibManagerPath, pos2path_pos_overflow) {
    EXPECT_STREQ(NULL, pos2path("Pkg", 111111));
}

TEST_F(MetaLibManagerPath, pos2path_many_level) {
    EXPECT_STREQ("body.login.zone", pos2path("Pkg", 123));
}

TEST_F(MetaLibManagerPath, path2pos_basic) {
    EXPECT_EQ(0, path2pos("Pkg", "head"));
}

TEST_F(MetaLibManagerPath, path2pos_nested) {
    EXPECT_EQ(0, path2pos("Pkg", "head.magic"));
}

TEST_F(MetaLibManagerPath, path2pos_middle) {
    EXPECT_EQ(2, path2pos("Pkg", "head.version"));
}

TEST_F(MetaLibManagerPath, path2pos_first_not_exist) {
    EXPECT_EQ(-1, path2pos("Pkg", "not-exist.version"));
}

TEST_F(MetaLibManagerPath, path2pos_last_not_exist) {
    EXPECT_EQ(-1, path2pos("Pkg", "head.not-exist"));
}

TEST_F(MetaLibManagerPath, path2pos_many_level) {
    EXPECT_EQ(64, path2pos("Pkg", "body.login.zone"));
}

