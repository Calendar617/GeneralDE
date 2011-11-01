#include <assert.h>
#include <stdio.h>
#include "dr_metalib_ops.h"
#include "dr_inbuild_error.h"

int dr_init_lib(OUT LPDRMETALIB pstLib, const LPDRLIBPARAM pstParam) {
    bzero(pstLib, pstParam->iSize);

    pstLib->m_magic = CPE_DR_MAGIC;
    pstLib->m_build_version = 11;
    //int8_t reserve1[4];
    pstLib->m_size = pstParam->iSize;
    //int8_t reserve2[16];
    pstLib->m_id = pstParam->iID;
    pstLib->m_tag_set_version = pstParam->iTagSetVersion;
    //int8_t reserve3[4];
    pstLib->m_meta_max_count = pstParam->iMaxMetas;
    pstLib->m_meta_count = 0;
    pstLib->m_macro_max_count = pstParam->iMaxMacros;
    pstLib->m_macro_count = 0;
    pstLib->m_macrogroup_max_count = pstParam->iMaxMacrosGroupNum;
    pstLib->m_macrogroup_count = 0;
    //int8_t reserve4[8];
    pstLib->m_version = pstParam->iVersion;
    pstLib->m_startpos_macro = 0; /*guess*/
    pstLib->m_startpos_meta_by_id = pstLib->m_startpos_macro + sizeof(struct tagDRMacro) * pstParam->iMaxMacros;
    pstLib->m_startpos_meta_by_name = pstLib->m_startpos_meta_by_id + sizeof(struct tagDRMetaIdxById) * pstParam->iMaxMetas;
    pstLib->m_startpos_meta_by_unknown = pstLib->m_startpos_meta_by_name + sizeof(struct tagDRMetaIdxByName) * pstParam->iMaxMetas;
    pstLib->m_startpos_meta = pstLib->m_startpos_meta_by_unknown + sizeof(struct tagDRMetaIdxByName /*TODO: change type*/) * pstParam->iMaxMetas;
    pstLib->m_startpos_str = pstLib->m_startpos_meta + pstParam->iMetaSize;
    pstLib->m_buf_size_str = pstParam->iStrBufSize;
    //int8_t reserve7[8];
    pstLib->m_buf_size_meta = pstParam->iMetaSize;
    //int8_t reserve8[4];
    pstLib->m_buf_size_macros = sizeof(struct tagDRMacro) * pstParam->iMaxMacros;
    //int8_t reserve9[24];
    strncpy(pstLib->m_name, pstParam->szName, CPE_DR_NAME_LEN);

    return 0;
}

int dr_add_metalib_string(
    LPDRMETALIB metaLib, const char * data, int * usedCount,
    void * userData, dr_inbuild_log_fun_t errorProcessor)
{
    char * base;
    int startPos;
    int dataSize;

    if (data == NULL || data[0] == 0) {
        return -1;
    }

    dataSize = strlen(data) + 1;
    if ((*usedCount + dataSize) > metaLib->m_buf_size_str) { /*overflow*/
        DR_NOTIFY_ERROR(userData, errorProcessor, CPE_DR_ERROR_NO_SPACE_FOR_STRBUF);
        return -1;
    }

    startPos = metaLib->m_startpos_str + *usedCount;
    base = (char *)(metaLib + 1);

    strncpy(base + startPos, data, dataSize);

    *usedCount += dataSize;

    return startPos;
}

LPDRMACRO dr_add_metalib_macro(LPDRMETALIB metaLib, LPDRMACRO macro, void * userData, dr_inbuild_log_fun_t errorProcessor) {
    char * base = (char*)(metaLib + 1);
    LPDRMACRO newMacro = NULL;

    if (metaLib->m_macro_count >= metaLib->m_macro_max_count) {
        return NULL;
    }

    newMacro = ((LPDRMACRO)(base + metaLib->m_startpos_macro)) + metaLib->m_macro_count;
    memcpy(newMacro, macro, sizeof(*macro));
    ++metaLib->m_macro_count;
    return newMacro;
}

static int dr_add_metalib_meta_find_next_pos(LPDRMETALIB metaLib) {
    char * base = (char*)(metaLib + 1);
    int i = 0;

    struct tagDRMetaIdxById * metaIdx = (struct tagDRMetaIdxById *)(base + metaLib->m_startpos_meta_by_id);

    int newMetaPos = metaLib->m_startpos_meta;

    for(i = 0; i < metaLib->m_meta_count; ++i) {
        int curNextMetaPos = metaIdx[i].m_diff_to_base
            + dr_calc_meta_use_size(((LPDRMETA)(base + metaIdx[i].m_diff_to_base))->m_entry_count);
        if (curNextMetaPos > newMetaPos) {
            newMetaPos = curNextMetaPos;
        }
    }

    return newMetaPos;
}

