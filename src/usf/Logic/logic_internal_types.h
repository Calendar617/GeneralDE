#ifndef USF_LOGIC_INTERNAL_TYPES_H
#define USF_LOGIC_INTERNAL_TYPES_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/hash_string.h"
#include "usf/logic/logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef TAILQ_HEAD(logic_require_list, logic_require) logic_require_list_t;
typedef TAILQ_HEAD(logic_data_list, logic_data) logic_data_list_t;

struct logic_manage {
    mem_allocrator_t m_alloc;

    uint32_t m_context_id;
    uint32_t m_require_id;

    struct cpe_hash_table m_contexts;
    struct cpe_hash_table m_requires;
    struct cpe_hash_table m_datas;
    struct cpe_hash_table m_require_types;
};

struct logic_context {
    logic_manage_t m_mgr;
    logic_context_id_t m_id;

    logic_data_list_t m_datas;
    logic_require_list_t m_requires;

    struct cpe_hash_entry m_hh;
};

struct logic_require {
    logic_context_t m_ctx;
    logic_require_id_t m_id;
    logic_require_state_t m_state;
    logic_require_type_t m_type;

    TAILQ_ENTRY(logic_require) m_next;
    struct cpe_hash_entry m_hh;
};

struct logic_require_type {
    logic_manage_t m_mgr;
    cpe_hash_string_t m_name;

    struct cpe_hash_entry m_hh;
};

struct logic_data {
    logic_context_t m_ctx;    
    const char * m_name;
    LPDRMETA m_meta;
    size_t m_capacity;

    TAILQ_ENTRY(logic_data) m_next;
    struct cpe_hash_entry m_hh;
};


#ifdef __cplusplus
}
#endif

#endif
