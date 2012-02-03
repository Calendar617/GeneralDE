#include <stdlib.h>
#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_strings.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_types.h"
#include "../dr_ctype_ops.h"
#include "dr_inbuild.h"
#include "dr_metalib_ops.h"

static int dr_inbuild_calc_lib_paras(
    struct DRInBuildMetaLib * inBuildLib, error_monitor_t em);

struct DRInBuildCreateCtx {
    LPDRMETALIB m_metaLib;
    error_monitor_t m_em;
    int m_stringUsedCount;
};

static int dr_inbuild_build_add_string(struct DRInBuildCreateCtx * ctx, const char * data) {
    size_t dataSize;
    void * p;

    if (data == NULL || data[0] == 0) {
        return -1;
    }

    dataSize = strlen(data) + 1;

    p = dr_lib_alloc_in_strbuf(ctx->m_metaLib, dataSize, &ctx->m_stringUsedCount, ctx->m_em);
    if (p == NULL) {
        return -1;
    }

    strncpy((char*)p, data, dataSize);

    return dr_lib_addr_to_pos(ctx->m_metaLib, p);
}

static void dr_inbuild_build_add_macro(
    struct DRInBuildCreateCtx * ctx,
    struct DRInBuildMacro * macroEle)
{
    macroEle->m_data.m_name_pos = dr_inbuild_build_add_string(ctx, macroEle->m_name);
    macroEle->m_data.m_desc_pos = dr_inbuild_build_add_string(ctx, macroEle->m_desc);
    dr_lib_add_macro(ctx->m_metaLib, &macroEle->m_data, ctx->m_em);
}

static void dr_inbuild_build_calc_entry_defaultvalue(
    struct DRInBuildCreateCtx * ctx, LPDRMETA createdMeta, struct DRInBuildMetaEntry * entryEle)
{
    void * p;

    if (entryEle->m_dft_value == NULL || entryEle->m_data.m_unitsize <= 0) {
        return;
    }

    p = dr_lib_alloc_in_strbuf(ctx->m_metaLib, entryEle->m_data.m_unitsize, &ctx->m_stringUsedCount, ctx->m_em);
    if (p == NULL) {
        return;
    }

    if (dr_entry_set_from_string(p, entryEle->m_dft_value, &entryEle->m_data, ctx->m_em) != 0) {
        return;
    }

    entryEle->m_data.m_dft_value_pos = dr_lib_addr_to_pos(ctx->m_metaLib, p);
}

static void dr_inbuild_build_calc_entry_composite_type(
    struct DRInBuildCreateCtx * ctx, LPDRMETA createdMeta, struct DRInBuildMetaEntry * entryEle)
{
    LPDRMETA refMeta;
    if (entryEle->m_data.m_type != CPE_DR_TYPE_UNKOWN) {
        return;
    }

    refMeta = dr_lib_find_meta_by_name(ctx->m_metaLib, entryEle->m_ref_type_name);
    if (refMeta) {
        char * base = (char*)(createdMeta) - createdMeta->m_self_pos;
        entryEle->m_data.m_type = refMeta->m_type;
        entryEle->m_data.m_ref_type_pos = (char*)refMeta - (char*)base;
    }
    else {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_NO_TYPE, "ref type \"%s\" is unknown!", entryEle->m_ref_type_name);
        entryEle->m_ignore = 1;
    }
}

static void dr_inbuild_build_calc_entry_select(
    struct DRInBuildCreateCtx * ctx, LPDRMETA createdMeta, struct DRInBuildMetaEntry * entryEle)
{
    LPDRMETAENTRY selectEntry;

    if (entryEle->m_selector_path == NULL) {
        return;
    }

    selectEntry = dr_meta_find_entry_by_path(createdMeta, entryEle->m_selector_path);
    if (selectEntry == NULL) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_INVALID_SELECT_VALUE, "entry at %s not exist!", entryEle->m_selector_path);
    }
    else {
        char * base = (char*)(createdMeta) - createdMeta->m_self_pos;
        entryEle->m_data.m_select_entry_pos = (int32_t)((char *)selectEntry - base);
        entryEle->m_data.m_select_data_start_pos = dr_meta_path_to_off(createdMeta, entryEle->m_selector_path, NULL);
    }
}

static void dr_inbuild_build_calc_array_refer(
    struct DRInBuildCreateCtx * ctx, LPDRMETA createdMeta, struct DRInBuildMetaEntry * entryEle)
{
    LPDRMETAENTRY referEntry;

