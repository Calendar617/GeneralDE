#ifndef USF_BASEPKG_TYPES_H
#define USF_BASEPKG_TYPES_H
#include "cpe/utils/error.h"
#include "cpe/cfg/cfg_types.h"
#include "cpe/dr/dr_types.h"
#include "usf/logic/logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum bpg_rsp_flag {
    bpg_rsp_flag_debug = 1 << 0
} bpg_rsp_flag_t;

typedef enum bpg_manage_flag {
    bpg_manage_flag_sn_use_client = 1 << 0
} bpg_manage_flag_t;

typedef struct bpg_manage * bpg_manage_t;
typedef struct bpg_rsp * bpg_rsp_t;

typedef int (*bpg_logic_ctx_init_fun_t)(logic_context_t context, void * ctx);
typedef void (*bpg_logic_ctx_fini_fun_t)(logic_context_t context, void * ctx);

typedef int (*bpg_data_convert_fun_t)(
    void * output, size_t output_capacity,
    const void * input, size_t input_capacity,
    void * ctx,
    error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
