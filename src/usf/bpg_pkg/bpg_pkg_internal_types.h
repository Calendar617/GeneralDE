#ifndef USF_BPG_REQ_INTERNAL_TYPES_H
#define USF_BPG_REQ_INTERNAL_TYPES_H
#include "cpe/utils/hash.h"
#include "usf/dr_store/dr_store_types.h"
#include "usf/bpg_pkg/bpg_pkg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bpg_pkg_manage {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    dr_ref_t m_metalib_basepkg_ref;
    error_monitor_t m_em;

    dr_cvt_t m_base_cvt;
    dr_cvt_t m_data_cvt;

    int m_debug;
};

struct bpg_pkg {
    bpg_pkg_manage_t m_mgr;

    LPDRMETA m_carry_data_meta;
    size_t m_carry_data_size;
    size_t m_carry_data_capacity;

    gd_dp_req_t m_dp_req;
};

#ifdef __cplusplus
}
#endif

#endif
