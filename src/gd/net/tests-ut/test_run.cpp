#include "NetTest.hpp"

void net_test_example_accept_fun(gd_net_listener_t listener, gd_net_ep_t ep) {
}

TEST_F(NetTest, run) {
    t_em_set_print();

    gd_net_listener_t listener =
        gd_net_listener_create(
            t_net(),
            "test-listener",
            "127.0.0.1",
            0,
            5,
            net_test_example_accept_fun);

    ASSERT_TRUE(listener);

    t_net_tick();
}