static void dr_add_metalib_meta_add_index_by_name(
    LPDRMETALIB metaLib, LPDRMETA newMeta, void * userData, dr_inbuild_log_fun_t errorProcessor)
{
    char * base = (char*)(metaLib + 1);
    int beginPos, endPos, curPos;
    struct tagDRMetaIdxByName * putAt = NULL;
    struct tagDRMetaIdxByName * searchStart =
        (struct tagDRMetaIdxByName *)(base + metaLib->m_startpos_meta_by_name);

    for(beginPos = 0, endPos = metaLib->m_meta_count, curPos = (endPos - beginPos - 1) / 2;
        beginPos < endPos;
        curPos = beginPos + (endPos - beginPos - 1) / 2)
    {
        struct tagDRMetaIdxByName * curItem = searchStart + curPos;

        int cmp_result = strcmp(base + newMeta->m_name_pos, base + curItem->m_name_pos);
        if (cmp_result <= 0) {
            endPos = curPos;
        }
        else {
            beginPos = curPos + 1;
        }
    }

    putAt = searchStart + curPos;

    if (strcmp(base + newMeta->m_name_pos, base + putAt->m_name_pos) == 0) {
        DR_NOTIFY_ERROR(userData, errorProcessor, CPE_DR_ERROR_META_NAME_CONFLICT);
    }

    memmove(
        searchStart + curPos + 1,
        searchStart + curPos,
        sizeof(struct tagDRMetaIdxByName) * (metaLib->m_meta_count - curPos));

    putAt->m_name_pos = newMeta->m_name_pos;
    putAt->m_diff_to_base = newMeta->m_self_pos;
}

static void dr_add_metalib_meta_add_index_by_id(
    LPDRMETALIB metaLib, LPDRMETA newMeta, void * userData, dr_inbuild_log_fun_t errorProcessor)
{
    char * base = (char*)(metaLib + 1);
    int beginPos, endPos, curPos;
    struct tagDRMetaIdxById * putAt = NULL;
    struct tagDRMetaIdxById * searchStart =
        (struct tagDRMetaIdxById *)(base + metaLib->m_startpos_meta_by_id);

    for(beginPos = 0, endPos = metaLib->m_meta_count, curPos = (endPos - beginPos - 1) / 2;
        beginPos < endPos;
        curPos = beginPos + (endPos - beginPos - 1) / 2)
    {
        struct tagDRMetaIdxById * curItem = searchStart + curPos;
        
        if (newMeta->m_id <= curItem->m_id) {
            endPos = curPos;
        }
        else {
            beginPos = curPos + 1;
        }
    }

    putAt = searchStart + curPos;

    if (newMeta->m_id == putAt->m_id && newMeta->m_id != -1) {
        DR_NOTIFY_ERROR(userData, errorProcessor, CPE_DR_ERROR_META_ID_CONFLICT);
    }

    memmove(
        searchStart + curPos + 1,
        searchStart + curPos,
        sizeof(struct tagDRMetaIdxById) * (metaLib->m_meta_count - curPos));

    putAt->m_id = newMeta->m_id;
    putAt->m_diff_to_base = newMeta->m_self_pos;
}

LPDRMETA
dr_add_metalib_meta(LPDRMETALIB metaLib, LPDRMETA meta, void * userData, dr_inbuild_log_fun_t errorProcessor) {
    char * base = (char*)(metaLib + 1);
    LPDRMETA newMeta = NULL;
    int newMetaPos = 0;
    int i = 0;
    int newMetaUsedSize = dr_calc_meta_use_size(meta->m_entry_count);

    if (metaLib->m_meta_count >= metaLib->m_meta_max_count) {
        DR_NOTIFY_ERROR(userData, errorProcessor, CPE_DR_ERROR_NO_SPACE_FOR_MATA);
        return NULL;
    }

    newMetaPos = dr_add_metalib_meta_find_next_pos(metaLib);

    if ( (newMetaPos + newMetaUsedSize)
         > (metaLib->m_startpos_meta + metaLib->m_buf_size_meta) )
    {
        DR_NOTIFY_ERROR(userData, errorProcessor, CPE_DR_ERROR_NO_SPACE_FOR_MATA);
        return NULL;
    }

    newMeta = (LPDRMETA)(base + newMetaPos);

    memcpy(newMeta, meta, sizeof(*newMeta));
    newMeta->m_self_pos = newMetaPos;

    dr_add_metalib_meta_add_index_by_name(metaLib, newMeta, userData, errorProcessor);
    dr_add_metalib_meta_add_index_by_id(metaLib, newMeta, userData, errorProcessor);

    /*must inc m_meta_count here
      becuse dr_add_metalib_meta_add_index_by_xxx will use this */
    ++metaLib->m_meta_count;

    return newMeta;;
}

LPDRMETAENTRY
dr_add_meta_entry(LPDRMETA meta, LPDRMETAENTRY entry, void * userData, dr_inbuild_log_fun_t errorProcessor) {
    char * base = (char*)(meta) - meta->m_self_pos;
    LPDRMETAENTRY newEntry =  (LPDRMETAENTRY)(meta + 1);
    int i;

    //search the slot for newEntry
    for(i = 0; i < meta->m_entry_count && newEntry->m_name_pos != 0; ++i, ++newEntry) {
    }
    
    if (i >= meta->m_entry_count) {
        DR_NOTIFY_ERROR(userData, errorProcessor, CPE_DR_ERROR_META_NO_ENTRY);
        return NULL;
    }

    memcpy(newEntry, entry, sizeof(*newEntry));

    newEntry->m_self_to_meta_pos = (char*)newEntry - (char*)meta;

    meta->m_data_size += newEntry->m_unitsize;

    return newEntry;
}

int dr_calc_meta_use_size(int entryCount) {
    return sizeof(struct tagDRMeta) + sizeof(struct tagDRMetaEntry) * entryCount;
}
