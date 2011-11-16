#ifndef CPE_DR_TEST_WITH_INPUTMETALIBTEST_H
#define CPE_DR_TEST_WITH_INPUTMETALIBTEST_H
#include "../../dr_internal_types.h"
#include "cpe/dr/dr_types.h"

class WithInputMetaLibTest {
public:
    WithInputMetaLibTest();

    void loadLib();
    void freeLib();

    LPDRMETALIB m_lib;
    int m_libSize;

    LPDRMETA meta(const char * name);
    LPDRMETAENTRY entry(const char * metaName, const char * entryname);

    int32_t address_to_start(void * p);
    int32_t address_to_pos(void * p);
};

#endif
