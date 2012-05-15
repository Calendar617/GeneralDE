#ifndef _3RDTOOLS_MMSGE_TEST_MMSEG_H
#define _3RDTOOLS_MMSGE_TEST_MMSEG_H
#include "gtest/gtest.h"
#include "../css/SegmenterManager.h"

class MMSegTest : public ::testing::Test {
public:
    MMSegTest();
    virtual void SetUp();

    css::SegmenterManager m_segmentManage;
};

#endif
