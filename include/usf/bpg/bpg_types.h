#ifndef USF_BASEPKG_TYPES_H
#define USF_BASEPKG_TYPES_H
#include "cpe/utils/error.h"
#include "cpe/cfg/cfg_types.h"
#include "usf/logic/logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum bpg_rsp_flag {
    bpg_rsp_flag_debug = 1 << 0
} bpg_rsp_flag_t;

typedef struct bpg_manage * bpg_manage_t;
typedef struct bpg_rsp * bpg_rsp_t;

#ifdef __cplusplus
}
#endif

#endif
