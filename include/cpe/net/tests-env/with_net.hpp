#ifndef CPE_NET_TESTENV_WITHNET_H
#define CPE_NET_TESTENV_WITHNET_H
#include "cpe/utils/tests-env/test-env.hpp"
#include "../net.h"

namespace cpe { namespace net { namespace testenv {

class with_net : public ::testenv::env<> {
public:
    with_net();

    void SetUp();
    void TearDown();

    net_mgr_t t_net();

    void t_net_break();
    void t_net_tick();
    void t_net_run(void);
private:
    net_mgr_t m_net;
};

}}}

#endif

