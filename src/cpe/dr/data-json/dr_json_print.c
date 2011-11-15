#include <string.h>
#include "yajl/yajl_gen.h"
#include "cpe/utils/stream_mem.h"
#include "cpe/dr/dr_json.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"

struct dr_json_print_stack_node {
    LPDRMETA m_meta;
    const void * m_data;
    int m_entryPos;
};

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

#define JSON_PRINT_GEN_STRING(str)                  \
    do {                                            \
        const char * __p = (str);                   \
        JSON_PRINT_CHECK_GEN_RESULT(                \
            yajl_gen_string(g, __p, strlen(__p)));  \
    } while(0)

static void dr_print_print_numeric(yajl_gen g, int typeId, const void * data, error_monitor_t em) {
    const struct tagDRCTypeInfo * typeInfo = dr_find_ctype_info_by_type(typeId);
    if (typeInfo == NULL || typeInfo->printf_to_stream == NULL) {
        CPE_ERROR(em, "unknown type %d!", typeInfo);
        yajl_gen_null(g);
    }
    else {
        char buf[20 + 1];
        struct write_stream_mem bufS = CPE_STREAM_MEM_INITIALIZER(buf, 20);
        int len = typeInfo->printf_to_stream((write_stream_t)&bufS, data);
        buf[len] = 0;
        yajl_gen_number(g, buf, len);
    }
}

static void dr_print_print_string(yajl_gen g, int typeId, size_t bufLen, const void * data, error_monitor_t em) {
    const struct tagDRCTypeInfo * typeInfo = dr_find_ctype_info_by_type(typeId);
    if (typeInfo == NULL || typeInfo->printf_to_stream == NULL) {
        CPE_ERROR(em, "unknown type %d!", typeInfo);
        yajl_gen_null(g);
    }
    else {
        char buf[bufLen + 1];
        struct write_stream_mem bufS = CPE_STREAM_MEM_INITIALIZER(buf, 20);
        int len = typeInfo->printf_to_stream((write_stream_t)&bufS, data);
        buf[len] = 0;
        yajl_gen_string(g, buf, len);
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

static void dr_json_print_i(
    write_stream_t output,
    const void * input,
    LPDRMETA rootMeta,
    int flag,
    error_monitor_t em)
{
    yajl_gen g;
    struct dr_json_print_stack_node metaStacks[CPE_DR_MAX_LEVEL];
    int metaPos = 0;

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
    yajl_gen_config(g, yajl_gen_validate_utf8, flag & DR_JSON_PRINT_VALIDATE_UTF8 ? 1 : 0);
    yajl_gen_config(g, yajl_gen_print_callback, stream_write, output);

    metaStacks[metaPos].m_meta = rootMeta;
    metaStacks[metaPos].m_data = input;
    metaStacks[metaPos].m_entryPos = 0;

    while(metaPos >= 0) {
        int curMetaPos = metaPos;
        LPDRMETA curMeta = metaStacks[metaPos].m_meta;
        const void * curData = metaStacks[metaPos].m_data;
        int curEntryPos = metaStacks[metaPos].m_entryPos;

        if (curEntryPos == 0) {
            JSON_PRINT_CHECK_GEN_RESULT(yajl_gen_map_open(g));
        }

        for(; curMetaPos == metaPos && curEntryPos < curMeta->m_entry_count; ++curEntryPos) {
            LPDRMETAENTRY curEntry = dr_meta_entry_at(curMeta, curEntryPos);
            const void * curData = metaStacks[metaPos].m_data + curEntry->m_data_start_pos;

            JSON_PRINT_GEN_STRING(dr_entry_name(curEntry));

            if(curEntry->m_type <= CPE_DR_TYPE_COMPOSITE) {
                metaStacks[curMetaPos].m_entryPos = metaPos + 1;
                ++curMetaPos;
                if (curMetaPos >= CPE_DR_MAX_LEVEL) {
                    CPE_ERROR_EX(em, CPE_DR_ERROR_TOO_COMPLIEX_META, "max level realched!");
                    return;
                }

                metaStacks[curMetaPos].m_meta = dr_entry_ref_meta(curEntry);
                if (metaStacks[curMetaPos].m_meta == NULL) {
                    CPE_ERROR(em, "ref meta not exist!");
                    return;
                }
                metaStacks[curMetaPos].m_data = curData;
                metaStacks[curMetaPos].m_entryPos = 0;
            }
            else {
                dr_print_print_basic_data(g, curEntry->m_type, curData, em);
            }
        }

        if (curMetaPos != metaPos) {
            metaPos = curMetaPos;
        }
        else {
            if (curEntryPos == curMeta->m_entry_count) {
                JSON_PRINT_CHECK_GEN_RESULT(yajl_gen_map_close(g));
            }

            --metaPos;
        }
    }

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
