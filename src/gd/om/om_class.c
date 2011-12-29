#include <assert.h>
#include "cpe/utils/bitarry.h"
#include "cpe/utils/range_bitarry.h"
#include "gd/om/om_error.h"
#include "om_class.h"
#include "om_page_head.h"

uint32_t gd_om_class_hash_fun(struct gd_om_class * class) {
    return cpe_hs_value(class->m_name);
}

int gd_om_class_cmp_fun(struct gd_om_class * l, struct gd_om_class * r) {
    return cpe_hs_cmp(l->m_name, r->m_name) == 0;
}

static int gd_om_class_init(struct gd_om_class * class, mem_allocrator_t alloc) {
    class->m_name = (cpe_hash_string_t)class->m_name_buf;

    cpe_hash_entry_init(&class->m_hh);

    if (cpe_range_mgr_init(&class->m_range_alloc, alloc) != 0) return -1;

    return 0;
}

static void gd_om_class_fini(struct gd_om_class * class) {
    cpe_range_mgr_fini(&class->m_range_alloc);
}

static void gd_om_class_mgr_classes_fini(struct gd_om_class_mgr * classMgr, int endPos) {
    int pos;
    for(pos = endPos - 1; pos >= 0; --pos) {
        gd_om_class_fini(&classMgr->m_classes[pos]);
    }
}

int gd_om_class_mgr_init(struct gd_om_class_mgr * classMgr, mem_allocrator_t alloc) {
    int i;

    assert(classMgr);

    bzero(classMgr, sizeof(struct gd_om_class_mgr));

    for(i = 0; i < GD_OM_CLASS_BUF_LEN; ++i) {
        if(gd_om_class_init(&classMgr->m_classes[i], alloc) != 0) {
            gd_om_class_mgr_classes_fini(classMgr, i);
            return -1;
        }
    }

    if (cpe_hash_table_init(
            &classMgr->m_classNameIdx,
            alloc,
            (cpe_hash_fun_t)gd_om_class_hash_fun,
            (cpe_hash_cmp_t)gd_om_class_cmp_fun,
            CPE_HASH_OBJ2ENTRY(gd_om_class, m_hh),
            -1) != 0)
    {
        gd_om_class_mgr_classes_fini(classMgr, i);
        return -1;
    }

    return 0;
}

void gd_om_class_mgr_fini(struct gd_om_class_mgr * classMgr) {
    assert(classMgr);

    gd_om_class_mgr_classes_fini(classMgr, GD_OM_CLASS_BUF_LEN);
    cpe_hash_table_fini(&classMgr->m_classNameIdx);
}

gd_om_class_id_t
gd_om_calss_add(
    struct gd_om_class_mgr * classMgr,
    const char * className,
    size_t object_size,
    size_t page_size,
    size_t align,
    error_monitor_t em)
{
    int i;
    struct gd_om_class * class;

    assert(classMgr);
    assert(className);

    if (align != 1 && align != 2 && align != 4 && align != 8) {
        CPE_ERROR_EX(em, gd_om_error_invalid_align, "invalid align %d!", align);
        return GD_OM_INVALID_CLASSID;
    }

    if (object_size % align) {
        object_size = ((object_size / align) + 1) * align;
    }

    class = NULL;
    for(i = 1; i < GD_OM_CLASS_BUF_LEN; ++i) {
        if (classMgr->m_classes[i].m_id == GD_OM_INVALID_CLASSID) {
            class = &classMgr->m_classes[i];
            break;
        }
    }

    if (class == NULL) {
        CPE_ERROR_EX(em, gd_om_class_overflow, "too many class!");
        return GD_OM_INVALID_CLASSID;
    }

    if (page_size > 0x1FFFFFFF) {
        CPE_ERROR_EX(
            em, gd_om_page_size_too_big,
            "page size(%d) is bigger then %d!", page_size, 0x1FFFFFFF);
        return GD_OM_INVALID_CLASSID;
    }

    assert(class->m_name); /*set in init, point to m_name_buf*/
    cpe_hs_init(class->m_name, cpe_hs_len_to_binary_len(GD_OM_MAX_TYPENAME_LEN), className);

    class->m_object_size = object_size;
    class->m_page_size = page_size;

    class->m_object_per_page = ((page_size - sizeof(struct gd_om_data_page_head)) << 3) / ((object_size << 3) + 1);
    if (class->m_object_per_page < 10) {
        CPE_ERROR_EX(
            em, gd_om_page_size_too_small,
            "page size(%d) is too small, only can contain %d object(s)!",
            page_size, class->m_object_per_page);
        return GD_OM_INVALID_CLASSID;
    }

    class->m_alloc_buf_capacity = cpe_ba_bytes_from_bits(class->m_object_per_page);
    class->m_object_buf_begin_in_page = (sizeof(struct gd_om_data_page_head) + class->m_alloc_buf_capacity);

    if (class->m_object_buf_begin_in_page % align) {
        class->m_object_buf_begin_in_page = ((class->m_object_buf_begin_in_page / align) + 1) * align;
        class->m_object_per_page = (page_size - class->m_object_buf_begin_in_page) / object_size;
        class->m_alloc_buf_capacity = cpe_ba_bytes_from_bits(class->m_object_per_page);
    }

    assert((class->m_object_per_page * class->m_object_size)
           <= (class->m_page_size - class->m_object_buf_begin_in_page));

    if (cpe_hash_table_insert_unique(&classMgr->m_classNameIdx, class) != 0) {
        CPE_ERROR_EX(
            em, gd_om_class_name_duplicate,
            "class %s name duplicate!", className);
        return GD_OM_INVALID_CLASSID;
    }

    /*last operation, set id*/
    class->m_id = i;
    return i;
}

