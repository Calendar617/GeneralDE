#include <assert.h>
#include "cpe/pal/pal_strings.h"
#include "cpe/pal/pal_stdio.h"
#include "cpe/utils/bitarry.h"
#include "cpe/utils/range_bitarry.h"
#include "gd/om/om_error.h"
#include "om_class_i.h"
#include "om_page_head.h"

uint32_t gd_om_class_hash_fun(struct gd_om_class * theClass) {
    return cpe_hs_value(theClass->m_name);
}

int gd_om_class_cmp_fun(struct gd_om_class * l, struct gd_om_class * r) {
    return cpe_hs_cmp(l->m_name, r->m_name) == 0;
}

static int gd_om_class_init(struct gd_om_class * theClass, mem_allocrator_t alloc) {
    theClass->m_name = (cpe_hash_string_t)theClass->m_name_buf;

    cpe_hash_entry_init(&theClass->m_hh);

    if (cpe_range_mgr_init(&theClass->m_range_alloc, alloc) != 0) return -1;

    return 0;
}

static void gd_om_class_fini(struct gd_om_class * theClass) {
    cpe_range_mgr_fini(&theClass->m_range_alloc);
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
gd_om_class_add(
    struct gd_om_class_mgr * classMgr,
    const char * className,
    size_t object_size,
    size_t page_size,
    size_t align,
    error_monitor_t em)
{
    int i;

    assert(classMgr);
    assert(className);

    for(i = 1; i < (int)GD_OM_CLASS_BUF_LEN; ++i) {
        if (classMgr->m_classes[i].m_id == GD_OM_INVALID_CLASSID) {
            break;
        }
    }

    if (i >= GD_OM_CLASS_BUF_LEN) {
        CPE_ERROR_EX(em, gd_om_class_overflow, "too many class!");
        return GD_OM_INVALID_CLASSID;
    }

    return 
        gd_om_class_add_with_id(
            classMgr,
            (gd_om_class_id_t)i,
            className,
            object_size,
            page_size,
            align,
            em) == 0
        ? (gd_om_class_id_t)i
        : GD_OM_INVALID_CLASSID;
}

int gd_om_class_add_with_id(
    struct gd_om_class_mgr * classMgr,
    gd_om_class_id_t classId,
    const char * className,
    size_t object_size,
    size_t page_size,
    size_t align,
    error_monitor_t em)
{
    struct gd_om_class * theClass;

    assert(classMgr);
    assert(className);

    if (align != 1 && align != 2 && align != 4 && align != 8) {
        CPE_ERROR_EX(em, gd_om_invalid_align, "invalid align "  FMT_SIZE_T "!", align);
        return -1;
    }

    if (object_size % align) {
        object_size = ((object_size / align) + 1) * align;
    }

    if (classId <= 0) {
        CPE_ERROR_EX(em, gd_om_class_overflow, "invalid class id!");
        return -1;
    }

    if (((int)classId - 1) >= (int)GD_OM_CLASS_BUF_LEN - 1) {
        CPE_ERROR_EX(em, gd_om_class_overflow, "class id overflow!");
        return -1;
    }

    if (strlen(className) > GD_OM_MAX_TYPENAME_LEN) {
        CPE_ERROR_EX(em, gd_om_class_name_too_long, "class name too long!");
        return -1;
    }

    theClass = &classMgr->m_classes[classId];

    if (page_size > 0x1FFFFFFF) {
        CPE_ERROR_EX(
            em, gd_om_page_size_too_big,
            "page size("  FMT_SIZE_T ") is bigger then %d!", page_size, 0x1FFFFFFF);
        return -1;
    }

    assert(theClass->m_name); /*set in init, point to m_name_buf*/
    cpe_hs_init(theClass->m_name, cpe_hs_len_to_binary_len(GD_OM_MAX_TYPENAME_LEN), className);

    theClass->m_object_size = object_size;
    theClass->m_page_size = page_size;

    theClass->m_object_per_page = ((page_size - sizeof(struct gd_om_data_page_head)) << 3) / ((object_size << 3) + 1);
    if (theClass->m_object_per_page < 10) {
        CPE_ERROR_EX(
            em, gd_om_page_size_too_small,
            "page size("  FMT_SIZE_T ") is too small, only can contain "  FMT_SIZE_T " object(s)!",
            page_size, theClass->m_object_per_page);
        return -1;
    }

    theClass->m_alloc_buf_capacity = cpe_ba_bytes_from_bits(theClass->m_object_per_page);
    theClass->m_object_buf_begin_in_page = (sizeof(struct gd_om_data_page_head) + theClass->m_alloc_buf_capacity);

    if (theClass->m_object_buf_begin_in_page % align) {
        theClass->m_object_buf_begin_in_page = ((theClass->m_object_buf_begin_in_page / align) + 1) * align;
        theClass->m_object_per_page = (page_size - theClass->m_object_buf_begin_in_page) / object_size;
        theClass->m_alloc_buf_capacity = cpe_ba_bytes_from_bits(theClass->m_object_per_page);
    }

    assert((theClass->m_object_per_page * theClass->m_object_size)
           <= (theClass->m_page_size - theClass->m_object_buf_begin_in_page));

    if (cpe_hash_table_insert_unique(&classMgr->m_classNameIdx, theClass) != 0) {
        CPE_ERROR_EX(
            em, gd_om_class_name_duplicate,
            "class %s name duplicate!", className);
        return -1;
    }

    /*last operation, set id*/
    theClass->m_id = classId;
    return 0;
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

static int gd_om_class_reserve_page_array_slot(struct gd_om_class * theClass, error_monitor_t em) {
    if (theClass->m_page_array_size >= theClass->m_page_array_capacity) {
        size_t new_page_array_capacity = theClass->m_page_array_capacity + 1024;
        void * new_page_array = mem_alloc(theClass->m_alloc, gd_om_class_page_buf_len(new_page_array_capacity));
        if (new_page_array == NULL) {
            CPE_ERROR_EX(em, gd_om_no_memory, "alloc for buffer list fail!");
            return -1;
        }

        if (theClass->m_page_array_size > 0) {
            memcpy(new_page_array, theClass->m_page_array, gd_om_class_page_buf_len(theClass->m_page_array_size));
        }

        mem_free(theClass->m_alloc, theClass->m_page_array);
        theClass->m_page_array = (void **)new_page_array;
        theClass->m_page_array_capacity = new_page_array_capacity;
    }

    return 0;
}

static
int gd_om_class_add_new_page(
    struct gd_om_class * theClass,
    struct gd_om_data_page_head * head,
    void * page,
    error_monitor_t em)
{
    int32_t newRangeStart;
    cpe_ba_t alloc_arry;

    if (gd_om_class_reserve_page_array_slot(theClass, em) != 0) return -1;

    alloc_arry = (cpe_ba_t)(head + 1);

    head->m_magic = GD_OM_PAGE_MAGIC;
    head->m_classId = theClass->m_id;
    head->m_reserve = 0;
    head->m_page_idx = theClass->m_page_array_size;
    head->m_reserve2 = 0;
    cpe_ba_set_all(alloc_arry, theClass->m_alloc_buf_capacity, cpe_ba_false);

    newRangeStart = theClass->m_object_per_page * theClass->m_page_array_size;
    cpe_range_put_range(&theClass->m_range_alloc, newRangeStart, newRangeStart + theClass->m_object_per_page);

    theClass->m_page_array[theClass->m_page_array_size] = page;
    ++theClass->m_page_array_size;

    return 0;
}

static
int gd_om_class_add_old_page(
    struct gd_om_class * theClass,
    struct gd_om_data_page_head * head,
    void * page,
    error_monitor_t em)
{
    cpe_ba_t alloc_arry;

    if (gd_om_class_reserve_page_array_slot(theClass, em) != 0) return -1;

    alloc_arry = gd_om_class_ba_of_page(page);

    if (cpe_range_free_from_bitarray(
            &theClass->m_range_alloc,
            alloc_arry,
            theClass->m_object_per_page * theClass->m_page_array_size,
            theClass->m_object_per_page) != 0)
    {
        size_t i;
        CPE_ERROR_EX(em, gd_om_no_memory, "alloc page range no memory!");

        cpe_range_mgr_clear(&theClass->m_range_alloc);
        for(i = 0; i < theClass->m_page_array_size; ++i) {
            cpe_range_free_from_bitarray(
                &theClass->m_range_alloc,
                gd_om_class_ba_of_page(theClass->m_page_array[i]),
                theClass->m_object_per_page * i,
                theClass->m_object_per_page);
        }

        return -1;
    }

    assert(theClass->m_page_array_size + 1 < theClass->m_page_array_capacity);
    theClass->m_page_array[theClass->m_page_array_size] = page;
    ++theClass->m_page_array_size;

    return 0;
}

int gd_om_class_add_page(struct gd_om_class *cls, void * page, error_monitor_t em) {
    struct gd_om_data_page_head * head;

    head = (struct gd_om_data_page_head *)page;

    if (head->m_magic != GD_OM_PAGE_MAGIC) {
        CPE_ERROR_EX(em, gd_om_page_head_error, "page head magic error!");
        return -1;
    }

    if (head->m_classId == GD_OM_INVALID_CLASSID) {
        return gd_om_class_add_new_page(cls, head, page, em);
    }
    else {
        if (head->m_classId != cls->m_id) {
            CPE_ERROR_EX(
                em, gd_om_page_head_error,
                "page head class id error, expect %d, but %d!",
                cls->m_id, head->m_classId);
            return -1;
        }

        return gd_om_class_add_old_page(cls, head, page, em);
    }
}

int32_t
gd_om_class_alloc_object(struct gd_om_class * theClass) {
    int32_t r;

    assert(theClass);

    r = cpe_range_get_one(&theClass->m_range_alloc);
    if (r >= 0) {
        cpe_ba_t alloc_arry;
        int32_t pagePos;

        pagePos = (int32_t)(r / theClass->m_object_per_page);
        assert(pagePos >= 0 && (size_t)pagePos < theClass->m_page_array_size);

        alloc_arry = gd_om_class_ba_of_page(theClass->m_page_array[pagePos]);

        cpe_ba_set(alloc_arry, r % theClass->m_object_per_page, cpe_ba_true);
    }

    return r;
}

void gd_om_class_free_object(struct gd_om_class * theClass, int32_t oid, error_monitor_t em) {
    cpe_ba_t alloc_arry;
    int32_t pagePos;
    assert(theClass);

    pagePos = (int32_t)(oid / theClass->m_object_per_page);

    if(pagePos < 0 || (size_t)pagePos >= theClass->m_page_array_size) {
        CPE_ERROR_EX(
            em, gd_om_invalid_oid, "class %s: page pos %d overflow, page count is %zu!", 
            cpe_hs_data(theClass->m_name), pagePos, theClass->m_page_array_size);
        return;
    }

    if (cpe_range_put_one(&theClass->m_range_alloc, oid) != 0) {
        CPE_ERROR_EX(em, gd_om_no_memory, "no memory: free to range fail!")
        return;
    }

    alloc_arry = gd_om_class_ba_of_page(theClass->m_page_array[pagePos]);
    cpe_ba_set(alloc_arry, oid % theClass->m_object_per_page, cpe_ba_false);
}

void * gd_om_class_get_object(struct gd_om_class * theClass, int32_t oid, error_monitor_t em) {
    cpe_ba_t alloc_arry;
    int32_t pagePos;
    int32_t posInPage;
    char * page;
    assert(theClass);

    pagePos = (int32_t)(oid / theClass->m_object_per_page);

    if(pagePos < 0 || (size_t)pagePos >= theClass->m_page_array_size) {
        CPE_ERROR_EX(
            em, gd_om_invalid_oid, "class %s: page pos %d overflow, page count is %zu!", 
            cpe_hs_data(theClass->m_name), pagePos, theClass->m_page_array_size);
        return NULL;
    }

    posInPage = oid % theClass->m_object_per_page;
    page = (char*)theClass->m_page_array[pagePos];

    alloc_arry = gd_om_class_ba_of_page(page);
    if (cpe_ba_get(alloc_arry, posInPage) != cpe_ba_true) {
        CPE_ERROR_EX(
            em, gd_om_invalid_oid, "class %s: oid %d not alloced!", 
            cpe_hs_data(theClass->m_name), oid);
        return NULL;
    }

    return (void*)(page + theClass->m_object_buf_begin_in_page + (theClass->m_object_size * posInPage));
}

int32_t gd_om_class_addr_2_object(struct gd_om_class *cls, void * page, void * addr) {
    int pos_to_start;
    int idx_in_page;
    struct gd_om_data_page_head * head;

    head = (struct gd_om_data_page_head *)page;
    if (head->m_page_idx < 0
        || (size_t)head->m_page_idx >= cls->m_page_array_size
        || page != cls->m_page_array[head->m_page_idx])
        return -1;

    pos_to_start = (char*)addr - (((char *)page) + cls->m_object_buf_begin_in_page);

    if (pos_to_start % cls->m_object_size != 0) return -1;

    idx_in_page = pos_to_start / cls->m_object_size;

    return head->m_page_idx * cls->m_object_per_page + idx_in_page;
}

gd_om_class_id_t om_class_id(gd_om_class_t cls) {
    return cls->m_id;
}

const char * gd_om_class_name(gd_om_class_t cls) {
    return cpe_hs_data(cls->m_name);
}

cpe_hash_string_t gd_om_class_name_hs(gd_om_class_t cls) {
    return cls->m_name;
}

size_t gd_om_class_obj_size(gd_om_class_t cls) {
    return cls->m_object_size;
}
