#include <sstream>
#include <stdexcept>
#include "cpepp/utils/ErrorCollector.hpp"
#include "gdpp/app/Log.hpp"
#include "gdpp/dp/Manager.hpp"
#include "gdpp/dp/Responser.hpp"

namespace Gd { namespace Dp {

void Manager::bind(gd_dp_rsp_t rsp, int32_t cmd) {
    Cpe::Utils::ErrorCollector ec;
    if (gd_dp_rsp_bind_numeric(rsp, cmd, ec) != 0) {
        ec.checkThrowWithMsg< ::std::runtime_error>();
    }
}

void Manager::bind(gd_dp_rsp_t rsp, const char * cmd) {
    Cpe::Utils::ErrorCollector ec;
    if (gd_dp_rsp_bind_string(rsp, cmd, ec) != 0) {
        ec.checkThrowWithMsg< ::std::runtime_error>();
    }
}

void Manager::unbind(gd_dp_rsp_t rsp, int32_t cmd) {
    gd_dp_rsp_unbind_numeric(rsp, cmd);
}

void Manager::unbind(gd_dp_rsp_t rsp, const char * cmd) {
    gd_dp_rsp_unbind_string(rsp, cmd);
}

Responser & Manager::createResponser(const char * name) {
    gd_dp_rsp_t rsp = gd_dp_rsp_create(*this, name);
    if (rsp == NULL) {
        ::std::ostringstream os;
        os << "create rsp " << name << " fail!";
        throw ::std::runtime_error(os.str());
    }

    return *(Responser*)rsp;
}

void Manager::deleteResponser(const char * name) {
    gd_dp_rsp_t rsp = gd_dp_rsp_find_by_name(*this, name);
    if (rsp) gd_dp_rsp_free(rsp);
}

Responser & Manager::rspByName(const char * name) {
    Responser * r = findRspByName(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "rsp " << name << " not exist!";
        throw ::std::runtime_error(os.str());
    }

    return *r;
}

Responser const & Manager::rspByName(const char * name) const {
    Responser const * r = findRspByName(name);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "rsp " << name << " not exist!";
        throw ::std::runtime_error(os.str());
    }

    return *r;
}

Responser & Manager::firstRsp(const char * cmd) {
    Responser * r = findFirstRsp(cmd);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "rsp to cmd " << cmd << " not exist!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Responser const & Manager::firstRsp(const char * cmd) const {
    Responser const * r = findFirstRsp(cmd);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "rsp to cmd " << cmd << " not exist!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Responser &
Manager::firstRsp(int32_t cmd) {
    Responser * r = findFirstRsp(cmd);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "rsp to cmd " << cmd << " not exist!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

Responser const &
Manager::firstRsp(int32_t cmd) const {
    Responser const * r = findFirstRsp(cmd);
    if (r == NULL) {
        ::std::ostringstream os;
        os << "rsp to cmd " << cmd << " not exist!";
        throw ::std::runtime_error(os.str());
    }
    return *r;
}

}}
