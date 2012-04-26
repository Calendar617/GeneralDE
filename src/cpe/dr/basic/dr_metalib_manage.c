#include <assert.h>
#include <string.h>
#include "cpe/pal/pal_stdio.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_ctypes_info.h"
#include "cpe/dr/dr_error.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"

int dr_lib_meta_num(LPDRMETALIB metaLib) {
    return metaLib->m_meta_count;
}

LPDRMETA dr_lib_meta_at(LPDRMETALIB metaLib, int idx) {
    assert(metaLib);

    if (idx >= 0 && idx < metaLib->m_meta_count) {
        char * base = (char *)(metaLib + 1);
        struct tagDRMetaIdxByName * idx_start = (struct tagDRMetaIdxByName *)(base + metaLib->m_startpos_meta_by_name);

        return (LPDRMETA)(base + (idx_start + idx)->m_diff_to_base);
    }
    else {
        return NULL;
    }
}

LPDRMETA dr_lib_find_meta_by_name(LPDRMETALIB metaLib, const char* name) {
    char * base;
    struct tagDRMetaIdxByName * searchStart;
    int beginPos, endPos, curPos;

    assert(metaLib);
    assert(name);

    base = (char *)(metaLib + 1);

    searchStart = (struct tagDRMetaIdxByName * )
        (base + metaLib->m_startpos_meta_by_name);

    for(beginPos = 0, endPos = metaLib->m_meta_count, curPos = (endPos - beginPos - 1) / 2;
        beginPos < endPos;
        curPos = beginPos + (endPos - beginPos - 1) / 2)
    {
        struct tagDRMetaIdxByName * curItem;
        int cmp_result;

        curItem = searchStart + curPos;
        cmp_result = strcmp(name, base + (curItem->m_name_pos & 0xFFFFFFFF));
        if (cmp_result == 0) {
            return (LPDRMETA)(base + curItem->m_diff_to_base);
        }
        else if (cmp_result < 0) {
            endPos = curPos;
        }
        else { /*cmp_result > 0*/
            beginPos = curPos + 1;
        }
    }
    
    return NULL;
}

LPDRMETA dr_lib_find_meta_by_id(LPDRMETALIB metaLib, int id) {
    char * base;
    struct tagDRMetaIdxById * searchStart;
    int beginPos, endPos, curPos;

    base = (char *)(metaLib + 1);

    searchStart = (struct tagDRMetaIdxById * )
        (base + metaLib->m_startpos_meta_by_id);

    for(beginPos = 0, endPos = metaLib->m_meta_count, curPos = (endPos - beginPos - 1) / 2;
        beginPos < endPos;
        curPos = beginPos + (endPos - beginPos - 1) / 2)
    {
        struct tagDRMetaIdxById * curItem = searchStart + curPos;
        
        if (id == curItem->m_id) {
            return (LPDRMETA)(base + curItem->m_diff_to_base);
        }
        else if (id < curItem->m_id) {
            endPos = curPos;
        }
        else { /*id > curItem->m_id*/
            beginPos = curPos + 1;
        }
    }
    
    return NULL;
}

int dr_meta_based_version(LPDRMETA meta) {
    return meta->m_based_version;
}

int dr_meta_current_version(LPDRMETA meta) {
    return meta->m_current_version;
}

int dr_meta_type(LPDRMETA meta) {
    return meta->m_type;
}

const char *dr_meta_name(LPDRMETA meta) {
    return (char *)(meta) - meta->m_self_pos + meta->m_name_pos;
}

const char *dr_meta_desc(LPDRMETA meta) {
    if (meta->m_desc_pos < 0) {
        return "";
    }
    else {
        return (char *)(meta) - meta->m_self_pos + meta->m_desc_pos;
    }
}

int dr_meta_id(LPDRMETA meta) {
    return meta->m_id;
}

size_t dr_meta_size(LPDRMETA meta) {
    return meta->m_data_size;
}

int dr_meta_align(LPDRMETA meta) {
    return meta->m_align;
}

int dr_meta_entry_num(LPDRMETA meta) {
    return meta->m_entry_count;
}

