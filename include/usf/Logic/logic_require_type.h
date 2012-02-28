#ifndef USF_LOGIC_REQUIRE_TYPE_H
#define USF_LOGIC_REQUIRE_TYPE_H
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_require_type_t
logic_require_type_create(logic_manage_t mgr);

void logic_require_type_free(logic_require_type_t rt);

#ifdef __cplusplus
}
#endif

#endif

