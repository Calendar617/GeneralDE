#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_ctype_ops.h"
#include "dr_inbuild.h"
#include "dr_metalib_ops.h"

static int dr_inbuild_calc_lib_paras(struct DRInBuildMetaLib * inBuildLib);

struct DRInBuildCreateCtx {
    LPDRMETALIB m_metaLib;
    error_monitor_t m_em;
    int m_stringUsedCount;
};

static int dr_inbuild_build_add_string(struct DRInBuildCreateCtx * ctx, const char * str) {
    return dr_add_metalib_string(ctx->m_metaLib, str, &ctx->m_stringUsedCount, ctx->m_em);
}

static void dr_inbuild_build_add_macro(
    struct DRInBuildCreateCtx * ctx,
    struct DRInBuildMacro * macroEle)
{
    macroEle->m_data.m_name_pos = dr_inbuild_build_add_string(ctx, macroEle->m_name);
    macroEle->m_data.m_desc_pos = dr_inbuild_build_add_string(ctx, macroEle->m_desc);
    dr_add_metalib_macro(ctx->m_metaLib, &macroEle->m_data, ctx->m_em);
}

static int dr_inbuild_build_calc_entry_type(
    struct DRInBuildCreateCtx * ctx,
    LPDRMETA createdMeta,
    struct DRInBuildMetaEntry * entryEle)
{
    char * base = (char*)(createdMeta) - createdMeta->m_self_pos;
    const struct tagDRCTypeInfo * ctypeInfo = NULL;
    ctypeInfo = dr_find_ctype_info_by_name(entryEle->m_ref_type_name);
    if (ctypeInfo) {
        entryEle->m_data.m_type = ctypeInfo->m_id;
        if ((int)ctypeInfo->m_size > 0) {
            entryEle->m_data.m_unitsize = ctypeInfo->m_size;
        }
        else if (entryEle->m_data.m_unitsize <= 0) {
            CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_INVALID_SIZE_VALUE, "no size of  type \"%s\" configured!", entryEle->m_ref_type_name);
            return -1;
        }
    }
    else {
        LPDRMETA refMeta = dr_lib_find_meta_by_name(ctx->m_metaLib, entryEle->m_ref_type_name);
        if (refMeta) {
            entryEle->m_data.m_type = refMeta->m_type;
            entryEle->m_data.m_unitsize = refMeta->m_data_size;
            entryEle->m_data.m_ref_type_pos = (char*)refMeta - (char*)base;
        }
        else {
            CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_NO_TYPE, "ref type \"%s\" is unknown!", entryEle->m_ref_type_name);
            return -1;
        }
    }

    return 0;
}

static void dr_inbuild_build_calc_entry_select(
    struct DRInBuildCreateCtx * ctx, LPDRMETA createdMeta, struct DRInBuildMetaEntry * entryEle)
{
    if (entryEle->m_selector_path == NULL) {
        return;
    }

    LPDRMETAENTRY selectEntry = dr_meta_find_entry_by_path(createdMeta, entryEle->m_selector_path);
    if (selectEntry == NULL) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_INVALID_SELECT_VALUE, "entry at %s not exist!", entryEle->m_selector_path);
    }
    else {
        char * base = (char*)(createdMeta) - createdMeta->m_self_pos;
        entryEle->m_data.m_select_entry_pos = (int32_t)((char *)selectEntry - base);
        entryEle->m_data.m_select_data_start_pos = dr_meta_path_to_off(createdMeta, entryEle->m_selector_path);
    }
}

static void dr_inbuild_build_add_entry(
    struct DRInBuildCreateCtx * ctx,
    LPDRMETA createdMeta,
    struct DRInBuildMetaEntry * entryEle)
{
    int ignoreEntry = 0;

    entryEle->m_data.m_name_pos = dr_inbuild_build_add_string(ctx, entryEle->m_name);
    entryEle->m_data.m_desc_pos = dr_inbuild_build_add_string(ctx, entryEle->m_desc);
    entryEle->m_data.m_cname_pos =dr_inbuild_build_add_string(ctx, entryEle->m_cname);

