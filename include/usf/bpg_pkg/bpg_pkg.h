#ifndef USF_BPG_PKG_H
#define USF_BPG_PKG_H
#include "cpe/utils/hash_string.h"
#include "cpe/dr/dr_types.h"
#include "bpg_pkg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern cpe_hash_string_t bpg_pkg_type_name;

bpg_pkg_t
bpg_pkg_create(
    bpg_pkg_manage_t mgr,
    size_t capacity,
    LPDRMETA carry_data_meta,
    size_t carry_data_capacity);

void bpg_pkg_free(bpg_pkg_t pkg);

bpg_pkg_manage_t bpg_pkg_mgr(bpg_pkg_t pkg);

uint32_t bpg_pkg_connection_id(bpg_pkg_t pkg);
void bpg_pkg_set_connection_id(bpg_pkg_t pkg, uint32_t connection_id);

LPDRMETA bpg_pkg_carry_data_meta(bpg_pkg_t pkg);
size_t bpg_pkg_carry_data_capacity(bpg_pkg_t pkg);

void * bpg_pkg_carry_data(bpg_pkg_t pkg);
size_t bpg_pkg_carry_data_size(bpg_pkg_t pkg);
int bpg_pkg_carry_data_set_size(bpg_pkg_t pkg, size_t size);

size_t bpg_pkg_pkg_capacity(bpg_pkg_t pkg);

void * bpg_pkg_pkg_data(bpg_pkg_t pkg);
size_t bpg_pkg_pkg_data_size(bpg_pkg_t pkg);
int bpg_pkg_pkg_data_set_size(bpg_pkg_t pkg, size_t size);


dr_cvt_t bpg_pkg_data_cvt(bpg_pkg_t pkg);
dr_cvt_t bpg_pkg_base_cvt(bpg_pkg_t pkg);
LPDRMETA bpg_pkg_base_meta(bpg_pkg_t pkg);

LPDRMETA bpg_pkg_cmd_meta(bpg_pkg_t pkg);

dp_req_t bpg_pkg_to_dp_req(bpg_pkg_t pkg);
bpg_pkg_t bpg_pkg_from_dp_req(dp_req_t pkg);


void bpg_pkg_init(bpg_pkg_t pkg);
void bpg_pkg_clear_data(bpg_pkg_t pkg);

/*pkg operations*/
uint32_t bpg_pkg_cmd(bpg_pkg_t pkg);
void bpg_pkg_set_cmd(bpg_pkg_t pkg, uint32_t cmd);

uint32_t bpg_pkg_errno(bpg_pkg_t pkg);
void bpg_pkg_set_errno(bpg_pkg_t pkg, uint32_t en);

uint64_t bpg_pkg_client_id(bpg_pkg_t pkg);
void bpg_pkg_set_client_id(bpg_pkg_t pkg, uint64_t client_id);

uint32_t bpg_pkg_sn(bpg_pkg_t pkg);
void bpg_pkg_set_sn(bpg_pkg_t pkg, uint32_t cmd);

void * bpg_pkg_body_data(bpg_pkg_t pkg);
uint32_t bpg_pkg_body_len(bpg_pkg_t pkg);
uint32_t bpg_pkg_body_origin_len(bpg_pkg_t pkg);

int32_t bpg_pkg_append_info_count(bpg_pkg_t pkg);
bpg_pkg_append_info_t bpg_pkg_append_info_at(bpg_pkg_t pkg, int32_t pos);

uint32_t bpg_pkg_append_info_id(bpg_pkg_append_info_t append_info);
void * bpg_pkg_append_info_data(bpg_pkg_t pkg, bpg_pkg_append_info_t append_info);
uint32_t bpg_pkg_append_info_size(bpg_pkg_append_info_t append_info);
uint32_t bpg_pkg_append_info_origin_size(bpg_pkg_append_info_t append_info);

LPDRMETALIB bpg_pkg_data_meta_lib(bpg_pkg_t pkg);
LPDRMETA bpg_pkg_main_data_meta(bpg_pkg_t pkg, error_monitor_t em);
LPDRMETA bpg_pkg_append_data_meta(bpg_pkg_t pkg, bpg_pkg_append_info_t append_info, error_monitor_t em);

int bpg_pkg_set_main_data(bpg_pkg_t pkg, LPDRMETA meta, const void * buf, size_t capacity, size_t * size, error_monitor_t em);
int bpg_pkg_add_append_data(bpg_pkg_t pkg, LPDRMETA meta, const void * buf, size_t capacity, size_t * size, error_monitor_t em);

int bpg_pkg_get_main_data(bpg_pkg_t pkg, LPDRMETA meta, void * buf, size_t * capacity, error_monitor_t em);
int bpg_pkg_get_append_data(bpg_pkg_t pkg, bpg_pkg_append_info_t append_inf, LPDRMETA meta, void * buf, size_t * capacity, error_monitor_t em);

const char * bpg_pkg_dump(bpg_pkg_t req, mem_buffer_t buffer);
int bpg_pkg_build_from_cfg(bpg_pkg_t req, cfg_t cfg, error_monitor_t em);

bpg_pkg_debug_level_t bpg_pkg_debug_level(bpg_pkg_t req);

#ifdef __cplusplus
}
#endif

#endif
