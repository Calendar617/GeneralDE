#include "../om_page_head.h"
#include "ClassTest.hpp"

class ClassAllocTest : public testenv::fixture<Loki::NullType, ClassTest> {
public:
    virtual void SetUp() {
        Base::SetUp();

        gd_om_class_id_t classId = addClass("class1", 11, 256, 4);
        EXPECT_EQ(1, classId);

        m_cls = gd_om_class_get(&m_classMgr, classId);
        ASSERT_TRUE(m_cls);
    }

    struct gd_om_class * m_cls;
};

TEST_F(ClassAllocTest, alloc_from_empty) {
    EXPECT_EQ(-1, gd_om_class_alloc_object(m_cls));
}

TEST_F(ClassAllocTest, alloc_basic) {
    char page_buf[256];
    gd_om_data_page_head_init(page_buf);
    ASSERT_EQ(0, gd_om_class_add_page(m_cls, &page_buf, t_em()));

    EXPECT_EQ(0, gd_om_class_alloc_object(m_cls));
}

TEST_F(ClassAllocTest, alloc_all) {
    char page_buf[256];
    gd_om_data_page_head_init(page_buf);
    ASSERT_EQ(0, gd_om_class_add_page(m_cls, &page_buf, t_em()));

    for(size_t i = 0; i < m_cls->m_object_per_page; ++i) {
        EXPECT_EQ((int32_t)i, gd_om_class_alloc_object(m_cls));
    }

    EXPECT_EQ(-1, gd_om_class_alloc_object(m_cls));
}

TEST_F(ClassAllocTest, free_basic) {
    char page_buf[256];
    gd_om_data_page_head_init(page_buf);
    ASSERT_EQ(0, gd_om_class_add_page(m_cls, &page_buf, NULL));

    EXPECT_EQ(0, gd_om_class_alloc_object(m_cls));
    EXPECT_EQ(1, gd_om_class_alloc_object(m_cls));
    EXPECT_EQ(2, gd_om_class_alloc_object(m_cls));

    gd_om_class_free_object(m_cls, 1, t_em());

    EXPECT_TRUE(NULL == gd_om_class_get_object(m_cls, 1, NULL));
}

TEST_F(ClassAllocTest, free_not_alloc) {
    char page_buf[256];
    gd_om_data_page_head_init(page_buf);
    ASSERT_EQ(0, gd_om_class_add_page(m_cls, &page_buf, NULL));

    EXPECT_EQ(0, gd_om_class_alloc_object(m_cls));
    EXPECT_EQ(1, gd_om_class_alloc_object(m_cls));
    EXPECT_EQ(2, gd_om_class_alloc_object(m_cls));

    gd_om_class_free_object(m_cls, 1, t_em());
}

TEST_F(ClassAllocTest, free_re_alloc) {
    char page_buf[256];
    gd_om_data_page_head_init(page_buf);
    ASSERT_EQ(0, gd_om_class_add_page(m_cls, &page_buf, NULL));

    EXPECT_EQ(0, gd_om_class_alloc_object(m_cls));
    EXPECT_EQ(1, gd_om_class_alloc_object(m_cls));
    EXPECT_EQ(2, gd_om_class_alloc_object(m_cls));

    gd_om_class_free_object(m_cls, 1, t_em());

    EXPECT_EQ(1, gd_om_class_alloc_object(m_cls));
}
