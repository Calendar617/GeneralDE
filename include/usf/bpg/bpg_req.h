#ifndef USF_BPG_REQ_H
#define USF_BPG_REQ_H
#include "bpg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern cpe_hash_string_t bpg_req_type_name;

bpg_req_t
bpg_req_create(
    gd_app_context_t app,
    size_t capacity,
    LPDRMETA carry_data_meta,
    size_t carry_data_capacity);

void bpg_req_free(bpg_req_t req);

LPDRMETA bpg_req_carry_data_meta(bpg_req_t req);
size_t bpg_req_carry_data_capacity(bpg_req_t req);

void * bpg_req_carry_data(bpg_req_t req);
size_t bpg_req_carry_data_size(bpg_req_t req);
int bpg_req_carry_data_set_size(bpg_req_t req, size_t size);

size_t bpg_req_pkg_capacity(bpg_req_t req);

void * bpg_req_pkg_data(bpg_req_t req);
size_t bpg_req_pkg_data_size(bpg_req_t req);
int bpg_req_pkg_data_set_size(bpg_req_t req, size_t size);

gd_dp_req_t bpg_req_to_dp_req(bpg_req_t req);
bpg_req_t bpg_req_from_dp_req(gd_dp_req_t req);

#ifdef __cplusplus
}
#endif

#endif
