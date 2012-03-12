#include "../om_page_head.h"
#include "ClassTest.hpp"

TEST_F(ClassTest, create_first_basic) {
    gd_om_class_id_t classId = addClass("class1", 11, 256, 4);

    EXPECT_EQ(1, classId);

    struct gd_om_class * cls = gd_om_class_get(&m_classMgr, classId);
    ASSERT_TRUE(cls);

    EXPECT_STREQ("class1", cpe_hs_data(cls->m_name));
    EXPECT_EQ((size_t)12, cls->m_object_size);
    EXPECT_EQ((size_t)256, cls->m_page_size);
    EXPECT_EQ((size_t)20, cls->m_object_per_page);
    EXPECT_EQ((size_t)12, cls->m_object_buf_begin_in_page);
}

TEST_F(ClassTest, create_name_max) {
    char max_name[GD_OM_MAX_TYPENAME_LEN + 1];
    memset(max_name, 'a', GD_OM_MAX_TYPENAME_LEN);
    max_name[GD_OM_MAX_TYPENAME_LEN] = 0;

    gd_om_class_id_t classId = addClass(max_name, 11, 256, 4);
    EXPECT_EQ(1, classId);

    struct gd_om_class * cls = gd_om_class_get(&m_classMgr, classId);
    ASSERT_TRUE(cls);

    EXPECT_STREQ(max_name, cpe_hs_data(cls->m_name));
}

TEST_F(ClassTest, create_name_too_long) {
    char max_name[GD_OM_MAX_TYPENAME_LEN + 2];
    memset(max_name, 'a', GD_OM_MAX_TYPENAME_LEN + 1);
    max_name[GD_OM_MAX_TYPENAME_LEN + 1] = 0;

    EXPECT_EQ(GD_OM_INVALID_CLASSID, addClass(max_name, 11, 256, 4));

    EXPECT_TRUE(t_em_have_errno(gd_om_class_name_too_long));
}

TEST_F(ClassTest, create_align_invalid) {
    EXPECT_EQ(GD_OM_INVALID_CLASSID, addClass("class1", 11, 256, 3));
    EXPECT_TRUE(NULL == gd_om_class_get(&m_classMgr, 1));

    ASSERT_TRUE(t_em_have_errno(gd_om_invalid_align));
}

TEST_F(ClassTest, create_page_too_small) {
    EXPECT_EQ(GD_OM_INVALID_CLASSID, addClass("class1", 11, 128, 4));
    EXPECT_TRUE(NULL == gd_om_class_get(&m_classMgr, 1));

    ASSERT_TRUE(t_em_have_errno(gd_om_page_size_too_small));
}

TEST_F(ClassTest, create_page_max) {
    EXPECT_EQ(1, addClass("class1", 11, 0x1FFFFFFF, 4));
}

TEST_F(ClassTest, create_page_overflow) {
    EXPECT_EQ(GD_OM_INVALID_CLASSID, addClass("class1", 11, 0x20000000, 4));
    ASSERT_TRUE(t_em_have_errno(gd_om_page_size_too_big));
}

TEST_F(ClassTest, create_class_overflow) {
    for(int i = 0; i < GD_OM_MAX_TYPE_COUNT; ++i) {
        char buf[256];
        snprintf(buf, 256, "class%d", i);
        EXPECT_EQ((gd_om_class_id_t)(i + 1), addClass(buf, 11, 256, 4));
    }

    EXPECT_EQ(GD_OM_INVALID_CLASSID, addClass("class1", 11, 246, 4));
    ASSERT_TRUE(t_em_have_errno(gd_om_class_overflow));
}

TEST_F(ClassTest, create_class_name_duplicate) {
    EXPECT_EQ(1, addClass("class1", 11, 246, 4));
    EXPECT_EQ(GD_OM_INVALID_CLASSID, addClass("class1", 11, 246, 4));
    ASSERT_TRUE(t_em_have_errno(gd_om_class_name_duplicate));
}

TEST_F(ClassTest, create_find_basic) {
    gd_om_class_id_t classId = addClass("class1", 11, 256, 4);
    EXPECT_EQ(1, classId);

    struct gd_om_class * cls = gd_om_class_get(&m_classMgr, classId);
    ASSERT_TRUE(cls);

    CPE_HS_DEF_VAR(name, "class1");
    EXPECT_TRUE(cls == gd_om_class_find(&m_classMgr, name));
}

TEST_F(ClassTest, create_find_basic_not_exist) {
    CPE_HS_DEF_VAR(name, "class1");
    EXPECT_TRUE(NULL == gd_om_class_find(&m_classMgr, name));
}

TEST_F(ClassTest, add_new_page_basic) {
    gd_om_class_id_t classId = addClass("class1", 11, 256, 4);
    EXPECT_EQ(1, classId);

    struct gd_om_class * cls = gd_om_class_get(&m_classMgr, classId);
    ASSERT_TRUE(cls);

    char page_buf[256];
    gd_om_data_page_head_init(page_buf);
    ASSERT_EQ(0, gd_om_class_add_page(cls, &page_buf, t_em()));

    EXPECT_EQ((size_t)1, cls->m_page_array_size);

    struct gd_om_data_page_head * head = (struct gd_om_data_page_head *)page_buf;

    EXPECT_EQ(GD_OM_PAGE_MAGIC, head->m_magic);
    EXPECT_EQ(classId, head->m_classId);
    EXPECT_EQ(0, head->m_reserve);
    EXPECT_EQ(0, head->m_page_idx);
    EXPECT_EQ(0, head->m_reserve2);
}
