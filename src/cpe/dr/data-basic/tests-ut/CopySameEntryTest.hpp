#ifndef CPE_DR_DATABASIC_SETDEFAULTSTEST_H
#define CPE_DR_DATABASIC_SETDEFAULTSTEST_H
#include "DataBasicTest.hpp"

class CopySameEntryTest : public DataBasicTest {
public:
    void copy(const char * desMeta, const char * srcMeta, int policy = 0, size_t capacity = 0);
};

#endif
