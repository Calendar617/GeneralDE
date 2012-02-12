#include "CompaireTest.hpp"

int CompaireTest::compaire(const char * l, const char * r) {
    return cfg_cmp(t_cfg_parse(l), t_cfg_parse(r), t_em());
}

