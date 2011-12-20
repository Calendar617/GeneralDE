#include <dlfcn.h>
#include "AppTest.hpp"

extern "C" {
int test_module_a_global_init() { return 0; }
void test_module_a_global_fini() {}
}

TEST_F(AppTest, module_basic) {
    gd_set_default_library(dlopen(NULL, RTLD_NOW));
    // dlerror();
    // void * sym = dlsym(, "_ff");
    // printf("xxxx: sym=%p, error=%s\n", sym, dlerror());

    gd_app_module_t module = 
        t_app_install_module("test_module_a", NULL);
    ASSERT_TRUE(module != NULL);

    EXPECT_STREQ("test_module_a", gd_app_module_name(module));
}

TEST_F(AppTest, module_no_init) {
    EXPECT_TRUE(
        NULL == t_app_install_module("test_module_not_exist", NULL));
}

