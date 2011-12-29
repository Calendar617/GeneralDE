#include "MgrTest.hpp" 

TEST_F(MgrTest, object_alloc_basic) {
    CPE_HS_DEF_VAR(className, "class1");
    addClass("class1", 20);

    gd_om_oid_t oid = obj_alloc(className);
    EXPECT_TRUE(oid != GD_OM_INVALID_OID);
}
