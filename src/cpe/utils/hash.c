#include <assert.h>
#include <strings.h>
#include "cpe/utils/hash.h"

struct cpe_hash_bucket {
    struct cpe_hash_entry * m_entry;
};

#define CPE_HASH_BUCKET_FREE_LIST_COUNT_PRE_ALLOC (64)

#define cpe_hash_entry_2_obj(hstable, entry)    \
    (((char*)entry) - hstable->m_obj2Entry)

#define cpe_hash_obj_2_entry(hstable, obj)    \
    ((struct cpe_hash_entry *)( ((char*)obj) + hstable->m_obj2Entry ))

uint32_t cpe_hash_str(const void * str, size_t len) {
    size_t i;
	uint32_t h = (uint32_t)len;
	size_t step = (len >> 5) + 1;
	for (i = len; i >= step; i -= step) {
	    h = h ^ ((h<<5)+(h>>2)+(uint32_t)((const char *)str)[i-1]);
    }

	return h;
}

static int32_t cpe_prime_array[] = {
    17,             /* 0 */
    37,             /* 1 */
    79,             /* 2 */
    163,            /* 3 */
    331,            /* 4 */
    673,            /* 5 */
    1361,           /* 6 */
    2729,           /* 7 */
    5471,           /* 8 */
    10949,          /* 9 */
    21911,          /* 10 */
    43853,          /* 11 */
    87719,          /* 12 */
    175447,         /* 13 */
    350899,         /* 14 */
    701819,         /* 15 */
    1403641,        /* 16 */
    2807303,        /* 17 */
    5614657,        /* 18 */
    11229331,       /* 19 */
    22458671,       /* 20 */
    44917381,       /* 21 */
    89834777,       /* 22 */
    179669557,      /* 23 */
    359339171,      /* 24 */
    718678369,      /* 25 */
    1437356741,     /* 26 */
    2147483647      /* 27 (largest signed int prime) */
};               

static int32_t select_next_prime(int32_t size) {
    int i;
    for(i = 0; i < sizeof(cpe_prime_array) / sizeof(int32_t); ++i) {
        if (cpe_prime_array[i] >= size) {
            return cpe_prime_array[i];
        }
    }

    return -1;
}

int cpe_hash_table_init(
    cpe_hash_table_t hashtable,
    mem_allocrator_t alloc,
    cpe_hash_fun_t hash_fun,
    cpe_hash_cmp_t cmp_fun,
    int32_t obj2Entry,
    int32_t suggestCapacity)
{
    int bucketsBufSize;

    if (hashtable == NULL) return -1;

    hashtable->m_alloc = alloc;
    hashtable->m_hash_fun = hash_fun;
    hashtable->m_compare_fun = cmp_fun;
    hashtable->m_destory_fun = NULL;
    hashtable->m_destory_context = NULL;
    hashtable->m_count = 0;
    hashtable->m_obj2Entry = obj2Entry;

    /*create bulck list*/
    hashtable->m_bucket_capacity = select_next_prime(suggestCapacity);
    if (hashtable->m_bucket_capacity < 0) return -1;

    bucketsBufSize = sizeof(struct cpe_hash_bucket) * hashtable->m_bucket_capacity;
    hashtable->m_buckets = mem_alloc(alloc, bucketsBufSize);
    if (hashtable->m_buckets == NULL) return -1;
    bzero(hashtable->m_buckets, bucketsBufSize);

    return 0;
}

void cpe_hash_table_set_destory_fun(
    cpe_hash_table_t hstable,
    cpe_hash_destory_t destory_fun,
    void * destory_context)
{
    if (hstable == NULL) return;

    hstable->m_destory_fun = destory_fun;
    hstable->m_destory_context = destory_context;
}

void cpe_hash_table_fini(cpe_hash_table_t hstable) {
    if (hstable == NULL) return;

    if (hstable->m_destory_fun) {
        //TODO: destory all object;
        /* struct cpe_hash_entry * e = hstable->m_list; */
        /* while(e) { */
        /*     struct cpe_hash_entry * c = e; */
        /*     e = e->m_next; */
        /*     hstable->m_destory_fun( */
        /*         cpe_hash_entry_2_obj(hstable, c), */
        /*         hstable->m_destory_context); */
        /* } */
    }

    if (hstable->m_buckets) {
        mem_free(hstable->m_alloc, hstable->m_buckets);
        hstable->m_buckets = NULL;
    }
}

int cpe_hash_table_insert_unique_noresize(cpe_hash_table_t hstable, void * obj) {
    int32_t bucketPos;
    struct cpe_hash_entry * entry;
    struct cpe_hash_entry ** bucket_insert;

    entry = cpe_hash_obj_2_entry(hstable, obj);

    entry->m_value = hstable->m_hash_fun(obj);
    bucketPos = entry->m_value % hstable->m_bucket_capacity;

    bucket_insert = &(hstable->m_buckets + bucketPos)->m_entry;

    while(*bucket_insert) {
        bucket_insert = &(*bucket_insert)->m_next;
    }

    entry->m_prev = bucket_insert;
    entry->m_next = NULL;
    *bucket_insert = entry;

    ++hstable->m_count;

    return 0;
}

int cpe_hash_table_insert_unique(cpe_hash_table_t hstable, void * obj) {
    return cpe_hash_table_insert_unique_noresize(hstable, obj);
}

int cpe_hash_table_remove_by_ins(cpe_hash_table_t hstable, void * obj) {
    struct cpe_hash_entry * entry;

    entry = cpe_hash_obj_2_entry(hstable, obj);
    (*entry->m_prev)->m_next = entry->m_next;
    if (entry->m_next) entry->m_next->m_prev = entry->m_prev;

    entry->m_next = NULL;
    entry->m_prev = NULL;

    /*destory object*/
    if (hstable->m_destory_fun) {
        hstable->m_destory_fun(obj, hstable->m_destory_context);
    }

    return 0;
}

void * cpe_hash_table_find(cpe_hash_table_t hstable, const void * obj) {
    int32_t bucketPos;
    int32_t hsvalue;
    struct cpe_hash_entry * entry;
    struct cpe_hash_entry ** find_pos;

    hsvalue = hstable->m_hash_fun(obj); 

    bucketPos = entry->m_value % hstable->m_bucket_capacity;

    find_pos = &(hstable->m_buckets + bucketPos)->m_entry;

    while(*find_pos) {
        void * checkObj = cpe_hash_entry_2_obj(hstable, *find_pos);
        if (hstable->m_compare_fun(checkObj, obj)) {
            return checkObj;
        }
        find_pos = &(*find_pos)->m_next;
    }

    return NULL;
}
