#ifndef GD_APP_SVR_CONTEXT_H
#define GD_APP_SVR_CONTEXT_H
#include "cpe/utils/memory.h"
#include "gd/app/app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_app_context {
    int m_argc;
    char ** m_argv;
    mem_allocrator_t m_alloc;
    size_t m_capacity;
};

#ifdef __cplusplus
}
#endif

#endif

