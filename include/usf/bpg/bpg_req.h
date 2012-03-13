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

void bpg_req_pkg_init(bpg_req_t req);

void bpg_req_set_convertor(bpg_req_t req, bpg_data_convert_fun_t encode, bpg_data_convert_fun_t decode, void * ctx);
bpg_data_convert_fun_t bpg_req_convertor_encode(bpg_req_t req);
bpg_data_convert_fun_t bpg_req_convertor_decode(bpg_req_t req);

int bpg_req_set_data(bpg_req_t req, LPDRMETA meta, void * buf, size_t capacity, error_monitor_t em);
int bpg_req_append_data(bpg_req_t req, LPDRMETA, void * buf, size_t capacity, error_monitor_t em);

gd_dp_req_t bpg_req_to_dp_req(bpg_req_t req);
bpg_req_t bpg_req_from_dp_req(gd_dp_req_t req);

int bpg_copy_convert(
    void * output, size_t output_capacity,
    const void * input, size_t input_capacity,
    void * ctx,
    error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
