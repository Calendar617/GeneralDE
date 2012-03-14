#ifndef USF_UTILS_IDGENERATE_H
#define USF_UTILS_IDGENERATE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "gd/app/app_types.h"
#include "utils_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct usf_id_generator * usf_id_generator_t;

usf_id_generator_t
usf_id_generator_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc,
    error_monitor_t em);

void usf_id_generator_free(usf_id_generator_t mgr);

usf_id_generator_t
usf_id_generator_find(gd_app_context_t app, cpe_hash_string_t name);

usf_id_generator_t
usf_id_generator_find_nc(gd_app_context_t app, const char * name);

int usf_id_generator_generate(usf_id_t * r, usf_id_generator_t generator);

gd_app_context_t usf_id_generator_app(usf_id_generator_t generator);
const char * usf_id_generator_name(usf_id_generator_t generator);
cpe_hash_string_t usf_id_generator_name_hs(usf_id_generator_t generator);

#ifdef __cplusplus
}
#endif

#endif

