#include "CompaireTest.hpp"

int CTypeCompaireTest::comp(int lType, const char * lValue, int rType, const char * rValue) {
    char lBuf[100];
    char rBuf[100];
    if (lType != CPE_DR_TYPE_STRING) {
        EXPECT_EQ(0, dr_ctype_set_from_string(lBuf, lType, lValue, 0));
    }
    else {
        strncpy(lBuf, lValue, sizeof(lBuf));
    }

    if (rType != CPE_DR_TYPE_STRING)  {
        EXPECT_EQ(0, dr_ctype_set_from_string(rBuf, rType, rValue, 0));
    }
    else {
        strncpy(rBuf, rValue, sizeof(rBuf));
    }

    return dr_ctype_cmp(lBuf, lType, rBuf, rType);
}

