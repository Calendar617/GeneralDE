#include <stdlib.h>
#include <strings.h>
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_ctype_ops.h"
#include "dr_inbuild.h"
#include "dr_metalib_ops.h"

static int dr_inbuild_calc_lib_paras(struct DRInBuildMetaLib * inBuildLib);

int dr_inbuild_build_lib(
    INOUT LPDRMETALIB *a_ppstLib,
    struct DRInBuildMetaLib * inBuildLib,
    error_monitor_t em)
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
            dr_add_metalib_string(metaLib, macroEle->m_name, &stringUsedCount, em);
        macroEle->m_data.m_desc_pos =
            dr_add_metalib_string(metaLib, macroEle->m_desc, &stringUsedCount, em);

        dr_add_metalib_macro(metaLib, &macroEle->m_data, em);
    }

    /*build metas*/
    TAILQ_FOREACH(metaEle, &inBuildLib->m_metas, m_next) {
        LPDRMETA createdMeta = 0;
        struct DRInBuildMetaEntry * entryEle = 0;

        metaEle->m_data.m_name_pos =
            dr_add_metalib_string(metaLib, metaEle->m_name, &stringUsedCount, em);
        metaEle->m_data.m_desc_pos =
            dr_add_metalib_string(metaLib, metaEle->m_desc, &stringUsedCount, em);

        createdMeta = dr_add_metalib_meta(metaLib, &metaEle->m_data, em);

        /*build entries*/
        TAILQ_FOREACH(entryEle, &metaEle->m_entries, m_next) {
            const struct tagDRCTypeInfo * ctypeInfo = NULL;

            entryEle->m_data.m_name_pos =
                dr_add_metalib_string(metaLib, entryEle->m_name, &stringUsedCount, em);
            entryEle->m_data.m_desc_pos =
                dr_add_metalib_string(metaLib, entryEle->m_desc, &stringUsedCount, em);
            entryEle->m_data.m_cname_pos =
                dr_add_metalib_string(metaLib, entryEle->m_cname, &stringUsedCount, em);

            ctypeInfo = dr_find_ctype_info_by_name(entryEle->m_ref_type_name);
            if (ctypeInfo) {
                entryEle->m_data.m_type = ctypeInfo->m_id;
                entryEle->m_data.m_unitsize = ctypeInfo->m_size;
            }
            else {
                LPDRMETA refMeta = dr_lib_find_meta_by_name(metaLib, entryEle->m_ref_type_name);
                if (refMeta) {
                    entryEle->m_data.m_type = refMeta->m_type;
                    entryEle->m_data.m_unitsize = refMeta->m_data_size;
                    entryEle->m_data.m_ref_type_pos = (char*)refMeta - (char*)(metaLib + 1);
                }
                else {
                    CPE_ERROR_EX(em, CPE_DR_ERROR_ENTRY_NO_TYPE, "ref type \"%s\" is unknown!", entryEle->m_ref_type_name);
                    continue;
                }
            }

            dr_add_meta_entry(createdMeta, &entryEle->m_data, em);
        }

        dr_meta_complete(createdMeta, em);
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


