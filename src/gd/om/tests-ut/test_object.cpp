#include "MgrTest.hpp" 

TEST_F(MgrTest, object_alloc_basic) {
    CPE_HS_DEF_VAR(className, "class1");
    addClass("class1", 20);

    gd_om_oid_t oid = obj_alloc(className);
    EXPECT_TRUE(oid != GD_OM_INVALID_OID);

    void * objData = gd_om_obj_get(m_omm, oid, t_em());
    ASSERT_TRUE(objData);

    gd_om_class_t cls = gd_om_obj_class(m_omm, oid, t_em());
    ASSERT_TRUE(cls);

    EXPECT_STREQ("class1", gd_om_class_name(cls));
}

TEST_F(MgrTest, object_alloc_no_class) {
    CPE_HS_DEF_VAR(className, "not-exist-class");

    EXPECT_EQ(GD_OM_INVALID_OID, obj_alloc(className));

    EXPECT_TRUE(t_em_have_errno(gd_om_class_not_exist));
}

TEST_F(MgrTest, object_size) {
    CPE_HS_DEF_VAR(className, "class1");
    addClass("class1", 20);

    gd_om_oid_t oid1 = obj_alloc(className);
    EXPECT_TRUE(oid1 != GD_OM_INVALID_OID);
    void * objData1 = gd_om_obj_get(m_omm, oid1, t_em());
    ASSERT_TRUE(objData1);

    gd_om_oid_t oid2 = obj_alloc(className);
    EXPECT_TRUE(oid2 != GD_OM_INVALID_OID);

    void * objData2 = gd_om_obj_get(m_omm, oid2, t_em());
    ASSERT_TRUE(objData2);

    EXPECT_EQ(1, (int)(oid2 - oid1));
    EXPECT_EQ(20, (int)((char*)objData2 - (char*)objData1));
}

TEST_F(MgrTest, object_free) {
    CPE_HS_DEF_VAR(className, "class1");
    addClass("class1", 20);

    gd_om_oid_t oid = obj_alloc(className);
    EXPECT_TRUE(oid != GD_OM_INVALID_OID);

    void * objData = gd_om_obj_get(m_omm, oid, t_em());
    ASSERT_TRUE(objData);

    gd_om_obj_free(m_omm, oid, t_em());

    EXPECT_TRUE(NULL == gd_om_obj_get(m_omm, oid, t_em()));

    EXPECT_EQ(oid, obj_alloc(className));
}

TEST_F(MgrTest, object_from_addr_basic) {
    CPE_HS_DEF_VAR(className, "class1");
    addClass("class1", 20);

    gd_om_oid_t oid = obj_alloc(className);
    EXPECT_TRUE(oid != GD_OM_INVALID_OID);

    void * objData = gd_om_obj_get(m_omm, oid, t_em());
    ASSERT_TRUE(objData);

    EXPECT_EQ(oid, gd_om_obj_id_from_addr(m_omm, objData, t_em()));
}
