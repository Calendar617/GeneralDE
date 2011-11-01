#ifndef CPE_DR_TEST_WITH_INPUTMETALIBTEST_H
#define CPE_DR_TEST_WITH_INPUTMETALIBTEST_H
#include "cpe/dr/dr_types.h"

class WithInputMetaLibTest {
public:
    WithInputMetaLibTest();

    void loadLib();
    void freeLib();

    LPDRMETALIB m_lib;
    int m_libSize;
};

#endif
