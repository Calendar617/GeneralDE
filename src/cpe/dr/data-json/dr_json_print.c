#include "yajl/yajl_gen.h"
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_stackbuf.h"
#include "cpe/utils/stream_mem.h"
#include "cpe/dr/dr_ctypes_op.h"
#include "cpe/dr/dr_json.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"
#include <assert.h>

static void dr_json_print_composite_type(
    yajl_gen g,
    LPDRMETA meta, LPDRMETAENTRY parentEntry, const void * data,
    error_monitor_t em, int level);

static const char * yajl_errno_to_string(yajl_gen_status s) {
    static const char * s_errorMsgs[] = {
        "keys_must_be_strings"
        , "max_depth_exceeded"
        , "in_error_state"
        , "generation_complete"
        , "invalid_number"
        , "no_buf"
        , "invalid_string" };

    if (s < 1/*yajl_gen_keys_must_be_strings*/ || s > yajl_gen_invalid_string) {
        return "invalid yajl error";
    }
    else {
        return s_errorMsgs[s - 1];
    }
}

#define JSON_PRINT_CHECK_GEN_RESULT(s)                          \
    { yajl_gen_status __s = (s);                                \
    if (__s != yajl_gen_status_ok) {                            \
    CPE_ERROR(em, "yajl error: %s", yajl_errno_to_string(__s)); \
    return;                                                     \
    }                                                           \
    }

#define JSON_PRINT_GEN_STRING(g, str)               \
    do {                                            \
        const char * __p = (str);                   \
        JSON_PRINT_CHECK_GEN_RESULT(                \
            yajl_gen_string(g, (const unsigned char *)__p, strlen(__p))); \
    } while(0)

static void dr_print_print_numeric(yajl_gen g, int typeId, const void * data, error_monitor_t em) {
    char buf[20 + 1];
    struct write_stream_mem bufS = CPE_WRITE_STREAM_MEM_INITIALIZER(buf, 20);
    int len = dr_ctype_print_to_stream((write_stream_t)&bufS, data, typeId, em);
    if (len > 0) {
        buf[len] = 0;
        yajl_gen_number(g, buf, len);
    }
    else {
        yajl_gen_null(g);
    }
}

static void dr_print_print_string(yajl_gen g, int typeId, size_t bufLen, const void * data, error_monitor_t em) {
    char buf[CPE_STACK_BUF_LEN(bufLen) + 1];
    struct write_stream_mem bufS = CPE_WRITE_STREAM_MEM_INITIALIZER(buf, CPE_STACK_BUF_LEN(bufLen) + 1);
    int len = dr_ctype_print_to_stream((write_stream_t)&bufS, data, typeId, em);

    if (len > 0) {
        buf[len] = 0;
        JSON_PRINT_CHECK_GEN_RESULT(yajl_gen_string(g, (const unsigned char *)buf, len));
    }
    else {
        yajl_gen_null(g);
    }
}

static void dr_print_print_basic_data(yajl_gen g, LPDRMETAENTRY entry, const void * data, error_monitor_t em) {
    switch(entry->m_type) {
    case CPE_DR_TYPE_INT8:
    case CPE_DR_TYPE_UINT8:
    case CPE_DR_TYPE_INT16:
    case CPE_DR_TYPE_UINT16:
    case CPE_DR_TYPE_INT32:
    case CPE_DR_TYPE_UINT32:
    case CPE_DR_TYPE_INT64:
    case CPE_DR_TYPE_UINT64:
        dr_print_print_numeric(g, entry->m_type, data, em);
        break;
    case CPE_DR_TYPE_CHAR:
    case CPE_DR_TYPE_UCHAR:
        dr_print_print_string(g, entry->m_type, 1, data, em);
        break;
    case CPE_DR_TYPE_STRING:
    case (CPE_DR_TYPE_STRING + 1):
        dr_print_print_string(g, entry->m_type, entry->m_unitsize, data, em);
        break;
    default:
        CPE_ERROR_EX(
            em, CPE_DR_ERROR_UNSUPPORTED_TYPE,
            "print basic data not supported type "FMT_DR_INT_T"!", entry->m_type);
        yajl_gen_null(g);
        break;
    }
}

static void dr_json_print_value(
    yajl_gen g,
    LPDRMETA meta,
    LPDRMETAENTRY entry,
    const void * data,
    error_monitor_t em,
    int level)
{
    if(entry->m_type <= CPE_DR_TYPE_COMPOSITE) {
        dr_json_print_composite_type(g, dr_entry_ref_meta(entry), entry, data, em, level + 1);
    }
    else {
        dr_print_print_basic_data(g, entry, data, em);
    }
}

static size_t dr_json_print_get_element_size(LPDRMETA meta, LPDRMETAENTRY entry, error_monitor_t em) {
    if (entry->m_type <= CPE_DR_TYPE_COMPOSITE) {
        LPDRMETA refMeta = dr_entry_ref_meta(entry);
        if (refMeta == NULL) {
            CPE_ERROR(
                em, "process %s.%s, ref meta not exist!",
                dr_meta_name(meta), dr_entry_name(entry));
            return 0;
        }

        return dr_meta_size(refMeta);
    }
    else {
        const struct tagDRCTypeInfo * typeInfo;
        typeInfo = dr_find_ctype_info_by_type(entry->m_type);
        if (typeInfo == NULL) {
            CPE_ERROR(
                em, "process %s.%s, type "FMT_DR_INT_T" is unknown!",
                dr_meta_name(meta), dr_entry_name(entry),
                entry->m_type);
            return 0;
        }

        if (typeInfo->m_size <= 0) {
            CPE_ERROR(
                em, "process %s.%s, type "FMT_DR_INT_T" size is invalid!",
                dr_meta_name(meta), dr_entry_name(entry),
                entry->m_type);
            return 0;
        }

        return typeInfo->m_size;
    }
}

