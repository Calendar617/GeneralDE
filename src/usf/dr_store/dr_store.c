#include <assert.h>
#include "cpe/cfg/cfg_manage.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_cfg.h"
#include "usf/dr_store/dr_store.h"
#include "dr_store_internal_ops.h"

dr_store_t
dr_store_create(dr_store_manage_t mgr, const char * name) {
    char * buf;
    dr_store_t dr_store;
    size_t name_len;

    name_len = strlen(name) + 1;

    buf = mem_alloc(mgr->m_alloc, sizeof(struct dr_store) + name_len);
    if (buf == NULL) return NULL;

    memcpy(buf, name, name_len);

    dr_store = (dr_store_t)(buf + name_len);

    dr_store->m_mgr = mgr;
    dr_store->m_name = name;
    dr_store->m_lib = NULL;
    dr_store->m_free_fun = NULL;
    dr_store->m_free_ctx = NULL;

    cpe_hash_entry_init(&dr_store->m_hh);
    if (cpe_hash_table_insert_unique(&mgr->m_stores, dr_store) != 0) {
        mem_free(mgr->m_alloc, buf);
        return NULL;
    }

    return dr_store;
}

void dr_store_free(dr_store_t dr_store) {
    assert(dr_store);
    cpe_hash_table_remove_by_ins(&dr_store->m_mgr->m_stores, dr_store);
    mem_free(dr_store->m_mgr->m_alloc, (void*)dr_store->m_name);
}

void dr_store_free_all(dr_store_manage_t mgr) {
    struct cpe_hash_it dr_store_it;
    dr_store_t dr_store;

    cpe_hash_it_init(&dr_store_it, &mgr->m_stores);

    dr_store = cpe_hash_it_next(&dr_store_it);
    while (dr_store) {
        dr_store_t next = cpe_hash_it_next(&dr_store_it);
        dr_store_free(dr_store);
        dr_store = next;
    }
}

dr_store_t
dr_store_find(dr_store_manage_t mgr, const char * name) {
    struct dr_store key;

    key.m_name = name;
    return (dr_store_t)cpe_hash_table_find(&mgr->m_stores, &key);
}

dr_store_t
dr_store_find_or_create(dr_store_manage_t mgr, const char * name) {
    dr_store_t r = dr_store_find(mgr, name);
    if (r == NULL) r = dr_store_create(mgr, name);
    return r;
}

LPDRMETALIB dr_store_lib(dr_store_t dr_store) {
    return dr_store->m_lib;
}

int dr_store_set_lib(dr_store_t dr_store, LPDRMETALIB lib, dr_lib_free_fun_t free_fun, void * free_ctx) {
    if (dr_store->m_lib) return -1;

    dr_store->m_lib = lib;
    dr_store->m_free_fun = free_fun;
    dr_store->m_free_ctx = free_ctx;

    return 0;
}

void dr_store_reset_lib(dr_store_t dr_store, LPDRMETALIB lib, dr_lib_free_fun_t free_fun, void * free_ctx) {
    if (dr_store->m_lib && dr_store->m_free_fun) {
        dr_store->m_free_fun(dr_store->m_lib, dr_store->m_free_ctx);
    }

    dr_store->m_lib = lib;
    dr_store->m_free_fun = free_fun;
    dr_store->m_free_ctx = free_ctx;
}