char * dr_meta_off_to_path(LPDRMETA meta, int a_iOff, char * a_pBuf, size_t a_iBufSize) {
    int writePos = 0;
    LPDRMETA pstCurMeta = meta;
    LPDRMETAENTRY pstEntry;
    char * base = (char *)(meta) - meta->m_self_pos;

    if (a_iBufSize <= 0) {
        return NULL;
    }

    while(a_iOff >= 0 && (size_t)writePos < a_iBufSize) {
        int beginPos, endPos, curPos;
        LPDRMETAENTRY curEntry = NULL;

        for(beginPos = 0, endPos = pstCurMeta->m_entry_count, curPos = (endPos - beginPos - 1) / 2;
            beginPos < endPos && curEntry == NULL;
            curPos = beginPos + (endPos - beginPos - 1) / 2)
        {
            pstEntry = dr_meta_entry_at(pstCurMeta, curPos);

            if (a_iOff < pstEntry->m_data_start_pos) {
                endPos = curPos;
            }
            else { /* a_iOff >= pstEntry->m_data_start_pos */
                if (a_iOff < (pstEntry->m_data_start_pos + pstEntry->m_unitsize)) {
                    curEntry = pstEntry;
                }
                else {
                    beginPos = curPos + 1;
                }
            }
        }

        if (curEntry == NULL) {
            return NULL;
        }

        writePos += snprintf(a_pBuf + writePos,
                             a_iBufSize - writePos,
                             pstCurMeta == meta ? "%s" : ".%s",
                             dr_entry_name(curEntry));

        if (curEntry->m_type <= CPE_DR_TYPE_COMPOSITE) {
            pstCurMeta = (LPDRMETA)(base + curEntry->m_ref_type_pos);
            a_iOff -= curEntry->m_data_start_pos;
        }
        else {
            break;
        }
    }

    return writePos == 0 ? NULL : a_pBuf;
}

int dr_meta_path_to_off(LPDRMETA meta, const char * path, LPDRMETAENTRY * entry) {
    char * base;
    LPDRMETA pstCurMeta;
    const char * nameBegin;
    const char * nameEnd;
    LPDRMETAENTRY pstEntry;
    int off;

    base = (char *)(meta) - meta->m_self_pos;

    off = 0;
    pstCurMeta = meta;
    for(nameBegin = path, nameEnd = strchr(nameBegin, '.');
        nameEnd;
        nameBegin = nameEnd + 1, nameEnd = strchr(nameBegin, '.'))
    {
        int i;
        LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(pstCurMeta + 1);
        pstEntry = NULL;

        for(i = 0; i < pstCurMeta->m_entry_count && pstEntry == NULL; ++i) {
            LPDRMETAENTRY pstCheckEntry = pstEntryBegin + i;

            if (strncmp(base + pstCheckEntry->m_name_pos, nameBegin, nameEnd - nameBegin) == 0) {
                pstEntry = pstCheckEntry;
            }
        }

        if (pstEntry == NULL) {
            return -1;
        }

        if (pstEntry->m_type > CPE_DR_TYPE_COMPOSITE) {
            return -1;
        }

        off += pstEntry->m_data_start_pos;
        pstCurMeta = (LPDRMETA)(base + pstEntry->m_ref_type_pos);
    }

    pstEntry = dr_meta_find_entry_by_name(pstCurMeta, nameBegin);
    if (pstEntry) {
        off += pstEntry->m_data_start_pos;
        if (entry) *entry = pstEntry;
        return off;
    }
    else {
        return -1;
    }
}

LPDRMETALIB dr_meta_owner_lib(LPDRMETA meta) {
    char * base = (char *)(meta) - meta->m_self_pos;
    return ((LPDRMETALIB)base) - 1;
}

LPDRMETAENTRY dr_meta_find_entry_by_name(LPDRMETA meta, const char* name) {
    int i;
    char * base = (char *)(meta) - meta->m_self_pos;
    LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(meta + 1);

    for(i = 0; i < meta->m_entry_count; ++i) {
        LPDRMETAENTRY pstCurEntry = pstEntryBegin + i;

        if (strcmp(base + pstCurEntry->m_name_pos, name) == 0) {
            return pstCurEntry;
        }
    }

    return NULL;
}

int dr_meta_find_entry_idx_by_name(LPDRMETA meta, const char* name) {
    int i;
    char * base = (char *)(meta) - meta->m_self_pos;
    LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(meta + 1);

    for(i = 0; i < meta->m_entry_count; ++i) {
        LPDRMETAENTRY pstCurEntry = pstEntryBegin + i;

        if (strcmp(base + pstCurEntry->m_name_pos, name) == 0) {
            return i;
        }
    }

    return -1;
}

int dr_meta_find_entry_idx_by_id(LPDRMETA meta, int a_iId) {
    int i;
    LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(meta + 1);

    if (a_iId < 0) {
        return -1;
    }

    for(i = 0; i < meta->m_entry_count; ++i) {
        LPDRMETAENTRY pstCurEntry = pstEntryBegin + i;

        if (pstCurEntry->m_id == a_iId) {
            return i;
        }
    }

    return -1;
}

LPDRMETAENTRY dr_meta_entry_at(LPDRMETA meta, int a_idxEntry) {
    if (a_idxEntry < 0 || a_idxEntry >= meta->m_entry_count) {
        return NULL;
    }

    return (struct tagDRMetaEntry *)(meta + 1) + a_idxEntry;
}

