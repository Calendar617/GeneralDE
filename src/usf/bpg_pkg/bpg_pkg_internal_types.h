#ifndef USF_BPG_REQ_INTERNAL_TYPES_H
#define USF_BPG_REQ_INTERNAL_TYPES_H
#include "cpe/utils/hash.h"
#include "gd/dr_store/dr_store_types.h"
#include "usf/bpg_pkg/bpg_pkg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bpg_pkg_manage {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    dr_ref_t m_metalib_basepkg_ref;
    dr_ref_t m_metalib_ref;
    error_monitor_t m_em;

    char m_cmd_meta_name[64];
    LPDRMETA m_cmd_meta;

    dr_cvt_t m_base_cvt;
    dr_cvt_t m_data_cvt;

    struct cpe_hash_table m_pkg_debug_infos;
    bpg_pkg_debug_level_t m_pkg_debug_default_level;

    int m_debug;
};

struct bpg_pkg_debug_info {
    uint32_t m_cmd;
    bpg_pkg_debug_level_t m_debug_level;
    struct cpe_hash_entry m_hh;
};

struct bpg_pkg {
    bpg_pkg_manage_t m_mgr;

    uint32_t m_connection_id;

    LPDRMETA m_carry_data_meta;
    size_t m_carry_data_size;
    size_t m_carry_data_capacity;

    dp_req_t m_dp_req;
};

enum bpg_pkg_dsp_type {
    bpg_pkg_dsp_to_cmd
    , bpg_pkg_dsp_to_str
};

union bpg_pkg_dsp_target {
    int32_t m_to_cmd;
    cpe_hash_string_t m_to_str;
};

struct bpg_pkg_dsp_node {
    enum bpg_pkg_dsp_type m_type;
    int32_t m_cmd;
    union bpg_pkg_dsp_target m_target;
    struct cpe_hash_entry m_hh;
};

struct bpg_pkg_dsp {
    mem_allocrator_t m_alloc;
    struct cpe_hash_table m_cmd_dsp;
    struct bpg_pkg_dsp_node * m_dft_dsp;
};

#ifdef __cplusplus
}
#endif

#endif
