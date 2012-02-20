#include "../net_internal_types.h"
#include "NetTest.hpp"

TEST_F(NetTest, basic) {
    t_net_tick();
}

TEST_F(NetTest, ep_create_basic) {
    gd_net_ep_t ep = gd_net_ep_create(t_net(), gd_net_ep_socket);
    ASSERT_TRUE(ep);

    EXPECT_EQ(0, (int)ep->m_id);
    EXPECT_EQ(-1, ep->m_fd);
    EXPECT_TRUE(ep->m_chanel_r == NULL);
    EXPECT_TRUE(ep->m_chanel_r == NULL);
}
