#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_metalib_xml.h"
#include "gd/app/tests-env/with_app.hpp"
#include "gd/dr_store/dr_store.h"
#include "gd/dr_store/dr_store_manage.h"
#include "gd/dr_store/tests-env/with_dr_store.hpp"
#include "cpe/utils/tests-env/with_em.hpp"

namespace gd { namespace dr_store { namespace testenv {

with_dr_store::with_dr_store()
    : m_dr_store_mgr(NULL)
{
}

void with_dr_store::SetUp() {
    m_dr_store_mgr = dr_store_manage_create(
        envOf<gd::app::testenv::with_app>().t_app(),
        NULL,
        t_allocrator(),
        NULL);
}

void with_dr_store::TearDown() {
    dr_store_manage_free(m_dr_store_mgr);
    m_dr_store_mgr = NULL;
}

void with_dr_store::t_dr_store_install(const char * name, const char * def) {
    struct mem_buffer buffer;
    mem_buffer_init(&buffer, t_tmp_allocrator());

    EXPECT_EQ(
        0,
        dr_create_lib_from_xml_ex(
            &buffer,
            def,
            strlen(def),
            NULL));

    dr_store_t dr_store = dr_store_create(m_dr_store_mgr, name);
    ASSERT_TRUE(dr_store) << "crate dr_store " << name << " fail!";

    EXPECT_EQ(
        0, 
        dr_store_set_lib(dr_store, (LPDRMETALIB)mem_buffer_make_continuous(&buffer, 0), NULL, NULL));
}

}}}
