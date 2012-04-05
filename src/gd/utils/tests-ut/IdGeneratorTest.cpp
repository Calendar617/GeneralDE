#include <sstream>
#include "cpe/pal/pal_dlfcn.h"
#include "IdGeneratorTest.hpp"

void IdGeneratorTest::SetUp() {
    Base::SetUp();

    gd_set_default_library(dlopen(NULL, RTLD_NOW));

    t_app_install_module(
        "test_id_generator", "gd_id_generator", NULL, NULL);
}

gd_id_generator_t IdGeneratorTest::id_generator(void) {
    gd_id_generator_t r = gd_id_generator_find_nc(t_app(), "test_id_generator");
    EXPECT_TRUE(r);
    return r;
}

void IdGeneratorTest::write_in_file(const char * value) {
    t_write_to_file("ItemId.txt", value);
}

void IdGeneratorTest::set_load_from_file(void) {
    gd_id_generator_set_save_file(id_generator(), t_path_make("ItemId.txt"));
}
