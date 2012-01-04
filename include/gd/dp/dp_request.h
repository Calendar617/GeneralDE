#ifndef GD_DP_REQUEST_H
#define GD_DP_REQUEST_H
#include "cpe/utils/hash_string.h"
#include "cpe/utils/error.h"
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_dp_req_t
gd_dp_req_create(
    gd_dp_mgr_t mgr,
    cpe_hash_string_t type,
    size_t capacity);

gd_dp_req_t
gd_dp_req_create_with_buf(
    gd_dp_mgr_t mgr,
    cpe_hash_string_t type,
    void * data,
    size_t capacity);

void gd_dp_req_free(gd_dp_req_t req);

void gd_dp_req_set_parent(gd_dp_req_t child, gd_dp_req_t parent);

gd_dp_req_t gd_dp_req_parent(gd_dp_req_t req);
gd_dp_req_t gd_dp_req_parent_find(gd_dp_req_t req, cpe_hash_string_t typeName);
gd_dp_req_t gd_dp_req_brother_find(gd_dp_req_t req, cpe_hash_string_t typeName);
gd_dp_req_t gd_dp_req_child_find(gd_dp_req_t req, cpe_hash_string_t typeName);

gd_dp_node_t gd_dp_req_from(gd_dp_req_t req);
void gd_dp_req_set_from(gd_dp_req_t req, gd_dp_node_t from);

gd_dp_node_t gd_dp_req_to(gd_dp_req_t req);
void gd_dp_req_set_to(gd_dp_req_t req, gd_dp_node_t to);

int gd_dp_req_set_buf(void * buf, size_t capacity);

cpe_hash_string_t gd_dp_req_type_hs(gd_dp_req_t req);
const char * gd_dp_req_type(gd_dp_req_t req);

void * gd_dp_req_data(gd_dp_req_t req);
size_t gd_dp_req_capacity(gd_dp_req_t req);
size_t gd_dp_req_size(gd_dp_req_t req);
int gd_dp_req_set_size(gd_dp_req_t req, size_t size);

mem_allocrator_t gd_dp_req_talloc(gd_dp_req_t req);

int gd_dp_req_send(gd_dp_req_t req, error_monitor_t em);
int gd_dp_req_reply(gd_dp_req_t req, void * buf, size_t size, error_monitor_t em);

extern cpe_hash_string_t gd_dp_req_type_reply;

#ifdef __cplusplus
}
#endif

#endif


