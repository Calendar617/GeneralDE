#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "gtest/gtest.h"
#include "with_InputMetaLibTest.hpp"

WithInputMetaLibTest::WithInputMetaLibTest()
    : m_lib(0)
    , m_libSize(0)
{
}

void WithInputMetaLibTest::loadLib() {
    ASSERT_TRUE(m_lib == 0) << "lib already loaded!";

    int inputFd = open("input.bin", O_RDONLY);
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

