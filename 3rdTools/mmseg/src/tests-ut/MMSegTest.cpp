#include "MMSegTest.hpp"

MMSegTest::MMSegTest() {
}

void MMSegTest::SetUp() {
    ::testing::Test::SetUp();
    EXPECT_EQ(0, m_segmentManage.init("data"));
}
