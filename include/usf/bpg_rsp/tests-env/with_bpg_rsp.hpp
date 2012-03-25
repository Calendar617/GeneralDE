#ifndef USF_BPG_RSP_TESTENV_WITH_BPG_H
#define USF_BPG_RSP_TESTENV_WITH_BPG_H
#include "cpe/utils/tests-env/test-env.hpp"
#include "../bpg_rsp_manage.h"
#include "../bpg_rsp.h"

namespace usf { namespace bpg_rsp { namespace testenv {

class with_bpg_rsp : public ::testenv::env<> {
public:
    with_bpg_rsp();

    void SetUp();
    void TearDown();

    bpg_rsp_manage_t t_bpg_rsp_manage(const char * name = NULL, const char * logic_name = NULL);
};

}}}

#endif
