#include "cpe/dr/dr_metalib_xml.h"
#include "gd/tl/tl_manage.h"
#include "EvtTest.hpp"

EvtTest::EvtTest()
    : m_tl_mgr(NULL)
    , m_evt_mgr(NULL)
{
}

void EvtTest::SetUp() {
    Base::SetUp();

    mem_buffer_init(&m_buffer, t_allocrator());

    m_tl_mgr = gd_tl_manage_create(t_allocrator());
}

void EvtTest::TearDown() {
    if (m_evt_mgr) {
        gd_evt_mgr_free(m_evt_mgr);
        m_evt_mgr = NULL;
    }
    mem_buffer_clear(&m_buffer);

    gd_tl_manage_free(m_tl_mgr);
    m_tl_mgr = NULL;

    Base::TearDown();
}

void EvtTest::createEvtMgr(const char * metalib) {
    if (m_evt_mgr) {
        gd_evt_mgr_free(m_evt_mgr);
        m_evt_mgr = NULL;
    }
    mem_buffer_clear(&m_buffer);

    EXPECT_EQ(
        0,
        dr_create_lib_from_xml_ex(
            &m_buffer,
            metalib,
            strlen(metalib),
            NULL));

    m_evt_mgr = gd_evt_mgr_create(
        m_tl_mgr,
        (LPDRMETALIB)mem_buffer_make_continuous(&m_buffer, 0),
        t_allocrator());
    EXPECT_TRUE(m_evt_mgr);
}

gd_evt_t EvtTest::createEvt(const char * typeName, size_t attach_capacity, ssize_t data_capacity) {
    EXPECT_TRUE(m_evt_mgr);
    return gd_evt_create(m_evt_mgr, attach_capacity, typeName, data_capacity, t_em());
}
