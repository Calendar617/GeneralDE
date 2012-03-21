#ifndef USFPP_BPG_PKG_PACKAGE_H
#define USFPP_BPG_PKG_PACKAGE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/dr/System.hpp"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "System.hpp"

namespace Usf { namespace Bpg {

class Package : public Cpe::Utils::SimulateObject {
public:
    operator bpg_pkg_t() const { return (bpg_pkg_t)this; }

    PackageManager & mgr(void) { return *(PackageManager*)bpg_pkg_mgr(*this); }
    PackageManager const & mgr(void) const { return *(PackageManager*)bpg_pkg_mgr(*this); }

    Gd::App::Application & app(void);
    Gd::App::Application const & app(void) const;

    void init(void) { bpg_pkg_init(*this); }

    uint32_t cmd(void) const { return bpg_pkg_cmd(*this); }
    void setCmd(uint32_t cmd) { bpg_pkg_set_cmd(*this, cmd); }

    uint32_t errCode(void) const { return bpg_pkg_errno(*this); }
    void setErrCode(uint32_t en) { bpg_pkg_set_errno(*this, en); }

    uint32_t clientId(void) const { return bpg_pkg_client_id(*this); }
    void setClientId(uint32_t client_id) { bpg_pkg_set_client_id(*this, client_id); }

    Cpe::Dr::MetaLib const & dataMetaLib(void) const;

    /*main data read*/
    Cpe::Dr::Meta const & mainDataMeta(void) const;
    Cpe::Dr::Meta const * tryGetMainDataMeta(void) const { return (Cpe::Dr::Meta const *)bpg_pkg_main_data_meta(*this, NULL); }

    void mainData(void * buf, size_t capacity, size_t * size = NULL) const;
    template<typename T>
    void mainData(T & buf) { mainData(&buf, sizeof(buf)); }

    /*main data write*/
    void setMainData(void const * data, size_t size);

    template<typename T>
    void setMainData(int cmd, T const & data) { setMainData(cmd, &data, sizeof(data)); }

    /*append data read*/
    void appendData(int metaId, void * buf, size_t capacity, size_t * size = NULL) const;
    void appendData(const char * metaName, void * buf, size_t capacity, size_t * size = NULL) const;
    void appendData(LPDRMETA meta, void * buf, size_t capacity, size_t * size = NULL) const;

    bool tryGetAppendData(int metaId, void * buf, size_t capacity, size_t * size = NULL) const;
    bool tryGetAppendData(const char * metaName, void * buf, size_t capacity, size_t * size = NULL) const;
    bool tryGetAppendData(LPDRMETA meta, void * buf, size_t capacity, size_t * size = NULL) const;

    template<typename T>
    void appendData(int metaId, T & buf) { appendData(metaId, &buf, sizeof(buf)); }

    template<typename T>
    void appendData(const char * metaName, T & buf) { appendData(metaName, &buf, sizeof(buf)); }

    template<typename T>
    bool tryGetAppendData(int metaId, T & buf) { return tryGetAppendData(metaId, &buf, sizeof(buf)); }

    template<typename T>
    bool tryGetAppendData(const char * metaName, T & buf) { return tryGetAppendData(metaName, &buf, sizeof(buf)); }

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

    /*other op*/
    const char * dump_data(mem_buffer_t buffer) const { return bpg_pkg_dump(*this, buffer); }

    static Package & _cast(bpg_pkg_t pkg);
    static Package & _cast(gd_dp_req_t req);
};

}}

#endif
