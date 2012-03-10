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
typedef TAILQ_HEAD(logic_executor_list, logic_executor) logic_executor_list_t;

struct logic_manage {
    mem_allocrator_t m_alloc;
    gd_app_context_t m_app;

    uint32_t m_context_id;
    uint32_t m_require_id;

    struct cpe_hash_table m_contexts;
    struct cpe_hash_table m_requires;
    struct cpe_hash_table m_datas;
    struct cpe_hash_table m_require_types;
};

struct logic_stack_item {
    logic_executor_t m_executr;
};

struct logic_stack {
    struct logic_stack_item m_inline_items[8];
    struct logic_stack_item * m_extern_items;
    int32_t m_extern_items_capacity;
    int32_t m_item_pos; 
};

struct logic_context {
    logic_manage_t m_mgr;
    logic_context_id_t m_id;
    logic_context_state_t m_state;
    logic_context_commit_fun_t m_commit_op;
    void * m_commit_ctx;
    size_t m_capacity;
    uint32_t m_flags;

    logic_data_list_t m_datas;
    uint32_t m_require_waiting_count;
    logic_require_list_t m_requires;

    struct logic_stack m_stack;
    int32_t m_errno;

    struct cpe_hash_entry m_hh;
};

struct logic_require {
    logic_context_t m_ctx;
    logic_require_id_t m_id;
    logic_require_state_t m_state;
    logic_require_type_t m_type;
    size_t m_capacity;

    TAILQ_ENTRY(logic_require) m_next;
    struct cpe_hash_entry m_hh;
};

struct logic_require_type {
    logic_manage_t m_mgr;
    cpe_hash_string_t m_name;

    logic_require_type_trigger_t m_destory_op;
    void * m_destory_ctx;

    logic_require_type_trigger_t m_cancel_op;
    void * m_cancel_ctx;

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

#define LOGIC_EXECUTOR_COMMON                   \
    logic_manage_t m_mgr;                       \
    const char * m_name;                        \
    TAILQ_ENTRY(logic_executor) m_next;         \
    logic_executor_category_t m_category

struct logic_executor {
    LOGIC_EXECUTOR_COMMON;
};

struct logic_executor_basic {
    LOGIC_EXECUTOR_COMMON;
    logic_op_fun_t m_op;
    void * m_ctx;
    cfg_t m_args;
};

struct logic_executor_decorate {
    LOGIC_EXECUTOR_COMMON;
    logic_decorate_fun_t m_op;
    void * m_ctx;
    logic_executor_t m_inner;
};

struct logic_executor_group {
    LOGIC_EXECUTOR_COMMON;
    logic_executor_list_t m_members;
};

struct logic_executor_type {
    logic_executor_type_group_t m_group;
};

struct logic_executor_type_group {
    mem_allocrator_t m_alloc;
    gd_app_context_t m_app;
};

#ifdef __cplusplus
}
#endif

#endif
