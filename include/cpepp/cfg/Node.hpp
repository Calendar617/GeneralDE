#ifndef CPEPP_CFG_NODE_H
#define CPEPP_CFG_NODE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpe/cfg/cfg.h"
#include "System.hpp"
#include "NodeIterator.hpp"

namespace Cpe { namespace Cfg {

class Node : public Cpe::Utils::SimulateObject {
public:
    operator cfg_t (void) const { return (cfg_t)this; }
    bool isValid(void) const { return this != NULL; }

    Node & operator[](const char * path) { return *((Node*)cfg_find_cfg(*this, path)); }
    Node const & operator[](const char * path) const { return *((Node*)cfg_find_cfg(*this, path)); }

    size_t childCount(void) const { return (size_t)cfg_child_count(*this); }

    NodeConstIterator childs(void) const {
        NodeConstIterator r;
        cfg_it_init(&r.m_it, *this);
        return r;
    }

    NodeIterator childs(void) {
        NodeIterator r;
        cfg_it_init(&r.m_it, *this);
        return r;
    }

    operator int8_t(void) const;
    operator uint8_t(void) const;
    operator int16_t(void) const;
    operator uint16_t(void) const;
    operator int32_t(void) const;
    operator uint32_t(void) const;
    operator int64_t(void) const;
    operator uint64_t(void) const;
    operator float(void) const;
    operator double(void) const;
    operator const char * (void) const;

    int8_t dft(int8_t dft) const;
    uint8_t dft(uint8_t dft) const;
    int16_t dft(int16_t dft) const;
    uint16_t dft(uint16_t dft) const;
    int32_t dft(int32_t dft) const;
    uint32_t dft(uint32_t dft) const;
    int64_t dft(int64_t dft) const;
    uint64_t dft(uint64_t dft) const;
    float dft(float dft) const;
    double dft(double dft) const;
    const char * dft(const char * dft) const;
};

}}

#endif