    if (entryEle->m_refer_path == NULL) {
        if (entryEle->m_data.m_array_count == 0) {
            CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_VARIABLE_ARRAY_NO_REFER, "entry is dynamic array, but no reffer!");
        }
        return;
    }

    if (entryEle->m_data.m_array_count == 1) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_INVALID_REFER_VALUE, "entry is not array, but config a reffer!");
        return;
    }

    referEntry = dr_meta_find_entry_by_path(createdMeta, entryEle->m_refer_path);
    if (referEntry == NULL) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_INVALID_REFER_VALUE, "entry at %s not exist!", entryEle->m_refer_path);
    }
    else {
        char * base = (char*)(createdMeta) - createdMeta->m_self_pos;
        entryEle->m_data.m_array_refer_entry_pos = (int32_t)((char *)referEntry - base);
        entryEle->m_data.m_array_refer_data_start_pos = dr_meta_path_to_off(createdMeta, entryEle->m_refer_path, NULL);
    }
}

static void dr_inbuild_build_add_entry(
    struct DRInBuildCreateCtx * ctx,
    LPDRMETA createdMeta,
    struct DRInBuildMetaEntry * entryEle)
{
    entryEle->m_data.m_name_pos = dr_inbuild_build_add_string(ctx, entryEle->m_name);
    entryEle->m_data.m_desc_pos = dr_inbuild_build_add_string(ctx, entryEle->m_desc);
    entryEle->m_data.m_cname_pos =dr_inbuild_build_add_string(ctx, entryEle->m_cname);

    dr_inbuild_build_calc_entry_defaultvalue(ctx, createdMeta, entryEle);
    dr_inbuild_build_calc_entry_composite_type(ctx, createdMeta, entryEle) ;
    dr_inbuild_build_calc_entry_select(ctx, createdMeta, entryEle);
    dr_inbuild_build_calc_array_refer(ctx, createdMeta, entryEle);

    if (!entryEle->m_ignore) {
        dr_meta_add_entry(createdMeta, &entryEle->m_data, ctx->m_em);
    }
}

static void dr_inbuild_build_add_meta(
    struct DRInBuildCreateCtx * ctx,
    struct DRInBuildMeta * metaEle)
{
    LPDRMETA createdMeta = 0;
    struct DRInBuildMetaEntry * entryEle = 0;

    metaEle->m_data.m_name_pos = dr_inbuild_build_add_string(ctx, metaEle->m_name);
    metaEle->m_data.m_desc_pos =dr_inbuild_build_add_string(ctx, metaEle->m_desc);

    createdMeta = dr_lib_add_meta(ctx->m_metaLib, &metaEle->m_data, ctx->m_em);

    /*build entries*/
    TAILQ_FOREACH(entryEle, &metaEle->m_entries, m_next) {
        dr_inbuild_build_add_entry(ctx, createdMeta, entryEle);
    }

    dr_meta_do_complete(createdMeta, ctx->m_em);
}

int dr_inbuild_build_lib(
    mem_buffer_t buffer,
    struct DRInBuildMetaLib * inBuildLib,
    error_monitor_t em)
{
    struct DRInBuildCreateCtx ctx;
    struct DRInBuildMacro * macroEle = 0;
    struct DRInBuildMeta * metaEle = 0;
    int ret = 0;

    ctx.m_stringUsedCount = 0;
    ctx.m_metaLib = NULL;
    ctx.m_em = em;

    ret = dr_inbuild_calc_lib_paras(inBuildLib, em);
    if (CPE_IS_ERROR(ret)) {
        return ret;
    }

    ctx.m_metaLib = (LPDRMETALIB)mem_buffer_alloc(buffer, inBuildLib->m_data.iSize);
    if (ctx.m_metaLib == NULL) {
        return -1;
    }

    ret = dr_lib_init(ctx.m_metaLib, &inBuildLib->m_data);
    if (CPE_IS_ERROR(ret)) {
        return ret;
    }

    /*build macros*/
    TAILQ_FOREACH(macroEle, &inBuildLib->m_macros, m_next) {
        dr_inbuild_build_add_macro(&ctx, macroEle);
    }

    /*build metas*/
    TAILQ_FOREACH(metaEle, &inBuildLib->m_metas, m_next) {
        dr_inbuild_build_add_meta(&ctx, metaEle);
    }

    return CPE_SUCCESS;
}

static int dr_inbuild_build_calc_defaultvalue_size(
    struct DRInBuildMetaLib * inBuildLib, error_monitor_t em)
{
    struct DRInBuildMeta * metaEle = 0;

    int totalSize = 0;

    TAILQ_FOREACH(metaEle, &inBuildLib->m_metas, m_next) {
        struct DRInBuildMetaEntry * entryEle = 0;
        TAILQ_FOREACH(entryEle, &metaEle->m_entries, m_next) {
            if (entryEle->m_ignore) {
                continue;
            }

            if (entryEle->m_dft_value) {
                if (entryEle->m_data.m_unitsize <= 0) {
                    entryEle->m_dft_value = NULL;
                    CPE_WARNING(em,
                        "ignore default value of %s.%s!", metaEle->m_name, entryEle->m_name);
                }
                else {
                    totalSize += entryEle->m_data.m_unitsize;
                }
            }
        }
    }

    return totalSize;
}

