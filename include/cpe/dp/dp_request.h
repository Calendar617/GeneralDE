#ifndef CPE_DP_REQUEST_H
#define CPE_DP_REQUEST_H
#include "cpe/utils/hash_string.h"
#include "cpe/utils/error.h"
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

dp_req_t
dp_req_create(
    dp_mgr_t mgr,
    cpe_hash_string_t type,
    size_t capacity);

dp_req_t
dp_req_create_with_buf(
    dp_mgr_t mgr,
    cpe_hash_string_t type,
    void * data,
    size_t capacity);

void dp_req_free(dp_req_t req);

dp_mgr_t dp_req_mgr(dp_req_t req);

void dp_req_set_parent(dp_req_t child, dp_req_t parent);

dp_req_t dp_req_parent(dp_req_t req);
dp_req_t dp_req_parent_find(dp_req_t req, cpe_hash_string_t typeName);
dp_req_t dp_req_brother_find(dp_req_t req, cpe_hash_string_t typeName);
dp_req_t dp_req_child_find(dp_req_t req, cpe_hash_string_t typeName);

dp_node_t dp_req_from(dp_req_t req);
void dp_req_set_from(dp_req_t req, dp_node_t from);

dp_node_t dp_req_to(dp_req_t req);
void dp_req_set_to(dp_req_t req, dp_node_t to);

void dp_req_set_buf(dp_req_t req, void * buf, size_t capacity);

cpe_hash_string_t dp_req_type_hs(dp_req_t req);
const char * dp_req_type(dp_req_t req);

void * dp_req_data(dp_req_t req);
size_t dp_req_capacity(dp_req_t req);
size_t dp_req_size(dp_req_t req);
int dp_req_set_size(dp_req_t req, size_t size);

mem_allocrator_t dp_req_talloc(dp_req_t req);

int dp_req_send(dp_req_t req, error_monitor_t em);
int dp_req_reply(dp_req_t req, void * buf, size_t size, error_monitor_t em);

extern cpe_hash_string_t dp_req_type_reply;

#ifdef __cplusplus
}
#endif

#endif