    if (dr_inbuild_build_calc_entry_type(ctx, createdMeta, entryEle) != 0) {
        ignoreEntry = 1;
    }

    dr_inbuild_build_calc_entry_select(ctx, createdMeta, entryEle);

    if (!ignoreEntry) {
        dr_add_meta_entry(createdMeta, &entryEle->m_data, ctx->m_em);
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

    createdMeta = dr_add_metalib_meta(ctx->m_metaLib, &metaEle->m_data, ctx->m_em);

    /*build entries*/
    TAILQ_FOREACH(entryEle, &metaEle->m_entries, m_next) {
        dr_inbuild_build_add_entry(ctx, createdMeta, entryEle);
    }

    dr_meta_complete(createdMeta, ctx->m_em);
}

int dr_inbuild_build_lib(
    LPDRMETALIB *a_ppstLib,
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

    ret = dr_inbuild_calc_lib_paras(inBuildLib);
    if (CPE_IS_ERROR(ret)) {
        return ret;
    }

    ctx.m_metaLib = (LPDRMETALIB)malloc(inBuildLib->m_data.iSize);
    if (ctx.m_metaLib == NULL) {
        return -1;
    }

    ret = dr_init_lib(ctx.m_metaLib, &inBuildLib->m_data);
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

    *a_ppstLib = ctx.m_metaLib;

    return CPE_SUCCESS;
}

static int calc_str_buf_size(const char * data) {
    if (data[0] == 0) {
        return 0;
    }
    else {
        return strlen(data) + 1;
    }
}

int dr_inbuild_calc_lib_paras(struct DRInBuildMetaLib * inBuildLib) {
    struct DRInBuildMacro * macroEle = 0;
    struct DRInBuildMeta * metaEle = 0;
    struct DRInBuildMetaEntry * entryEle = 0;

    int strBufSize = 0;

    inBuildLib->m_data.iMaxMacros = 0;
    TAILQ_FOREACH(macroEle, &inBuildLib->m_macros, m_next) {
        strBufSize += calc_str_buf_size(macroEle->m_name);
        strBufSize += calc_str_buf_size(macroEle->m_desc);

        ++inBuildLib->m_data.iMaxMacros;
    }

    inBuildLib->m_data.iMetaSize = 0;
    inBuildLib->m_data.iMaxMetas = 0;
    TAILQ_FOREACH(metaEle, &inBuildLib->m_metas, m_next) {
        strBufSize += calc_str_buf_size(metaEle->m_name);
        strBufSize += calc_str_buf_size(metaEle->m_desc);

        metaEle->m_data.m_entry_count = metaEle->m_entries_count;

        inBuildLib->m_data.iMaxMetas++;
        inBuildLib->m_data.iMetaSize += dr_calc_meta_use_size(metaEle->m_entries_count);

        TAILQ_FOREACH(entryEle, &metaEle->m_entries, m_next) {
            strBufSize += calc_str_buf_size(entryEle->m_name);
            strBufSize += calc_str_buf_size(entryEle->m_desc);
            strBufSize += calc_str_buf_size(entryEle->m_cname);
        }
    }

    inBuildLib->m_data.iMaxMacrosGroupNum = 0;
    inBuildLib->m_data.iMacrosGroupSize = 0;

    inBuildLib->m_data.iSize
        = sizeof(struct tagDRMetaLib)                                    /*head*/
        + sizeof(struct tagDRMacro) * inBuildLib->m_data.iMaxMacros /*macros*/
        + (sizeof(struct tagDRMetaIdxById)                               /*meta indexes*/
           + sizeof(struct tagDRMetaIdxByName)
           + sizeof(struct tagDRMetaIdxByName/*TODO: unknown*/)
            ) * inBuildLib->m_data.iMaxMetas
        + inBuildLib->m_data.iMetaSize                              /*metas*/
        + inBuildLib->m_data.iMacrosGroupSize                       /*macro group*/
        + strBufSize                                                     /*strings*/
        ;

    inBuildLib->m_data.iStrBufSize += strBufSize;

    return CPE_SUCCESS;
}


