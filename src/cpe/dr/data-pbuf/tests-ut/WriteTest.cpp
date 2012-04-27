#include "WriteTest.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_xml.h"
#include "cpe/dr/dr_json.h"
#include "cpe/utils/stream_buffer.h"

WriteTest::WriteTest() : m_metaLib(0) {
}

void WriteTest::SetUp() {
    Base::SetUp();
    mem_buffer_init(&m_metaLib_buffer, NULL);
    mem_buffer_init(&m_buffer, NULL);
}

void WriteTest::TearDown() {
    mem_buffer_clear(&m_buffer);

    m_metaLib = NULL;
    mem_buffer_clear(&m_metaLib_buffer);

    Base::TearDown();
}

void WriteTest::installMeta(const char * def) {
    m_metaLib = NULL;
    mem_buffer_clear(&m_metaLib_buffer);

    t_elist_clear();

    EXPECT_EQ(
        0,
        dr_create_lib_from_xml_ex(&m_metaLib_buffer, def, strlen(def), t_em()))
        << "install meta error";

    m_metaLib = (LPDRMETALIB)mem_buffer_make_exactly(&m_metaLib_buffer);
}

int WriteTest::write(const char * typeName, const char * defs) {
    LPDRMETA meta = dr_lib_find_meta_by_name(m_metaLib, typeName);
    EXPECT_TRUE(meta) << "get meta " << typeName << " error!";

    char buf[1024];

    int r = dr_json_read(buf, sizeof(buf), defs, meta, t_em());
    EXPECT_GT(r, 0);

    return write(typeName, buf, r);
}

int WriteTest::write(const char * typeName, const void * data, size_t data_size) {
    LPDRMETA meta = dr_lib_find_meta_by_name(m_metaLib, typeName);
    EXPECT_TRUE(meta) << "get meta " << typeName << " error!";

    t_elist_clear();

    struct write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(&m_buffer);

    int r = dr_pbuf_write((write_stream_t)&stream, data, data_size, meta, t_em());
    stream_putc((write_stream_t)&stream, 0);
    return r;
}

const char * WriteTest::result(void) {
    return (const char *)mem_buffer_make_exactly(&m_buffer);
}
