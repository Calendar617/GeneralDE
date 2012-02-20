#include "MgrTest.hpp"

MgrTest::MgrTest() : m_omm(NULL) {
}

void MgrTest::SetUp() {
    Base::SetUp();

    m_omm = gd_om_mgr_create(t_allocrator(), 256, 1024);
    ASSERT_TRUE(m_omm);

    gd_om_mgr_set_backend_memory(m_omm, t_allocrator());
}

void MgrTest::TearDown() {
    if (m_omm) {
        gd_om_mgr_free(m_omm);
        m_omm = NULL;
    }

    Base::TearDown();
}

gd_om_buffer_id_t
MgrTest::buf_alloc(size_t size, void * context) {
    MgrTest * t = reinterpret_cast<MgrTest *>(context);
    return (gd_om_buffer_id_t)mem_alloc(t->t_allocrator(), size);
}

gd_om_class_id_t
MgrTest::addClass(const char * className, size_t object_size) {
    return gd_om_mgr_add_class(m_omm, className, object_size, 4, t_em());
}

gd_om_oid_t
MgrTest::obj_alloc(cpe_hash_string_t className) {
    return gd_om_obj_alloc(m_omm, className, t_em());
}



