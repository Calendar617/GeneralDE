#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_ctypes_info.h"
#include "cpe/dr/dr_error.h"
#include "../dr_internal_types.h"

LPDRMETA dr_lib_find_meta_by_name(LPDRMETALIB a_pstLib, const char* a_pszName) {
    char * base;
    struct tagDRMetaIdxByName * searchStart;
    int beginPos, endPos, curPos;

    base = (char *)(a_pstLib + 1);

    searchStart = (struct tagDRMetaIdxByName * )
        (base + a_pstLib->m_startpos_meta_by_name);

    for(beginPos = 0, endPos = a_pstLib->m_meta_count, curPos = (endPos - beginPos - 1) / 2;
        beginPos < endPos;
        curPos = beginPos + (endPos - beginPos - 1) / 2)
    {
        struct tagDRMetaIdxByName * curItem = searchStart + curPos;
        
        int cmp_result = strcmp(a_pszName, base + curItem->m_name_pos);
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

static int dr_comp_metaidx_by_id(const void * l, const void * r) {
    struct tagDRMetaIdxById * lItem = (struct tagDRMetaIdxById *)l;
    struct tagDRMetaIdxById * rItem = (struct tagDRMetaIdxById *)r;

    return lItem->m_id - rItem->m_id;
}

LPDRMETA dr_lib_find_meta_by_id(LPDRMETALIB a_pstLib, int id) {
    char * base;
    struct tagDRMetaIdxById * searchStart;
    int beginPos, endPos, curPos;

    base = (char *)(a_pstLib + 1);

    searchStart = (struct tagDRMetaIdxById * )
        (base + a_pstLib->m_startpos_meta_by_id);

    for(beginPos = 0, endPos = a_pstLib->m_meta_count, curPos = (endPos - beginPos - 1) / 2;
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
    int i;
    LPDRMETA pstCurMeta = meta;
    char * base = (char *)(meta) - meta->m_self_pos;

    if (a_iBufSize <= 0) {
        return NULL;
    }

    while(a_iOff >= 0 && writePos < a_iBufSize) {
        int beginPos, endPos, curPos;
        LPDRMETAENTRY curEntry = NULL;

        for(beginPos = 0, endPos = pstCurMeta->m_entry_count, curPos = (endPos - beginPos - 1) / 2;
            beginPos < endPos && curEntry == NULL;
            curPos = beginPos + (endPos - beginPos - 1) / 2)
        {
            LPDRMETAENTRY pstEntry = dr_meta_entry_at(pstCurMeta, curPos);

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

LPDRMETAENTRY dr_meta_find_entry_by_name(LPDRMETA meta, const char* a_pszName) {
    int i;
    char * base = (char *)(meta) - meta->m_self_pos;
    LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(meta + 1);

    for(i = 0; i < meta->m_entry_count; ++i) {
        LPDRMETAENTRY pstCurEntry = pstEntryBegin + i;

        if (strcmp(base + pstCurEntry->m_name_pos, a_pszName) == 0) {
            return pstCurEntry;
        }
    }

    return NULL;
}

int dr_meta_find_entry_idx_by_name(LPDRMETA meta, const char* a_pszName) {
    int i;
    char * base = (char *)(meta) - meta->m_self_pos;
    LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(meta + 1);

    for(i = 0; i < meta->m_entry_count; ++i) {
        LPDRMETAENTRY pstCurEntry = pstEntryBegin + i;

        if (strcmp(base + pstCurEntry->m_name_pos, a_pszName) == 0) {
            return i;
        }
    }

    return -1;
}

int dr_meta_find_entry_idx_by_id(LPDRMETA meta, int a_iId) {
    if (a_iId < 0) {
        return -1;
    }

    int i;
    LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(meta + 1);

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

LPDRMETAENTRY dr_meta_find_entry_by_path(LPDRMETA meta, const char* a_pszEntryPath) {
    char * base;
    LPDRMETALIB pstLib;
    LPDRMETA pstCurMeta;
    const char * nameBegin;
    const char * nameEnd;

    base = (char *)(meta) - meta->m_self_pos;
    pstLib = ((LPDRMETALIB)base) - 1;

    pstCurMeta = meta;
    for(nameBegin = a_pszEntryPath, nameEnd = strchr(nameBegin, '.');
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
    return entry->m_version;
}

const char *dr_entry_name(LPDRMETAENTRY a_pstEntry) {
    LPDRMETA pstMeta = (LPDRMETA)((char * )a_pstEntry - a_pstEntry->m_self_to_meta_pos);

    char * base = (char *)(pstMeta) - pstMeta->m_self_pos;

    return base + a_pstEntry->m_name_pos;
}

const char *dr_entry_cname(LPDRMETAENTRY a_pstEntry) {
    LPDRMETA pstMeta = (LPDRMETA)((char * )a_pstEntry - a_pstEntry->m_self_to_meta_pos);

    char * base = (char *)(pstMeta) - pstMeta->m_self_pos;

    if (a_pstEntry->m_cname_pos < 0) {
        return "";
    }
    else {
        return base + a_pstEntry->m_cname_pos;
    }
}

const char *dr_entry_desc(LPDRMETAENTRY a_pstEntry) {
    LPDRMETA pstMeta = (LPDRMETA)((char * )a_pstEntry - a_pstEntry->m_self_to_meta_pos);

    char * base = (char *)(pstMeta) - pstMeta->m_self_pos;

    if (a_pstEntry->m_desc_pos < 0) {
        return "";
    }
    else {
        return base + a_pstEntry->m_desc_pos;
    }
}

LPDRMACROSGROUP dr_entry_macrosgroup(LPDRMETAENTRY a_pstEntry) {
    //TODO
    return NULL;
}

int dr_entry_id(LPDRMETAENTRY a_pstEntry) {
    return a_pstEntry->m_id;
}

size_t dr_entry_size(LPDRMETAENTRY a_pstEntry) {
    return a_pstEntry->m_unitsize;
}

int dr_entry_type(LPDRMETAENTRY a_pstEntry) {
    return a_pstEntry->m_type;
}

int dr_entry_array_count(LPDRMETAENTRY a_pstEntry) {
    return a_pstEntry->m_array_count;
}

LPDRMETA dr_entry_self_meta(LPDRMETAENTRY a_pstEntry) {
    return (LPDRMETA)((char * )a_pstEntry - a_pstEntry->m_self_to_meta_pos);
}

LPDRMETA dr_entry_ref_meta(LPDRMETAENTRY a_pstEntry) {
    if (a_pstEntry->m_type > CPE_DR_TYPE_COMPOSITE) {
        return NULL;
    }
    else {
        LPDRMETA pstMeta = (LPDRMETA)((char * )a_pstEntry - a_pstEntry->m_self_to_meta_pos);
        char * base = (char *)(pstMeta) - pstMeta->m_self_pos;
        return (LPDRMETA)(base + a_pstEntry->m_ref_type_pos);
    }
}

const char *dr_entry_customattr(LPDRMETALIB a_pstLib, LPDRMETAENTRY a_pstEntry) {
    //TODO
    return NULL;
}

int dr_lib_find_macro_value(int *a_piID, LPDRMETALIB a_pstLib, const  char *a_pszName) {
    char * base;
    int i;
    struct tagDRMacro* macros = 0;
    struct tagDRMacro* curMacro = 0;

    base = (char *)(a_pstLib + 1);

    macros = (struct tagDRMacro*)(base + a_pstLib->m_startpos_macro);

    for(i = 0; i < a_pstLib->m_macro_count; ++i) {
        curMacro = macros + i;
        if (strcmp(a_pszName, (char *)(a_pstLib + 1) + curMacro->m_name_pos) == 0) {
            *a_piID = curMacro->m_value;
            return 0;
        }
    }

    return -1;
}

int dr_lib_macro_num(LPDRMETALIB a_pstLib) {
    assert(a_pstLib);

    return a_pstLib->m_macro_count;
}

LPDRMACRO dr_lib_macro_at(LPDRMETALIB a_pstLib, int a_iIdx) {
    assert(a_pstLib);

    if (a_iIdx < 0 || a_iIdx >= a_pstLib->m_macro_count) {
        return 0;
    }
    else {
        char * base = (char *)(a_pstLib + 1);
        return (struct tagDRMacro*)(base + a_pstLib->m_startpos_macro) + a_iIdx;
    }
}

const char* dr_macro_name(LPDRMETALIB a_pstLib, LPDRMACRO a_pstMacro) {
    char * base;

    assert(a_pstLib);
    base = (char *)(a_pstLib + 1);

    return (char *)base + a_pstMacro->m_name_pos;
}

int dr_macro_value(LPDRMACRO a_pstMacro) {
    return a_pstMacro->m_value;
}

const char* dr_macro_desc(LPDRMETALIB a_pstLib, LPDRMACRO a_pstMacro) {
    assert(a_pstLib);
    if (a_pstMacro->m_desc_pos < 0) {
        return "";
    }
    else {
        char * base = (char *)(a_pstLib + 1);
        return (char *)(base) + a_pstMacro->m_desc_pos;
    }
}

LPDRMACROSGROUP dr_macro_macrosgroup(LPDRMETALIB a_pstLib, LPDRMACRO a_pstMacro) {
    //TODO
}

int dr_lib_macrosgroup_num(LPDRMETALIB a_pstLib) {
    //TODO
}

LPDRMACROSGROUP dr_lib_macrosgroup_at(LPDRMETALIB a_pstLib, int a_iIdx) {
    //TODO
}

const char* dr_macrosgroup_name(LPDRMACROSGROUP a_pstGroup) {
    //TODO
}

int dr_macrosgroup_macro_num(LPDRMACROSGROUP a_pstGroup) {
    //TODO
}

LPDRMACRO dr_macrosgroup_macro_at(LPDRMETALIB a_pstLib, LPDRMACROSGROUP a_pstGroup, int a_iIdx) {
    //TODO
}
