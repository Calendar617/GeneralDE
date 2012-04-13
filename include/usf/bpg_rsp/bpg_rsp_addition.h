#ifndef USF_BPG_RSP_ADDITION_H
#define USF_BPG_RSP_ADDITION_H
#include "bpg_rsp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int16_t bpg_rsp_addition_data_count(logic_context_t ctx);
uint32_t bpg_rsp_addition_data_at(logic_context_t ctx, int16_t pos);
int bpg_rsp_addition_data_add(logic_context_t ctx, uint32_t meta_id);

#ifdef __cplusplus
}
#endif

#endif
