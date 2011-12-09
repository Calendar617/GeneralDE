#ifndef GD_DP_REQUEST_H
#define GD_DP_REQUEST_H
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void * gd_dp_req_data(gd_dp_req_t req);
size_t gd_dp_req_size(gd_dp_req_t req);
mem_allocrator_t gd_dp_req_talloc(gd_dp_req_t req);

#ifdef __cplusplus
}
#endif

#endif


