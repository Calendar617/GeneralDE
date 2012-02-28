#ifndef USF_LOGIC_MANAGE_H
#define USF_LOGIC_MANAGE_H
#include "cpe/utils/memory.h"
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_manage_t
logic_manage_create(mem_allocrator_t alloc);

void logic_manage_free(logic_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif

