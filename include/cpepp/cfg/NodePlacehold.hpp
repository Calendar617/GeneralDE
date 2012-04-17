#ifndef CPEPP_CFG_NODE_PLACEHOLD_H
#define CPEPP_CFG_NODE_PLACEHOLD_H
#include "cpe/cfg/cfg_read.h"
#include "cpepp/utils/CString.hpp"
#include "System.hpp"

namespace Cpe { namespace Cfg {

class NodePlacehold {
public:
    operator Node & (void) { return *(Node*)m_node; }
    Node & must_exist(void);

    NodePlacehold operator[](const char * path) { 
        return NodePlacehold(*this, path, cfg_find_cfg(m_node, path));
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

    int8_t asInt8(void) const { return *this; }
    uint8_t asUInt8(void) const { return *this; }
    int16_t asInt16(void) const { return *this; }
    uint16_t asUInt16(void) const { return *this; }
    int32_t asInt32(void) const { return *this; }
    uint32_t asUInt32(void) const { return *this; }
    int64_t asInt64(void) const { return *this; }
    uint64_t asUInt64(void) const { return *this; }
    float asFloat(void) const { return *this; }
    double asDouble(void) const { return *this; }
    Utils::CString const & asString(void) const { 
        return Utils::CString::_cast((const char *)(*this));
    }

    int8_t asInt8(int8_t v) const { return dft(v); }
    uint8_t asUInt8(uint8_t v) const { return dft(v); }
    int16_t asInt16(int16_t v) const { return dft(v); }
    uint16_t asUInt16(uint16_t v) const { return dft(v); }
    int32_t asInt32(int32_t v) const { return dft(v); }
    uint32_t asUInt32(uint32_t v) const { return dft(v); }
    int64_t asInt64(int64_t v) const { return dft(v); }
    uint64_t asUInt64(uint64_t v) const { return dft(v); }
    float asFloat(float v) const { return dft(v); }
    double asDouble(double v) const { return dft(v); }
    Utils::CString const & asString(const char * v) const { 
        return Utils::CString::_cast((const char *)(dft(v)));
    }

private:
    NodePlacehold(NodePlacehold & parent, const char * path, cfg_t node)
        : m_parent_p(&parent)
        , m_parent_n(NULL)
        , m_node(node)
        , m_path(path)
    {
    }

    NodePlacehold(Node & parent, const char * path, cfg_t node)
        : m_parent_p(NULL)
        , m_parent_n(&parent)
        , m_node(node)
        , m_path(path)
    {
    }

    NodePlacehold(NodePlacehold const & o)
        : m_parent_p(o.m_parent_p)
        , m_parent_n(o.m_parent_n)
        , m_node(o.m_node)
        , m_path(o.m_path)
    {
    }

    NodePlacehold & operator=(NodePlacehold const & o);

private:
    NodePlacehold * m_parent_p;
    Node * m_parent_n;
    cfg_t m_node;
    const char * m_path;

friend class Node;
};

}}

#endif
