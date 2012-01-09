#ifndef CPEPP_UTILS_RANGEMGR_H
#define CPEPP_UTILS_RANGEMGR_H
#include "cpe/utils/range.h"
#include "ClassCategory.hpp"

namespace Cpe { namespace Utils {

class RangeMgr : public Noncopyable {
public:
    RangeMgr(mem_allocrator_t alloc);
    ~RangeMgr();

    void clear(void) { cpe_range_mgr_clear(&m_rm); }

    int getOne(void);
    void putOne(ptr_int_t value); 

private:
    struct cpe_range_mgr m_rm;
};

}}

#endif