LPDRMETAENTRY dr_meta_find_entry_by_path(LPDRMETA meta, const char* entryPath) {
    char * base;
    LPDRMETA pstCurMeta;
    const char * nameBegin;
    const char * nameEnd;

    assert(meta);
    assert(entryPath);

    base = (char *)(meta) - meta->m_self_pos;

    pstCurMeta = meta;
    for(nameBegin = entryPath, nameEnd = strchr(nameBegin, '.');
        nameEnd;
        nameBegin = nameEnd + 1, nameEnd = strchr(nameBegin, '.'))
    {
        int i;
        LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(pstCurMeta + 1);
        LPDRMETAENTRY pstEntry = NULL;

        for(i = 0; i < pstCurMeta->m_entry_count && pstEntry == NULL; ++i) {
            LPDRMETAENTRY pstCheckEntry = pstEntryBegin + i;

            if (strncmp(base + pstCheckEntry->m_name_pos, nameBegin, nameEnd - nameBegin) == 0) {
                pstEntry = pstCheckEntry;
            }
        }

        if (pstEntry == NULL) {
            return NULL;
        }

        if (pstEntry->m_type > CPE_DR_TYPE_COMPOSITE) {
            return NULL;
        }

        pstCurMeta = (LPDRMETA)(base + pstEntry->m_ref_type_pos);
    }

    return dr_meta_find_entry_by_name(pstCurMeta, nameBegin);
}

int dr_entry_version(LPDRMETAENTRY entry) {
    assert(entry);
    return entry->m_version;
}

const char *dr_entry_name(LPDRMETAENTRY entry) {
    assert(entry);
    if (entry->m_name_pos < 0) {
        return "";
    }
    else {
        LPDRMETA pstMeta = (LPDRMETA)((char * )entry - entry->m_self_to_meta_pos);
        char * base = (char *)(pstMeta) - pstMeta->m_self_pos;
        return base + entry->m_name_pos;
    }
}

const char *dr_entry_cname(LPDRMETAENTRY entry) {
    assert(entry);
    if (entry->m_cname_pos < 0) {
        return "";
    }
    else {
        LPDRMETA pstMeta = (LPDRMETA)((char * )entry - entry->m_self_to_meta_pos);
        char * base = (char *)(pstMeta) - pstMeta->m_self_pos;
        return base + entry->m_cname_pos;
    }
}

const char *dr_entry_desc(LPDRMETAENTRY entry) {
    assert(entry);
    if (entry->m_desc_pos < 0) {
        return "";
    }
    else {
        LPDRMETA pstMeta = (LPDRMETA)((char * )entry - entry->m_self_to_meta_pos);
        char * base = (char *)(pstMeta) - pstMeta->m_self_pos;
        return base + entry->m_desc_pos;
    }
}

const void * dr_entry_dft_value(LPDRMETAENTRY entry) {
    assert(entry);
    if (entry->m_dft_value_pos < 0) {
        return NULL;
    }
    else {
        LPDRMETA pstMeta = (LPDRMETA)((char * )entry - entry->m_self_to_meta_pos);
        char * base = (char *)(pstMeta) - pstMeta->m_self_pos;
        return (const void *)(base + entry->m_dft_value_pos);
    }
}

LPDRMETAENTRY
dr_entry_select_entry(LPDRMETAENTRY entry) {
    assert(entry);
    if (entry->m_select_entry_pos < 0) {
        return NULL;
    }
    else {
        LPDRMETA pstMeta = (LPDRMETA)((char * )entry - entry->m_self_to_meta_pos);
        char * base = (char *)(pstMeta) - pstMeta->m_self_pos;
        return (LPDRMETAENTRY)(base + entry->m_select_entry_pos);
    }
}

LPDRMACROSGROUP dr_entry_macrosgroup(LPDRMETAENTRY entry) {
    //TODO
    return NULL;
}

int dr_entry_id(LPDRMETAENTRY entry) {
    return entry->m_id;
}

size_t dr_entry_size(LPDRMETAENTRY entry) {
    return entry->m_unitsize;
}

int dr_entry_type(LPDRMETAENTRY entry) {
    return entry->m_type;
}

int dr_entry_align(LPDRMETAENTRY entry) {
    if (entry->m_type > CPE_DR_TYPE_COMPOSITE) {
        const struct tagDRCTypeInfo * typeInfo;

        typeInfo = dr_find_ctype_info_by_type(entry->m_type);

        return typeInfo == NULL ? -1 : typeInfo->m_align;
    }
    else {
        return dr_meta_align(dr_entry_ref_meta(entry));
    }
}

int dr_entry_array_count(LPDRMETAENTRY entry) {
    return entry->m_array_count;
}

size_t dr_entry_data_start_pos(LPDRMETAENTRY entry) {
    return entry->m_data_start_pos;
}

