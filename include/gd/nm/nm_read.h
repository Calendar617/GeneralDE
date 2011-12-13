#ifndef GD_NM_NODE_H
#define GD_NM_NODE_H
#include "cpe/utils/memory.h"
#include "nm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t gd_nm_node_capacity(gd_nm_node_t node);
void * gd_nm_node_data(gd_nm_node_t node);

#ifdef __cplusplus
}
#endif

#endif