struct gd_om_class *
gd_om_class_get(struct gd_om_class_mgr * classMgr, gd_om_class_id_t classId) {
    struct gd_om_class * r;

    assert(classMgr);

    r = &classMgr->m_classes[classId];

    return r->m_id == GD_OM_INVALID_CLASSID
        ? NULL
        : r;
}

struct gd_om_class *
gd_om_class_find(struct gd_om_class_mgr * classMgr, cpe_hash_string_t className) {
    struct gd_om_class key;

    assert(classMgr);
    assert(className);

    key.m_name = className;

    return (struct gd_om_class *)cpe_hash_table_find(&classMgr->m_classNameIdx, &key);
}

static int gd_om_class_reserve_page_array_slot(struct gd_om_class * class, error_monitor_t em) {
    if (class->m_page_array_size >= class->m_page_array_capacity) {
        size_t new_page_array_capacity = class->m_page_array_capacity + 1024;
        void * new_page_array = mem_alloc(class->m_alloc, gd_om_class_page_buf_len(new_page_array_capacity));
        if (new_page_array == NULL) {
            CPE_ERROR_EX(em, gd_om_no_memory, "alloc for buffer list fail!");
            return -1;
        }

        if (class->m_page_array_size > 0) {
            memcpy(new_page_array, class->m_page_array, gd_om_class_page_buf_len(class->m_page_array_size));
        }

        mem_free(class->m_alloc, class->m_page_array);
        class->m_page_array = new_page_array;
        class->m_page_array_capacity = new_page_array_capacity;
    }

    return 0;
}

int gd_om_class_add_new_page(struct gd_om_class * class, void * page, error_monitor_t em) {
    int32_t newRangeStart;
    struct gd_om_data_page_head * head;
    cpe_ba_t alloc_arry;

    if (gd_om_class_reserve_page_array_slot(class, em) != 0) return -1;

    head = (struct gd_om_data_page_head *)page;
    alloc_arry = (cpe_ba_t)(head + 1);

    head->m_magic = GD_OM_PAGE_MAGIC;
    head->m_classId = class->m_id;
    head->m_reserve = 0;
    head->m_page_idx = class->m_page_array_size;
    head->m_reserve2 = 0;
    cpe_ba_set_all(alloc_arry, class->m_alloc_buf_capacity, cpe_ba_false);

    newRangeStart = class->m_object_per_page * class->m_page_array_size;
    cpe_range_put_range(&class->m_range_alloc, newRangeStart, newRangeStart + class->m_object_per_page);

    class->m_page_array[class->m_page_array_size] = page;
    ++class->m_page_array_size;

    return 0;
}

static int gd_om_class_check_page_head(struct gd_om_class * class, struct gd_om_data_page_head * head, error_monitor_t em) {
    if (head->m_magic != GD_OM_PAGE_MAGIC) {
        CPE_ERROR_EX(em, gd_om_page_head_error, "page head magic error!");
        return -1;
    }

    if (head->m_classId != class->m_id) {
        CPE_ERROR_EX(
            em, gd_om_page_head_error,
            "page head class id error, expect %d, but %d!",
            class->m_id, head->m_classId);
        return -1;
    }

    return 0;
}

