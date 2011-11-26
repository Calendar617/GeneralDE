#include "TlTest.hpp"

class DispatchTest : public TlTest {
public:
    void SetUp() {
        TlTest::SetUp();
        installTl();
    }
};

TEST_F(DispatchTest, empty) {
    EXPECT_EQ(
        0,
        gd_tl_manage_tick(m_manage, 10));
}
