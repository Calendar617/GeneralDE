#include "AppTest.hpp"

TEST_F(AppTest, module_create_basic) {
    gd_app_module_t module = installTestModule();
    ASSERT_TRUE(module != NULL);

    EXPECT_STREQ("test_module", gd_app_module_name(module));
    EXPECT_TRUE(NULL == gd_app_module_lib(module));
}

TEST_F(AppTest, module_create_no_init) {
    EXPECT_TRUE(
        NULL == t_app_install_module("test_module_not_exist", NULL));
}

TEST_F(AppTest, module_find_basic) {
    gd_app_module_t module = installTestModule();

    EXPECT_TRUE(module == gd_app_find_module(t_app(), "test_module"));
}

TEST_F(AppTest, module_find_not_exist) {
    EXPECT_TRUE(NULL == gd_app_find_module(t_app(), "test_module"));
}
