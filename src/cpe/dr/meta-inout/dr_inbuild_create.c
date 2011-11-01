#include <stdlib.h>
#include "cpe/dr/dr_error.h"
#include "dr_inbuild.h"
#include "dr_metalib_ops.h"

static int dr_inbuild_calc_lib_paras(struct DRInBuildMetaLib * inBuildLib);

int dr_inbuild_build_lib(
    INOUT LPDRMETALIB *a_ppstLib,
    struct DRInBuildMetaLib * inBuildLib,
    void * userLogCtx,
    dr_inbuild_log_fun_t errorProcessor)
{
    struct DRInBuildString * stringEle = 0;
    struct DRInBuildMacro * macroEle = 0;
    struct DRInBuildMeta * metaEle = 0;
    int ret = 0;
    int stringUsedCount = 0;
    LPDRMETALIB metaLib = NULL;

    ret = dr_inbuild_calc_lib_paras(inBuildLib);
    if (CPE_IS_ERROR(ret)) {
        return ret;
    }

    metaLib = (LPDRMETALIB)malloc(inBuildLib->m_data.iSize);
    if (metaLib == NULL) {
        return -1;
    }

    ret = dr_init_lib(metaLib, &inBuildLib->m_data);
    if (CPE_IS_ERROR(ret)) {
        return ret;
    }

    /*build macros*/
    TAILQ_FOREACH(macroEle, &inBuildLib->m_macros, m_next) {
        macroEle->m_data.m_name_pos =
            dr_add_metalib_string(metaLib, macroEle->m_name, &stringUsedCount, userLogCtx, errorProcessor);
        macroEle->m_data.m_desc_pos =
            dr_add_metalib_string(metaLib, macroEle->m_desc, &stringUsedCount, userLogCtx, errorProcessor);

        dr_add_metalib_macro(metaLib, &macroEle->m_data, userLogCtx, errorProcessor);
    }

    /*build metas*/
    TAILQ_FOREACH(metaEle, &inBuildLib->m_metas, m_next) {
        LPDRMETA createdMeta = 0;
        struct DRInBuildMetaEntry * entryEle = 0;

        metaEle->m_data.m_name_pos =
            dr_add_metalib_string(metaLib, metaEle->m_name, &stringUsedCount, userLogCtx, errorProcessor);
        metaEle->m_data.m_desc_pos =
            dr_add_metalib_string(metaLib, metaEle->m_desc, &stringUsedCount, userLogCtx, errorProcessor);

        createdMeta = dr_add_metalib_meta(metaLib, &metaEle->m_data, userLogCtx, errorProcessor);

        /*build entries*/
        TAILQ_FOREACH(entryEle, &metaEle->m_entries, m_next) {
            entryEle->m_data.m_name_pos =
                dr_add_metalib_string(metaLib, entryEle->m_name, &stringUsedCount, userLogCtx, errorProcessor);
            entryEle->m_data.m_desc_pos =
                dr_add_metalib_string(metaLib, entryEle->m_desc, &stringUsedCount, userLogCtx, errorProcessor);
            entryEle->m_data.m_cname_pos =
                dr_add_metalib_string(metaLib, entryEle->m_cname, &stringUsedCount, userLogCtx, errorProcessor);


            if (entryEle->m_ref_type_name[0]) {
                //LPDRMETA refMeta = dr_get_meta_by_name(metaLib, 
            }

            dr_add_meta_entry(createdMeta, &entryEle->m_data, userLogCtx, errorProcessor);
        }
    }

    *a_ppstLib = metaLib;

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
    struct DRInBuildString * stringEle = 0;
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


