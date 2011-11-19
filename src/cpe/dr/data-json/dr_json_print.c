#include <string.h>
#include "yajl/yajl_gen.h"
#include "cpe/utils/stream_mem.h"
#include "cpe/dr/dr_ctypes_op.h"
#include "cpe/dr/dr_json.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"

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
    struct write_stream_mem bufS = CPE_STREAM_MEM_INITIALIZER(buf, 20);
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
    char buf[bufLen + 1];
    struct write_stream_mem bufS = CPE_STREAM_MEM_INITIALIZER(buf, bufLen + 1);
    int len = dr_ctype_print_to_stream((write_stream_t)&bufS, data, typeId, em);
    if (len > 0) {
        buf[len] = 0;
        JSON_PRINT_CHECK_GEN_RESULT(yajl_gen_string(g, (const unsigned char *)buf, len));
    }
    else {
        yajl_gen_null(g);
    }
}

static void dr_print_print_basic_data(yajl_gen g, int typeId, const void * data, error_monitor_t em) {
    switch(typeId) {
    case CPE_DR_TYPE_INT8:
    case CPE_DR_TYPE_UINT8:
    case CPE_DR_TYPE_INT16:
    case CPE_DR_TYPE_UINT16:
    case CPE_DR_TYPE_INT32:
    case CPE_DR_TYPE_UINT32:
    case CPE_DR_TYPE_INT64:
    case CPE_DR_TYPE_UINT64:
        dr_print_print_numeric(g, typeId, data, em);
        break;
    case CPE_DR_TYPE_CHAR:
    case CPE_DR_TYPE_UCHAR:
        dr_print_print_string(g, typeId, 1, data, em);
        break;
    default:
        CPE_ERROR_EX(em, CPE_DR_ERROR_UNSUPPORTED_TYPE, "not supported type %d!", typeId);
        yajl_gen_null(g);
        break;
    }
}

static void dr_json_print_entry(
    yajl_gen g,
    LPDRMETA meta,
    LPDRMETAENTRY entry,
    const void * data,
    error_monitor_t em,
    int level)
{
    JSON_PRINT_GEN_STRING(g, dr_entry_name(entry));

    if(entry->m_type <= CPE_DR_TYPE_COMPOSITE) {
        dr_json_print_composite_type(g, dr_entry_ref_meta(entry), entry, data, em, level + 1);
    }
    else {
        dr_print_print_basic_data(g, entry->m_type, data, em);
    }
}

static void dr_json_print_struct(yajl_gen g, LPDRMETA meta, const void * data, error_monitor_t em, int level) {
    int entryPos = 0;
    for(entryPos = 0; entryPos < meta->m_entry_count; ++entryPos) {
        LPDRMETAENTRY curEntry = dr_meta_entry_at(meta, entryPos);
        const void * curData = data + curEntry->m_data_start_pos;
        dr_json_print_entry(g, meta, curEntry, curData, em, level);
    }
}

static void dr_json_print_union(yajl_gen g, LPDRMETA meta, LPDRMETAENTRY parentEntry, const void * data, error_monitor_t em, int level) {
    LPDRMETAENTRY printEntry = NULL;

    int entryPos = 0;
    LPDRMETAENTRY selectEntry = dr_entry_select_entry(parentEntry);
    if (selectEntry) {
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
        const void * curData = data + printEntry->m_data_start_pos;
        dr_json_print_entry(g, meta, printEntry, curData, em, level);
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
        CPE_ERROR(em, "unknown complex type %d!", meta->m_type);
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
