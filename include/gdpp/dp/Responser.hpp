#ifndef GDPP_DP_RESPONSER_H
#define GDPP_DP_RESPONSER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "gd/dp/dp_responser.h"
#include "System.hpp"

namespace Gd { namespace Dp {

class Responser : public Cpe::Utils::SimulateObject {
public:
    operator gd_dp_rsp_t () const { return (gd_dp_rsp_t)this; }

    void setProcessor(gd_dp_rsp_process_fun_t processor, void * ctx) {
        gd_dp_rsp_set_processor(*this, processor, ctx);
    }

    void setType(struct gd_dp_rsp_type * rspType) {
        gd_dp_rsp_set_type(*this, rspType);
    }
};

}}

#endif