static void dr_inbuild_build_calc_basic_type_and_size(struct DRInBuildMetaLib * inBuildLib, error_monitor_t em) {
    struct DRInBuildMeta * metaEle = 0;

    TAILQ_FOREACH(metaEle, &inBuildLib->m_metas, m_next) {
        struct DRInBuildMetaEntry * entryEle = 0;

        TAILQ_FOREACH(entryEle, &metaEle->m_entries, m_next) {
            const struct tagDRCTypeInfo * ctypeInfo = NULL;
            ctypeInfo = dr_find_ctype_info_by_name(entryEle->m_ref_type_name);
            if (ctypeInfo) {
                entryEle->m_data.m_type = ctypeInfo->m_id;
                if ((int)ctypeInfo->m_size > 0) {
                    entryEle->m_data.m_unitsize =
                        ctypeInfo->m_size * ( entryEle->m_data.m_array_count < 1 ? 1 : entryEle->m_data.m_array_count);
                }
                else if (entryEle->m_data.m_unitsize <= 0) {
                    CPE_ERROR_EX(
                        em, CPE_DR_ERROR_ENTRY_INVALID_SIZE_VALUE,
                        "%s.%s: no size of  type \"%s\" configured!",
                        metaEle->m_name, entryEle->m_name, entryEle->m_ref_type_name);
                    entryEle->m_ignore = 1;
                }
            }
            else {
                assert(entryEle->m_data.m_type == CPE_DR_TYPE_UNKOWN);
            }
        }
    }
}

static int dr_inbuild_calc_string_size(const char * data) {
    return data[0] == 0
        ? 0
        : strlen(data) + 1;
}

static int dr_inbuild_calc_strbuf_size(struct DRInBuildMetaLib * inBuildLib) {
    struct DRInBuildMacro * macroEle = 0;
    struct DRInBuildMeta * metaEle = 0;
    struct DRInBuildMetaEntry * entryEle = 0;

    int strBufSize = 0;

    TAILQ_FOREACH(macroEle, &inBuildLib->m_macros, m_next) {
        strBufSize += dr_inbuild_calc_string_size(macroEle->m_name);
        strBufSize += dr_inbuild_calc_string_size(macroEle->m_desc);
    }

    TAILQ_FOREACH(metaEle, &inBuildLib->m_metas, m_next) {
        strBufSize += dr_inbuild_calc_string_size(metaEle->m_name);
        strBufSize += dr_inbuild_calc_string_size(metaEle->m_desc);

        TAILQ_FOREACH(entryEle, &metaEle->m_entries, m_next) {
            strBufSize += dr_inbuild_calc_string_size(entryEle->m_name);
            strBufSize += dr_inbuild_calc_string_size(entryEle->m_desc);
            strBufSize += dr_inbuild_calc_string_size(entryEle->m_cname);
        }
    }

    return strBufSize;
}

int dr_inbuild_calc_lib_paras(
    struct DRInBuildMetaLib * inBuildLib, error_monitor_t em)
{
    struct DRInBuildMacro * macroEle = 0;
    struct DRInBuildMeta * metaEle = 0;

    dr_inbuild_build_calc_basic_type_and_size(inBuildLib, em);

    inBuildLib->m_data.iMaxMacros = 0;
    TAILQ_FOREACH(macroEle, &inBuildLib->m_macros, m_next) {
        ++inBuildLib->m_data.iMaxMacros;
    }

    inBuildLib->m_data.iMetaSize = 0;
    inBuildLib->m_data.iMaxMetas = 0;
    TAILQ_FOREACH(metaEle, &inBuildLib->m_metas, m_next) {
        metaEle->m_data.m_entry_count = metaEle->m_entries_count;

        inBuildLib->m_data.iMaxMetas++;
        inBuildLib->m_data.iMetaSize += dr_calc_meta_use_size(metaEle->m_entries_count);
    }

    inBuildLib->m_data.iMaxMacrosGroupNum = 0;
    inBuildLib->m_data.iMacrosGroupSize = 0;

    inBuildLib->m_data.iStrBufSize =
        dr_inbuild_calc_strbuf_size(inBuildLib)
        + dr_inbuild_build_calc_defaultvalue_size(inBuildLib, em);

    inBuildLib->m_data.iSize
        = sizeof(struct tagDRMetaLib)                                    /*head*/
        + sizeof(struct tagDRMacro) * inBuildLib->m_data.iMaxMacros /*macros*/
        + (sizeof(struct tagDRMetaIdxById)                               /*meta indexes*/
           + sizeof(struct tagDRMetaIdxByName)
           + sizeof(struct tagDRMetaIdxByName/*TODO: unknown*/)
            ) * inBuildLib->m_data.iMaxMetas
        + inBuildLib->m_data.iMetaSize                              /*metas*/
        + inBuildLib->m_data.iMacrosGroupSize                       /*macro group*/
        + inBuildLib->m_data.iStrBufSize                            /*strings*/
        ;

    return CPE_SUCCESS;
}


