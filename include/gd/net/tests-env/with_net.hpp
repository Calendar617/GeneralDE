#ifndef GD_NET_TESTENV_WITHNET_H
#define GD_NET_TESTENV_WITHNET_H
#include "cpe/utils/tests-env/test-env.hpp"
#include "../net.h"

namespace gd { namespace net { namespace testenv {

class with_net : public ::testenv::env<> {
public:
    with_net();

    void SetUp();
    void TearDown();

    gd_net_mgr_t t_net();

    void t_net_tick();
    void t_net_run(void);
private:
    gd_net_mgr_t m_net;
};

}}}

#endif

