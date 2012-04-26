#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "gtest/gtest.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../../dr_internal_types.h"
#include "with_InputMetaLibTest.hpp"

WithInputMetaLibTest::WithInputMetaLibTest()
    : m_lib(0)
    , m_libSize(0)
{
}

void WithInputMetaLibTest::loadLib() {
    ASSERT_TRUE(m_lib == 0) << "lib already loaded!";

    int inputFd = open(CPE_DR_METALIB_SIZE == 64 ? "input.bin.64" : "input.bin.32" , O_RDONLY);
    ASSERT_TRUE(inputFd != -1) << "open input file \"input.bin\" fail!";

    struct stat buffer;
    ASSERT_TRUE(fstat(inputFd, &buffer) == 0) << "fstat of input file error, errno=" << errno;

    m_libSize = buffer.st_size;

    m_lib = (LPDRMETALIB)malloc(buffer.st_size);
    ASSERT_TRUE(m_lib != 0) << "malloc for read meta lib error!";

    ssize_t readSize = read(inputFd, m_lib, buffer.st_size);
    ASSERT_EQ(buffer.st_size, readSize);

    close(inputFd);
}

void WithInputMetaLibTest::freeLib() {
    if (m_lib) {
        free(m_lib);
        m_lib = 0;
        m_libSize = 0;
    }
}

LPDRMETA
WithInputMetaLibTest::meta(const char * name) {
    LPDRMETA r = dr_lib_find_meta_by_name(m_lib, name);
    EXPECT_TRUE(r) << "get meta " << name << " fail!";
    return r;
}

LPDRMETAENTRY
WithInputMetaLibTest::entry(const char * metaName, const char * entryName) {
    LPDRMETA meta = dr_lib_find_meta_by_name(m_lib, metaName);
    EXPECT_TRUE(meta) << "get meta " << metaName << " fail!";
    if (meta == NULL) {
        return NULL;
    }

    LPDRMETAENTRY e = dr_meta_find_entry_by_path(meta, entryName);
    EXPECT_TRUE(e) << "get entry " << metaName << "." << entryName << " fail!";
    return e;
}

LPDRMETAENTRY
WithInputMetaLibTest::entry(const char * metaName, int pos) {
    LPDRMETA meta = dr_lib_find_meta_by_name(m_lib, metaName);
    EXPECT_TRUE(meta) << "get meta " << metaName << " fail!";
    if (meta == NULL) {
        return NULL;
    }

    LPDRMETAENTRY e = dr_meta_entry_at(meta, pos);
    EXPECT_TRUE(e) << "get entry at " << pos << " in " << metaName << " fail!";
    return e;
}

int32_t WithInputMetaLibTest::address_to_pos(void * p) {
    return (int32_t)((char *)p - (char *)(m_lib + 1));
}

int32_t WithInputMetaLibTest::address_to_start(void * p) {
    return (int32_t)((char *)p - (char *)m_lib);
}
