#include <assert.h>
#include <strings.h>
#include <string.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_ctype_ops.h"
#include "dr_metalib_ops.h"
#include "dr_inbuild_error.h"

int dr_init_lib(LPDRMETALIB pstLib, const LPDRLIBPARAM pstParam) {
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
    error_monitor_t em)
{
    char * base;
    int startPos;
    int dataSize;

    if (data == NULL || data[0] == 0) {
        return -1;
    }

    dataSize = strlen(data) + 1;
    if ((*usedCount + dataSize) > metaLib->m_buf_size_str) { /*overflow*/
        DR_NOTIFY_ERROR(em, CPE_DR_ERROR_NO_SPACE_FOR_STRBUF);
        return -1;
    }

    startPos = metaLib->m_startpos_str + *usedCount;
    base = (char *)(metaLib + 1);

    strncpy(base + startPos, data, dataSize);

    *usedCount += dataSize;

    return startPos;
}

LPDRMACRO dr_add_metalib_macro(LPDRMETALIB metaLib, LPDRMACRO macro, error_monitor_t em) {
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
    LPDRMETALIB metaLib, LPDRMETA newMeta, error_monitor_t em)
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

    if (curPos < metaLib->m_meta_count) {
        if (strcmp(base + newMeta->m_name_pos, base + putAt->m_name_pos) == 0) {
            DR_NOTIFY_ERROR(em, CPE_DR_ERROR_META_NAME_CONFLICT);
        }

        memmove(
            searchStart + curPos + 1,
            searchStart + curPos,
            sizeof(struct tagDRMetaIdxByName) * (metaLib->m_meta_count - curPos));
    }

    putAt->m_name_pos = newMeta->m_name_pos;
    putAt->m_diff_to_base = newMeta->m_self_pos;
}

static void dr_add_metalib_meta_add_index_by_id(
    LPDRMETALIB metaLib, LPDRMETA newMeta, error_monitor_t em)
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

    if (curPos < metaLib->m_meta_count) {
        if (newMeta->m_id == putAt->m_id && newMeta->m_id >= 0) {
            DR_NOTIFY_ERROR(em, CPE_DR_ERROR_META_ID_CONFLICT);
        }

        memmove(
            searchStart + curPos + 1,
            searchStart + curPos,
            sizeof(struct tagDRMetaIdxById) * (metaLib->m_meta_count - curPos));
    }

    putAt->m_id = newMeta->m_id;
    putAt->m_diff_to_base = newMeta->m_self_pos;
}

LPDRMETA
dr_add_metalib_meta(LPDRMETALIB metaLib, LPDRMETA meta, error_monitor_t em) {
    char * base = (char*)(metaLib + 1);
    LPDRMETA newMeta = NULL;
    int newMetaPos = 0;
    int newMetaUsedSize = dr_calc_meta_use_size(meta->m_entry_count);

    if (metaLib->m_meta_count >= metaLib->m_meta_max_count) {
        DR_NOTIFY_ERROR(em, CPE_DR_ERROR_NO_SPACE_FOR_MATA);
        return NULL;
    }

    newMetaPos = dr_add_metalib_meta_find_next_pos(metaLib);

    if ( (newMetaPos + newMetaUsedSize)
         > (metaLib->m_startpos_meta + metaLib->m_buf_size_meta) )
    {
        DR_NOTIFY_ERROR(em, CPE_DR_ERROR_NO_SPACE_FOR_MATA);
        return NULL;
    }

    newMeta = (LPDRMETA)(base + newMetaPos);

    memcpy(newMeta, meta, sizeof(*newMeta));
    newMeta->m_self_pos = newMetaPos;
    newMeta->m_entry_count = 0;

    dr_add_metalib_meta_add_index_by_name(metaLib, newMeta, em);
    dr_add_metalib_meta_add_index_by_id(metaLib, newMeta, em);

    /*must inc m_meta_count here
      becuse dr_add_metalib_meta_add_index_by_xxx will use this */
    ++metaLib->m_meta_count;

    return newMeta;;
}

