#ifndef GD_APP_CONTEXT_H
#define GD_APP_CONTEXT_H
#include "cpe/utils/memory.h"
#include "app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_app_context_t gd_app_context_create(
    mem_allocrator_t alloc, size_t capacity,
    int argc, char ** argv);

void gd_app_context_free(gd_app_context_t context);

int gd_app_context_argc(gd_app_context_t context);
char ** gd_app_context_argv(gd_app_context_t context);
void * gd_app_context_user_data(gd_app_context_t context);

int gd_app_run(gd_app_context_t context);

#ifdef __cplusplus
}
#endif

#endif