static void dr_json_print_entry(
    yajl_gen g,
    LPDRMETA meta,
    LPDRMETAENTRY entry,
    const char * alldata,
    const void * data,
    error_monitor_t em,
    int level)
{
    JSON_PRINT_GEN_STRING(g, dr_entry_name(entry));

    if (entry->m_array_count == 1) {
        dr_json_print_value(g, meta, entry, data, em, level);
    }
    else {
        LPDRMETAENTRY referEntry;
        int32_t arrayCount;
        int32_t i;
        size_t elementSize;

        referEntry = dr_entry_array_refer_entry(entry);
        if (referEntry) {
            if (dr_entry_try_read_int32(
                    &arrayCount,
                    (const char *)alldata + entry->m_array_refer_data_start_pos,
                    referEntry,
                    em) != 0)
            {
                arrayCount = 0;
            }
        }
        else {
            arrayCount = entry->m_array_count;
        }

        yajl_gen_array_open(g);

        elementSize = dr_json_print_get_element_size(meta, entry, em);

        if (elementSize > 0) {
            for(i = 0; i < arrayCount; ++i) {
                dr_json_print_value(
                    g, meta, entry, (char *)data + elementSize * i, em, level);
            }
        }

        yajl_gen_array_close(g);
    }
}

static void dr_json_print_struct(yajl_gen g, LPDRMETA meta, const void * data, error_monitor_t em, int level) {
    int entryPos = 0;
    for(entryPos = 0; entryPos < meta->m_entry_count; ++entryPos) {
        LPDRMETAENTRY curEntry = dr_meta_entry_at(meta, entryPos);
        dr_json_print_entry(g, meta, curEntry, data, (const char *)data + curEntry->m_data_start_pos, em, level);
    }
}

static void dr_json_print_union(yajl_gen g, LPDRMETA meta, LPDRMETAENTRY parentEntry, const void * data, error_monitor_t em, int level) {
    LPDRMETAENTRY printEntry = NULL;

    int entryPos = 0;
    LPDRMETAENTRY selectEntry = dr_entry_select_entry(parentEntry);
    if (selectEntry) {
        int32_t selectValue;
        if (dr_ctype_try_read_int32(
                &selectValue,
                ((const char*)data) + parentEntry->m_select_data_start_pos,
                selectEntry->m_type,
                em) == 0)
        {
            for(; entryPos < meta->m_entry_count; ++entryPos) {
                LPDRMETAENTRY curEntry = dr_meta_entry_at(meta, entryPos);

                if (curEntry->m_select_range_min <= selectValue && curEntry->m_select_range_max >= selectValue) {
                    printEntry = curEntry;
                    break;
                }
            }
        }
    }
    else { /*have select entry*/
        for(; entryPos < meta->m_entry_count; ++entryPos) {
            LPDRMETAENTRY curEntry = dr_meta_entry_at(meta, entryPos);

            if (curEntry->m_select_range_min > curEntry->m_select_range_max) {
                printEntry = curEntry;
                break;
            }
        }
    }

    if (printEntry) {
        const void * curData = (const char *)data + printEntry->m_data_start_pos;
        dr_json_print_entry(g, meta, printEntry, data, curData, em, level);
    }
}

static void dr_json_print_composite_type(
    yajl_gen g,
    LPDRMETA meta, LPDRMETAENTRY parentEntry, const void * data,
    error_monitor_t em, int level)
{
    if (level >= CPE_DR_MAX_LEVEL) {
        CPE_ERROR_EX(em, CPE_DR_ERROR_TOO_COMPLIEX_META, "max level realched!");
        return;
    }

    yajl_gen_map_open(g);

    if (meta->m_type == CPE_DR_TYPE_STRUCT) {
        dr_json_print_struct(g, meta, data, em, level);
    }
    else if (meta->m_type == CPE_DR_TYPE_UNION) {
        dr_json_print_union(g, meta, parentEntry, data, em, level);
    }
    else {
        CPE_ERROR(em, "unknown complex type "FMT_DR_INT_T"!", meta->m_type);
    }

    yajl_gen_map_close(g);
}

static void dr_json_print_i(
    write_stream_t output,
    const void * input,
    LPDRMETA rootMeta,
    int flag,
    error_monitor_t em)
{
    yajl_gen g;

    if (output == NULL || input == NULL || rootMeta == NULL) {
        CPE_ERROR(em, "bad para");
        return;
    }

    g = yajl_gen_alloc(NULL);
    if (g == NULL) {
        CPE_ERROR_EX(em, CPE_DR_ERROR_NO_MEMORY, "alloc yajl_gen fail!");
        return;
    }

    yajl_gen_config(g, yajl_gen_beautify, flag & DR_JSON_PRINT_MINIMIZE ? 1 : 0);
    //yajl_gen_config(g, yajl_gen_validate_utf8, flag & DR_JSON_PRINT_VALIDATE_UTF8 ? 1 : 0);
    yajl_gen_config(g, yajl_gen_print_callback, stream_write, output);

    dr_json_print_composite_type(g, rootMeta, NULL, input, em, 0);

    yajl_gen_free(g);    
}

int dr_json_print(
    write_stream_t output,
    const void * input,
    LPDRMETA meta,
    int flag,
    error_monitor_t em)
{
    int ret = 0;

    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        dr_json_print_i(output, input, meta, flag, em);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        dr_json_print_i(output, input, meta, flag, &logError);
    }

    return ret;
}
