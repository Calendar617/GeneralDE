#include "NetTest.hpp"

class RunTest : public NetTest {
};

void run_test_example_accept_fun(gd_net_listener_t listener, gd_net_ep_t ep, void * ctx) {
}

void run_test_connector_state_monitor(gd_net_connector_t connector, void * ctx) {
    RunTest * runTest = (RunTest *)ctx;

    gd_net_connector_state_t state = gd_net_connector_state(connector);

    if (state == gd_net_connector_state_error
        || state == gd_net_connector_state_connected)
    {
        runTest->t_net_break();
    }
}

TEST_F(RunTest, run) {
    gd_net_listener_t listener =
        gd_net_listener_create(
            t_net(),
            "test-listener",
            "",
            0,
            5,
            run_test_example_accept_fun,
            this);
    ASSERT_TRUE(listener);

    gd_net_connector_t connector =
        gd_net_connector_create_with_ep(
            t_net(),
            "test-connector",
            "127.0.0.1",
            gd_net_listener_using_port(listener));
    ASSERT_TRUE(connector);

    gd_net_connector_set_monitor(
        connector, run_test_connector_state_monitor, this);

    gd_net_connector_enable(connector);

    t_net_run();
}
