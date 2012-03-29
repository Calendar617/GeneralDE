#ifndef GD_OM_CLASS_H
#define GD_OM_CLASS_H
#include "cpe/utils/hash.h"
#include "cpe/utils/error.h"
#include "cpe/utils/range.h"
#include "gd/om/om_class.h"
#include "gd/om/om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GD_OM_CLASS_BUF_LEN (GD_OM_MAX_TYPE_COUNT + 1)

struct gd_om_page;

struct gd_om_class {
    gd_om_class_id_t m_id;
    char m_name_buf[cpe_hs_len_to_binary_len(GD_OM_MAX_TYPENAME_LEN)];
    cpe_hash_string_t m_name;
    struct cpe_hash_entry m_hh;
    struct cpe_range_mgr m_range_alloc;
    mem_allocrator_t m_alloc;
    size_t m_object_size;

    size_t m_page_size;
    size_t m_object_per_page;
    size_t m_alloc_buf_capacity;
    size_t m_object_buf_begin_in_page;

    size_t m_page_array_capacity;
    size_t m_page_array_size;
    void * * m_page_array;
};

struct gd_om_class_mgr {
    struct gd_om_class m_classes[GD_OM_CLASS_BUF_LEN];
    struct cpe_hash_table m_classNameIdx;
};

int gd_om_class_mgr_init(struct gd_om_class_mgr * classMgr, mem_allocrator_t alloc);
void gd_om_class_mgr_fini(struct gd_om_class_mgr * classMgr);

gd_om_class_id_t
gd_om_class_add(
    struct gd_om_class_mgr * classMgr,
    const char * className,
    size_t object_size,
    size_t page_size,
    size_t align,
    error_monitor_t em);

int gd_om_class_add_with_id(
    struct gd_om_class_mgr * classMgr,
    gd_om_class_id_t classId,
    const char * className,
    size_t object_size,
    size_t page_size,
    size_t align,
    error_monitor_t em);

struct gd_om_class *
gd_om_class_get(struct gd_om_class_mgr * classMgr, gd_om_class_id_t classId);

struct gd_om_class *
gd_om_class_find(struct gd_om_class_mgr * classMgr, cpe_hash_string_t className);

int gd_om_class_add_page(struct gd_om_class *cls, void * page, error_monitor_t em);

int32_t gd_om_class_alloc_object(struct gd_om_class *cls);
int32_t gd_om_class_addr_2_object(struct gd_om_class *cls, void * page, void * addr);

void gd_om_class_free_object(struct gd_om_class *cls, int32_t value, error_monitor_t em);
void * gd_om_class_get_object(struct gd_om_class *cls, int32_t value, error_monitor_t em);

#define gd_om_class_ba_of_page(page) (cpe_ba_t)(((char*)(page)) + sizeof(struct gd_om_data_page_head))
#define gd_om_class_page_buf_len(page_count) (sizeof(void*) * (page_count))

#ifdef __cplusplus
}
#endif

#endif