size_t dr_entry_element_size(LPDRMETAENTRY entry) {
    if (entry->m_type <= CPE_DR_TYPE_COMPOSITE) {
        LPDRMETA refMeta = dr_entry_ref_meta(entry);
        if (refMeta == NULL) return 0;
        return dr_meta_size(refMeta);
    }
    else if (entry->m_type == CPE_DR_TYPE_STRING) {
        return entry->m_size;
    }
    else {
        const struct tagDRCTypeInfo * typeInfo;
        typeInfo = dr_find_ctype_info_by_type(entry->m_type);
        if (typeInfo == NULL) return 0;
        return typeInfo->m_size;
    }
}

LPDRMETAENTRY
dr_entry_array_refer_entry(LPDRMETAENTRY entry) {
    assert(entry);
    if (entry->m_array_count == 1 || entry->m_array_refer_entry_pos < 0) {
        return NULL;
    }
    else {
        LPDRMETA pstMeta = (LPDRMETA)((char * )entry - entry->m_self_to_meta_pos);
        char * base = (char *)(pstMeta) - pstMeta->m_self_pos;
        return (LPDRMETAENTRY)(base + entry->m_array_refer_entry_pos);
    }
}

LPDRMETA dr_entry_self_meta(LPDRMETAENTRY entry) {
    return (LPDRMETA)((char * )entry - entry->m_self_to_meta_pos);
}

LPDRMETA dr_entry_ref_meta(LPDRMETAENTRY entry) {
    if (entry->m_type > CPE_DR_TYPE_COMPOSITE) {
        return NULL;
    }
    else {
        LPDRMETA pstMeta = (LPDRMETA)((char * )entry - entry->m_self_to_meta_pos);
        char * base = (char *)(pstMeta) - pstMeta->m_self_pos;
        return (LPDRMETA)(base + entry->m_ref_type_pos);
    }
}

const char *dr_entry_customattr(LPDRMETALIB metaLib, LPDRMETAENTRY entry) {
    //TODO
    return NULL;
}

int dr_lib_find_macro_value(int *result, LPDRMETALIB metaLib, const  char *name) {
    char * base;
    int i;
    struct tagDRMacro* macros = 0;
    struct tagDRMacro* curMacro = 0;

    assert(result);
    assert(metaLib);
    assert(name);

    base = (char *)(metaLib + 1);
    macros = (struct tagDRMacro*)(base + metaLib->m_startpos_macro);

    for(i = 0; i < metaLib->m_macro_count; ++i) {
        curMacro = macros + i;
        if (strcmp(name, (char *)(metaLib + 1) + curMacro->m_name_pos) == 0) {
            *result = curMacro->m_value;
            return 0;
        }
    }

    return -1;
}

int dr_lib_macro_num(LPDRMETALIB metaLib) {
    assert(metaLib);

    return metaLib->m_macro_count;
}

LPDRMACRO dr_lib_macro_at(LPDRMETALIB metaLib, int a_iIdx) {
    assert(metaLib);

    if (a_iIdx < 0 || a_iIdx >= metaLib->m_macro_count) {
        return 0;
    }
    else {
        char * base = (char *)(metaLib + 1);
        return (struct tagDRMacro*)(base + metaLib->m_startpos_macro) + a_iIdx;
    }
}

const char* dr_macro_name(LPDRMETALIB metaLib, LPDRMACRO a_pstMacro) {
    char * base;

    assert(metaLib);
    base = (char *)(metaLib + 1);

    return (char *)base + a_pstMacro->m_name_pos;
}

int dr_macro_value(LPDRMACRO a_pstMacro) {
    return a_pstMacro->m_value;
}

const char* dr_macro_desc(LPDRMETALIB metaLib, LPDRMACRO a_pstMacro) {
    assert(metaLib);
    if (a_pstMacro->m_desc_pos < 0) {
        return "";
    }
    else {
        char * base = (char *)(metaLib + 1);
        return (char *)(base) + a_pstMacro->m_desc_pos;
    }
}

LPDRMACROSGROUP dr_macro_macrosgroup(LPDRMETALIB metaLib, LPDRMACRO a_pstMacro) {
    //TODO
    return NULL;
}

int dr_lib_macrosgroup_num(LPDRMETALIB metaLib) {
    //TODO
    return -1;
}

LPDRMACROSGROUP dr_lib_macrosgroup_at(LPDRMETALIB metaLib, int a_iIdx) {
    //TODO
    return NULL;
}

const char* dr_macrosgroup_name(LPDRMACROSGROUP a_pstGroup) {
    //TODO
    return NULL;
}

int dr_macrosgroup_macro_num(LPDRMACROSGROUP a_pstGroup) {
    //TODO
    return -1;
}

LPDRMACRO dr_macrosgroup_macro_at(LPDRMETALIB metaLib, LPDRMACROSGROUP a_pstGroup, int a_iIdx) {
    //TODO
    return NULL;
}
