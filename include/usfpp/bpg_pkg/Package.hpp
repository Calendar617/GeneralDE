#ifndef USFPP_BPG_PKG_PACKAGE_H
#define USFPP_BPG_PKG_PACKAGE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "System.hpp"

namespace Usf { namespace Bpg {

class Package : public Cpe::Utils::SimulateObject {
public:
    operator bpg_pkg_t() const { return (bpg_pkg_t)this; }

    PackageManager & mgr(void) { return *(PackageManager*)bpg_pkg_mgr(*this); }
    PackageManager const & mgr(void) const { return *(PackageManager*)bpg_pkg_mgr(*this); }

    void init(void) { bpg_pkg_init(*this); }

    uint32_t cmd(void) const { return bpg_pkg_cmd(*this); }
    void setCmd(uint32_t cmd) { bpg_pkg_set_cmd(*this, cmd); }

    uint32_t errCode(void) const { return bpg_pkg_errno(*this); }
    void setErrCode(uint32_t en) { bpg_pkg_set_errno(*this, en); }

    uint32_t clientId(void) const { return bpg_pkg_client_id(*this); }
    void setClientId(uint32_t client_id) { bpg_pkg_set_client_id(*this, client_id); }

    /*main data write*/
    void setMainData(void const * data, size_t size);

    template<typename T>
    void setMainData(int cmd, T const & data) { setMainData(cmd, &data, sizeof(data)); }

    /*main data read*/
    void mainData(void * buf, size_t capacity) const;
    template<typename T>
    void mainData(T & buf) { mainData(&buf, sizeof(buf)); }

    /*append data write*/
    void addAppendData(const char * metaName, void const * data, size_t size);
    void addAppendData(int metaid, void const * data, size_t size);
    void addAppendData(LPDRMETA meta, void const * data, size_t size);

    template<typename T>
    void addAppendData(const char * metaName, T const & data) {
        addAppendData(metaName, &data, sizeof(data));
    }

    template<typename T>
    void addAppendData(int metaId, T const & data) {
        addAppendData(metaId, &data, sizeof(data));
    }

    /*append data read*/
    void appendData(int metaId, void * buf, size_t capacity) const;
    void appendData(const char * metaName, void * buf, size_t capacity) const;

    bool tryGetAppendData(int metaId, void * buf, size_t capacity) const;
    bool tryGetAppendData(const char * metaName, void * buf, size_t capacity) const;

    template<typename T>
    void appendData(int metaId, T & buf) { appendData(metaId, &buf, sizeof(buf)); }

    template<typename T>
    void appendData(const char * metaName, T & buf) { appendData(metaName, &buf, sizeof(buf)); }

    template<typename T>
    bool tryGetAppendData(int metaId, T & buf) { return tryGetAppendData(metaId, &buf, sizeof(buf)); }

    template<typename T>
    bool tryGetAppendData(const char * metaName, T & buf) { return tryGetAppendData(metaName, &buf, sizeof(buf)); }

    /*other op*/
    const char * dump_data(mem_buffer_t buffer) const;

    static Package & _cast(bpg_pkg_t pkg);
    static Package & _cast(gd_dp_req_t req);
};

}}

#endif
