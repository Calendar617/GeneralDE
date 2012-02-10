#ifndef CPE_CFT_TESTENV_WITH_CFG_H
#define CPE_CFT_TESTENV_WITH_CFG_H
#include "cpe/utils/tests-env/test-env.hpp"
#include "../cfg.h"

namespace cpe { namespace cfg { namespace testenv {

class with_cfg : public ::testenv::env<> {
public:
    cfg_t t_cfg_parse(const char * input);
    const char * t_cfg_dump(cfg_t cfg, int ident = 0, int level_ident = 4);
    const char * t_cfg_dump_inline(cfg_t cfg);
};

}}}

#endif
