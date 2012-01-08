#ifndef GDPP_DP_MANAGER_H
#define GDPP_DP_MANAGER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "gd/dp/dp_manage.h"
#include "System.hpp"
#include "ResponserIterator.hpp"

namespace Gd { namespace Dp {

class Manager : public Cpe::Utils::SimulateObject {
public:
    operator gd_dp_mgr_t () const { return (gd_dp_mgr_t)this; }

    void bind(gd_dp_rsp_t rsp, int32_t cmd);
    void bind(gd_dp_rsp_t rsp, const char * cmd);

    void unbind(gd_dp_rsp_t rsp, int32_t cmd);
    void unbind(gd_dp_rsp_t rsp, const char * cmd);

    Responser & createResponser(const char * name);
    void deleteResponser(const char * name);

    //rsp by name
    Responser * findRspByName(const char * name) { 
        return (Responser*)gd_dp_rsp_find_by_name(*this, name);
    }
    Responser const * findRspByName(const char * name) const { 
        return (Responser const *)gd_dp_rsp_find_by_name(*this, name);
    }
    Responser & rspByName(const char * name);
    Responser const & rspByName(const char * name) const;

    //rsp by string cmd
    Responser * findFirstRsp(const char * cmd) { 
        return (Responser*)gd_dp_rsp_find_first_by_string(*this, cmd);
    }
    Responser const * findFirstRsp(const char * cmd) const { 
        return (Responser*)gd_dp_rsp_find_first_by_string(*this, cmd);
    }
    Responser & firstRsp(const char * cmd);
    Responser const & firstRsp(const char * cmd) const;
    
    ResponserIterator rsps(const char * cmd) {
        ResponserIterator it;
        gd_dp_rsp_find_by_string(&it.m_it, *this, cmd);
        return it;
    }

    ConstResponserIterator rsps(const char * cmd) const {
        ConstResponserIterator it;
        gd_dp_rsp_find_by_string(&it.m_it, *this, cmd);
        return it;
    }

    //rsp by numeric cmd
    Responser * findFirstRsp(int32_t cmd) { 
        return (Responser*)gd_dp_rsp_find_first_by_numeric(*this, cmd);
    }
    Responser const * findFirstRsp(int32_t cmd) const { 
        return (Responser*)gd_dp_rsp_find_first_by_numeric(*this, cmd);
    }
    Responser & firstRsp(int32_t cmd);
    Responser const & firstRsp(int32_t cmd) const;

    ResponserIterator rsps(int32_t cmd) {
        ResponserIterator it;
        gd_dp_rsp_find_by_numeric(&it.m_it, *this, cmd);
        return it;
    }

    ConstResponserIterator rsps(int32_t cmd) const {
        ConstResponserIterator it;
        gd_dp_rsp_find_by_numeric(&it.m_it, *this, cmd);
        return it;
    }
};

}}

#endif