void dr_add_meta_entry_calc_align(
    LPDRMETA meta, LPDRMETAENTRY newEntry, int entryAlign,
    error_monitor_t em)
{
    int align = entryAlign < meta->m_align ? entryAlign : meta->m_align;

    if (meta->m_type == CPE_DR_TYPE_STRUCT) {
        int panding = meta->m_data_size % align;
        if (panding) {
            panding = align - panding;
        }

        newEntry->m_data_start_pos = meta->m_data_size + panding;
        meta->m_data_size += panding + newEntry->m_unitsize;
    }
    else if (meta->m_type == CPE_DR_TYPE_UNION) {
        newEntry->m_data_start_pos = 0;
        if (meta->m_data_size < newEntry->m_unitsize) {
            meta->m_data_size = newEntry->m_unitsize;
        }
    }
    else {
        CPE_ERROR_EX(em, CPE_DR_ERROR_ENTRY_INVALID_TYPE_VALUE, "unknown meta type %d!", meta->m_type);
    }
}

int dr_add_meta_entry_set_type_calc_align(LPDRMETA meta, LPDRMETAENTRY entry, error_monitor_t em) {
    char * base = (char*)(meta) - meta->m_self_pos;
    int entryAlign = 0;

    //process type
    if (entry->m_type <= CPE_DR_TYPE_COMPOSITE) {/*is composite type*/
        LPDRMETA usedType = NULL;

        if (entry->m_ref_type_pos < 0) {
            DR_NOTIFY_ERROR(em, CPE_DR_ERROR_ENTRY_INVALID_TYPE_VALUE);
            return -1;
        }

        usedType = (LPDRMETA)(base + entry->m_ref_type_pos);

        if (usedType == NULL) {
            DR_NOTIFY_ERROR(em, CPE_DR_ERROR_ENTRY_INVALID_TYPE_VALUE);
            return -1;
        }

        entry->m_unitsize = usedType->m_data_size;
        entryAlign = usedType->m_align;
    }
    else { /* is basic type */
        const struct tagDRCTypeInfo * typeInfo = dr_find_ctype_info_by_type(entry->m_type);
        if (typeInfo == NULL) {
            DR_NOTIFY_ERROR(em, CPE_DR_ERROR_ENTRY_INVALID_TYPE_VALUE);
            return -1;
        }

        if ((int)typeInfo->m_size > 0) {
            entryAlign = typeInfo->m_size;
        }
        else {
            entryAlign = 1;
        }
    }

    return entryAlign;
}

LPDRMETAENTRY
dr_add_meta_entry(LPDRMETA meta, LPDRMETAENTRY entry, error_monitor_t em) {
    LPDRMETAENTRY newEntry =  (LPDRMETAENTRY)(meta + 1) + meta->m_entry_count;
    int i;
    int entryAlign = 0;

    if (entry->m_name_pos < 0) {
        CPE_ERROR_EX(em, CPE_DR_ERROR_META_NO_NAME, "entry have no name!");
        return NULL;
    }

    //process type
    entryAlign = dr_add_meta_entry_set_type_calc_align(meta, entry, em);
    if (entryAlign < 0) {
        return NULL;
    }

    memcpy(newEntry, entry, sizeof(*newEntry));

    newEntry->m_self_to_meta_pos = (char*)newEntry - (char*)meta;

    dr_add_meta_entry_calc_align(meta, newEntry, entryAlign, em);

    if (newEntry->m_version > meta->m_current_version) {
        meta->m_current_version = newEntry->m_version;
    }

    ++meta->m_entry_count;
    return newEntry;
}

int dr_calc_meta_use_size(int entryCount) {
    return sizeof(struct tagDRMeta) + sizeof(struct tagDRMetaEntry) * entryCount;
}

void dr_meta_complete(LPDRMETA meta, error_monitor_t em) {
    int panding = meta->m_data_size % meta->m_align;
    if (panding) {
        panding = meta->m_align - panding;
    }
    meta->m_data_size += panding;

    if (meta->m_entry_count == 0) {
        CPE_ERROR_EX(em, CPE_DR_ERROR_META_NO_ENTRY, "meta %s have no entry", dr_meta_name(meta));
    }
}
