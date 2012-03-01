#ifndef USF_LOGIC_BUILD_H
#define USF_LOGIC_BUILD_H
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_executor_t
logic_executor_build(
    logic_manage_t mgr,
    cfg_t cfg);

#ifdef __cplusplus
}
#endif

#endif

