#ifndef GDPP_DP_RESPONSER_H
#define GDPP_DP_RESPONSER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/utils/CString.hpp"
#include "cpe/cfg/cfg_types.h"
#include "gd/dp/dp_responser.h"
#include "System.hpp"

namespace Gd { namespace Dp {

class ResponserAdp : public Cpe::Utils::Noncopyable {
public:
    ResponserAdp(gd_dp_mgr_t mgr, const char * name);
    virtual ~ResponserAdp();

    Responser const & responser(void) const { return *(Responser*)m_dp_rsp; }
    void bind(const char * targt);
    void bind(int32_t target);
    void bind(cfg_t cfg);

    virtual void processRequest(Gd::Dp::Request & req, error_monitor_t em) = 0;

private:
    gd_dp_rsp_t m_dp_rsp;

    static int gd_dp_rsp_adp_process(gd_dp_req_t req, void * ctx, error_monitor_t em);
};

}}

#endif

