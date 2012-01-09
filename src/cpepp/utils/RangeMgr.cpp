#include <stdexcept>
#include "cpepp/utils/RangeMgr.hpp"

namespace Cpe { namespace Utils {

RangeMgr::RangeMgr(mem_allocrator_t alloc) {
    if (cpe_range_mgr_init(&m_rm, alloc) != 0) {
        throw ::std::runtime_error("init RangeMgr fail!");
    }
}

RangeMgr::~RangeMgr() {
    cpe_range_mgr_fini(&m_rm);
}

}}