int gd_om_class_add_old_page(struct gd_om_class * class, void * page, error_monitor_t em) {
    struct gd_om_data_page_head * head;
    cpe_ba_t alloc_arry;

    if (gd_om_class_reserve_page_array_slot(class, em) != 0) return -1;

    head = (struct gd_om_data_page_head *)page;
    if (gd_om_class_check_page_head(class, head, em) != 0) return -1;

    alloc_arry = gd_om_class_ba_of_page(page);

    if (cpe_range_free_from_bitarray(
            &class->m_range_alloc,
            alloc_arry,
            class->m_object_per_page * class->m_page_array_size,
            class->m_object_per_page) != 0)
    {
        int i;
        CPE_ERROR_EX(em, gd_om_no_memory, "alloc page range no memory!");

        cpe_range_mgr_clear(&class->m_range_alloc);
        for(i = 0; i < class->m_page_array_size; ++i) {
            cpe_range_free_from_bitarray(
                &class->m_range_alloc,
                gd_om_class_ba_of_page(class->m_page_array[i]),
                class->m_object_per_page * i,
                class->m_object_per_page);
        }

        return -1;
    }

    assert(class->m_page_array_size + 1 < class->m_page_array_capacity);
    class->m_page_array[class->m_page_array_size] = page;
    ++class->m_page_array_size;

    return 0;
}

int32_t
gd_om_class_alloc_object(struct gd_om_class * class) {
    int32_t r;

    assert(class);

    r = cpe_range_get_one(&class->m_range_alloc);
    if (r >= 0) {
        cpe_ba_t alloc_arry;
        int32_t pagePos;

        pagePos = (int32_t)(r / class->m_object_per_page);
        assert(pagePos >= 0 && pagePos < class->m_page_array_size);

        alloc_arry = gd_om_class_ba_of_page(class->m_page_array[pagePos]);

        cpe_ba_set(alloc_arry, r % class->m_object_per_page, cpe_ba_true);
    }

    return r;
}

void gd_om_class_free_object(struct gd_om_class * class, int32_t oid, error_monitor_t em) {
    cpe_ba_t alloc_arry;
    int32_t pagePos;
    assert(class);

    pagePos = (int32_t)(oid / class->m_object_per_page);

    if(pagePos < 0 || pagePos >= class->m_page_array_size) {
        CPE_ERROR_EX(
            em, gd_om_error_invalid_oid, "class %s: page pos %d overflow, page count is %zu!", 
            cpe_hs_data(class->m_name), pagePos, class->m_page_array_size);
        return;
    }

    if (cpe_range_put_one(&class->m_range_alloc, oid) != 0) {
        CPE_ERROR_EX(em, gd_om_no_memory, "no memory: free to range fail!")
        return;
    }

    alloc_arry = gd_om_class_ba_of_page(class->m_page_array[pagePos]);
    cpe_ba_set(alloc_arry, oid % class->m_object_per_page, cpe_ba_false);
}

void * gd_om_class_get_object(struct gd_om_class * class, int32_t oid, error_monitor_t em) {
    cpe_ba_t alloc_arry;
    int32_t pagePos;
    int32_t posInPage;
    char * page;
    assert(class);

    pagePos = (int32_t)(oid / class->m_object_per_page);

    if(pagePos < 0 || pagePos >= class->m_page_array_size) {
        CPE_ERROR_EX(
            em, gd_om_error_invalid_oid, "class %s: page pos %d overflow, page count is %zu!", 
            cpe_hs_data(class->m_name), pagePos, class->m_page_array_size);
        return NULL;
    }

    posInPage = oid % class->m_object_per_page;
    page = (char*)class->m_page_array[pagePos];

    alloc_arry = gd_om_class_ba_of_page(page);
    if (cpe_ba_get(alloc_arry, posInPage) != cpe_ba_true) {
        CPE_ERROR_EX(
            em, gd_om_error_invalid_oid, "class %s: oid %d not alloced!", 
            cpe_hs_data(class->m_name), oid);
        return NULL;
    }

    return (void*)(page + class->m_object_buf_begin_in_page + (class->m_object_size * posInPage));
}
