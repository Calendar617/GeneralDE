#include <stdexcept>
#include "cpepp/cfg/Node.hpp"

namespace Cpe { namespace Cfg {

#define CPE_CFG_GEN_READ_TYPE(__type)                               \
Node::operator __type ## _t(void) {                                 \
    __type ## _t rv;                                                \
    if (cfg_try_as_ ## __type (*this, &rv) != 0) {                  \
        if (this == NULL) {                                         \
            throw ::std::runtime_error("Cfg Node is not valid!");   \
        }                                                           \
        else {                                                      \
            throw ::std::runtime_error("Cfg Node read int8 fail!"); \
        }                                                           \
    }                                                               \
                                                                    \
    return rv;                                                      \
}                                                                   \
__type ## _t Node::dft(__type ## _t v) {                            \
    return cfg_as_ ## __type(*this, v);                             \
}

CPE_CFG_GEN_READ_TYPE(int8);
CPE_CFG_GEN_READ_TYPE(uint8);
CPE_CFG_GEN_READ_TYPE(int16);
CPE_CFG_GEN_READ_TYPE(uint16);
CPE_CFG_GEN_READ_TYPE(int32);
CPE_CFG_GEN_READ_TYPE(uint32);
CPE_CFG_GEN_READ_TYPE(int64);
CPE_CFG_GEN_READ_TYPE(uint64);

}}
