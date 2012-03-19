#ifndef USF_BPG_TESTENV_WITH_BPG_H
#define USF_BPG_TESTENV_WITH_BPG_H
#include "cpe/utils/tests-env/test-env.hpp"
#include "../bpg_manage.h"
#include "../bpg_rsp.h"

namespace usf { namespace bpg { namespace testenv {

class with_bpg : public ::testenv::env<> {
public:
    with_bpg();

    void SetUp();
    void TearDown();

    bpg_manage_t t_bpg_manage(const char * name = NULL, const char * logic_name = NULL);
};

}}}

#endif
