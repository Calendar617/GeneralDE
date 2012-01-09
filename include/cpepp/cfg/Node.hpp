#ifndef CPEPP_CFG_NODE_H
#define CPEPP_CFG_NODE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpe/cfg/cfg.h"
#include "System.hpp"

namespace Cpe { namespace Cfg {

class Node : public Cpe::Utils::SimulateObject {
public:
    operator cfg_t (void) const { return (cfg_t)this; }
    bool isValid(void) const { return this != NULL; }

    Node & operator[](const char * path) { return *((Node*)cfg_find_cfg(*this, path)); }

    operator int8_t(void);
    operator uint8_t(void);
    operator int16_t(void);
    operator uint16_t(void);
    operator int32_t(void);
    operator uint32_t(void);
    operator int64_t(void);
    operator uint64_t(void);

    int8_t dft(int8_t dft);
    uint8_t dft(uint8_t dft);
    int16_t dft(int16_t dft);
    uint16_t dft(uint16_t dft);
    int32_t dft(int32_t dft);
    uint32_t dft(uint32_t dft);
    int64_t dft(int64_t dft);
    uint64_t dft(uint64_t dft);
};

}}

#endif
