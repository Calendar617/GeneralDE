#ifndef CPE_DR_METAINOUT_BUILDER_H
#define CPE_DR_METAINOUT_BUILDER_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/memory.h"
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dr_metalib_source_relation;
typedef TAILQ_HEAD(dr_metalib_source_list, dr_metalib_source) dr_metalib_source_list_t;
typedef TAILQ_HEAD(dr_metalib_source_relation_list, dr_metalib_source_relation) dr_metalib_source_relation_list_t;

struct dr_metalib_builder {
    mem_allocrator_t m_alloc;
    struct DRInBuildMetaLib * m_inbuild_lib;
    struct cpe_hash_table m_sources;
};

struct dr_metalib_source {
    dr_metalib_builder_t m_builder;
    const char * m_name;
    dr_metalib_source_relation_list_t m_includes;
    dr_metalib_source_relation_list_t m_include_by;

    struct cpe_hash_entry m_hh;
};

struct dr_metalib_source_relation {
    dr_metalib_source_t m_user;
    dr_metalib_source_t m_using;
    TAILQ_ENTRY(dr_metalib_source_relation) m_next_for_includes;
    TAILQ_ENTRY(dr_metalib_source_relation) m_next_for_include_by;
};

#ifdef __cplusplus
}
#endif

#endif


