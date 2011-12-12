#ifndef CPE_UTILS_HASH_H
#define CPE_UTILS_HASH_H
#include "cpe/pal/types.h"
#include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t (*cpe_hash_fun_t)(const void *);
typedef int (*cpe_hash_cmp_t)(const void *, const void *);
typedef void (*cpe_hash_destory_t)(void * obj, void * context);

struct cpe_hash_entry {
    uint32_t m_value;
    struct cpe_hash_entry * m_next;
    struct cpe_hash_entry ** m_prev;
};

struct cpe_hash_bucket;

typedef struct cpe_hash_table {
    mem_allocrator_t m_alloc;
    cpe_hash_fun_t m_hash_fun;
    cpe_hash_cmp_t  m_compare_fun;

     /*delta between object end entry*/
    int32_t m_obj2Entry;

    /*auto destory object in fini*/
    cpe_hash_destory_t m_destory_fun;
    void * m_destory_context;

    /*bulckets*/
    struct cpe_hash_bucket * m_buckets;
    int32_t m_bucket_capacity;

    int32_t m_count;
} * cpe_hash_table_t;

int cpe_hash_table_init(
    cpe_hash_table_t hstable,
    mem_allocrator_t alloc,
    cpe_hash_fun_t hash_fun,
    cpe_hash_cmp_t cmp_fun,
    int32_t obj2Entry,
    int32_t suggestCapacity);

void cpe_hash_table_set_destory_fun(
    cpe_hash_table_t hstable,
    cpe_hash_destory_t destory_fun,
    void * destory_context);

void cpe_hash_table_fini(cpe_hash_table_t hstable);

int cpe_hash_table_insert_unique(cpe_hash_table_t hstable, void * obj);
void * cpe_hash_table_find(cpe_hash_table_t hstable, const void * obj);
int cpe_hash_table_remove_by_ins(cpe_hash_table_t hstable, void * obj);

uint32_t cpe_hash_str(const void * str, size_t len);

#define CPE_HASH_OBJ2ENTRY(__typeName, __itemName)  (((char*)(&((struct __typeName *)1000)->__itemName)) - ((char*)1000))

#ifdef __cplusplus
}
#endif

#endif
