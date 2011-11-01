#include <stdio.h>
#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_ctypes_info.h"
#include "cpe/dr/dr_error.h"
#include "../dr_internal_types.h"

LPDRMETA dr_get_meta_by_name(IN LPDRMETALIB a_pstLib, IN const char* a_pszName) {
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

LPDRMETA dr_get_meta_by_id(IN LPDRMETALIB a_pstLib, IN int id) {
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

int dr_get_meta_based_version(IN LPDRMETA a_pstMeta) {
    return a_pstMeta->m_based_version;
}

int dr_get_meta_current_version(IN LPDRMETA a_pstMeta) {
    return a_pstMeta->m_current_version;
}

int dr_get_meta_type(IN LPDRMETA a_pstMeta) {
    return a_pstMeta->m_type;
}

const char *dr_get_meta_name(IN LPDRMETA a_pstMeta) {
    return (char *)(a_pstMeta) - a_pstMeta->m_self_pos + a_pstMeta->m_name_pos;
}

const char *dr_get_meta_desc(IN LPDRMETA a_pstMeta) {
    if (a_pstMeta->m_desc_pos < 0) {
        return "";
    }
    else {
        return (char *)(a_pstMeta) - a_pstMeta->m_self_pos + a_pstMeta->m_desc_pos;
    }
}

int dr_get_meta_id(IN LPDRMETA a_pstMeta) {
    return a_pstMeta->m_id;
}

size_t dr_get_meta_size(IN LPDRMETA a_pstMeta) {
    return a_pstMeta->m_data_size;
}

int dr_get_entry_num(IN LPDRMETA a_pstMeta) {
    return a_pstMeta->m_entry_count;
}

int dr_sizeinfo_to_off(LPDRSIZEINFO a_pstRedirector, LPDRMETA a_pstMeta, int a_iEntry, const char* a_pszName) {
    //TODO
    return -1;
}

int dr_entry_path_to_off(IN LPDRMETA a_pstMeta, INOUT LPDRMETAENTRY *a_ppstEntry, OUT DROFF *a_piHOff, IN const char *a_pszPath) {
    //TODO
    return -1;
}

char *dr_entry_off_to_path(IN LPDRMETA a_pstMeta, IN int a_iOff, char * a_pBuf, size_t a_iBufSize) {
    int writePos = 0;
    int i;
    LPDRMETA pstCurMeta = a_pstMeta;
    char * base = (char *)(a_pstMeta) - a_pstMeta->m_self_pos;

    if (a_iBufSize <= 0) {
        return NULL;
    }

    if (a_iOff < 0 || a_iOff >= pstCurMeta->m_data_size) {
        return NULL;
    }

    while(a_iOff >= 0 && writePos < a_iBufSize) {
        for(i = 0; i < pstCurMeta->m_entry_count;) {
            LPDRMETAENTRY pstEntry = dr_get_entry_by_index(pstCurMeta, i);

            if (a_iOff < pstEntry->m_unitsize) {/*in this entry*/
                writePos += snprintf(a_pBuf + writePos,
                                     a_iBufSize - writePos,
                                     pstCurMeta == a_pstMeta ? "%s" : ".%s",
                                     dr_get_entry_name(pstEntry));

                if (pstEntry->m_type <= CPE_DR_TYPE_COMPOSITE) {
                    pstCurMeta = (LPDRMETA)(base + pstEntry->m_ref_type_pos);
                }
                else {
                    a_iOff = -1; /*already founded*/
                }

                break;
            }
            else { /*not in this entry*/
                a_iOff -= pstEntry->m_unitsize;
                ++i;
            }
        }
    }

    return writePos == 0 ? NULL : a_pBuf;
}

int dr_do_have_autoincrement_entry(IN LPDRMETA a_pstMeta) {
    //TODO
}

const char *dr_get_entry_name(IN LPDRMETAENTRY a_pstEntry) {
    LPDRMETA pstMeta = (LPDRMETA)((char * )a_pstEntry - a_pstEntry->m_self_to_meta_pos);

    char * base = (char *)(pstMeta) - pstMeta->m_self_pos;

    return base + a_pstEntry->m_name_pos;
}

const char *dr_get_entry_cname(IN LPDRMETAENTRY a_pstEntry) {
    LPDRMETA pstMeta = (LPDRMETA)((char * )a_pstEntry - a_pstEntry->m_self_to_meta_pos);

    char * base = (char *)(pstMeta) - pstMeta->m_self_pos;

    if (a_pstEntry->m_cname_pos < 0) {
        return "";
    }
    else {
        return base + a_pstEntry->m_cname_pos;
    }
}

const char *dr_get_entry_desc(IN LPDRMETAENTRY a_pstEntry) {
    LPDRMETA pstMeta = (LPDRMETA)((char * )a_pstEntry - a_pstEntry->m_self_to_meta_pos);

    char * base = (char *)(pstMeta) - pstMeta->m_self_pos;

    if (a_pstEntry->m_desc_pos < 0) {
        return "";
    }
    else {
        return base + a_pstEntry->m_desc_pos;
    }
}

LPDRMACROSGROUP dr_get_entry_macrosgroup(IN LPDRMETAENTRY a_pstEntry) {
    //TODO
}

LPDRMETAENTRY dr_get_entryptr_by_name(IN LPDRMETA a_pstMeta, IN const char* a_pszName) {
    int i;
    char * base = (char *)(a_pstMeta) - a_pstMeta->m_self_pos;
    LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(a_pstMeta + 1);

    for(i = 0; i < a_pstMeta->m_entry_count; ++i) {
        LPDRMETAENTRY pstCurEntry = pstEntryBegin + i;

        if (strcmp(base + pstCurEntry->m_name_pos, a_pszName) == 0) {
            return pstCurEntry;
        }
    }

    return NULL;
}

int dr_get_entry_by_name(OUT int* a_piIdx, IN LPDRMETA a_pstMeta, IN const char* a_pszName) {
    int i;
    char * base = (char *)(a_pstMeta) - a_pstMeta->m_self_pos;
    LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(a_pstMeta + 1);

    for(i = 0; i < a_pstMeta->m_entry_count; ++i) {
        LPDRMETAENTRY pstCurEntry = pstEntryBegin + i;

        if (strcmp(base + pstCurEntry->m_name_pos, a_pszName) == 0) {
            *a_piIdx = i;
            return 0;
        }
    }

    return -1;
}

int dr_get_entry_by_id(OUT int* a_piIdx, IN LPDRMETA a_pstMeta, IN int a_iId) {
    if (a_iId < 0) {
        return -1;
    }

    int i;
    LPDRMETAENTRY pstEntryBegin = (LPDRMETAENTRY)(a_pstMeta + 1);

    for(i = 0; i < a_pstMeta->m_entry_count; ++i) {
        LPDRMETAENTRY pstCurEntry = pstEntryBegin + i;

        if (pstCurEntry->m_id == a_iId) {
            *a_piIdx = i;
            return 0;
        }
    }

    return -1;
}

LPDRMETAENTRY dr_get_entry_by_index(IN LPDRMETA a_pstMeta, IN int a_idxEntry) {
    if (a_idxEntry < 0 || a_idxEntry >= a_pstMeta->m_entry_count) {
        return NULL;
    }

    return (struct tagDRMetaEntry *)(a_pstMeta + 1) + a_idxEntry;
}

int dr_get_entry_id(IN LPDRMETAENTRY a_pstEntry) {
    return a_pstEntry->m_id;
}

const char *dr_get_entry_id_name(IN LPDRMETALIB a_pstLib, IN LPDRMETAENTRY a_pstEntry) {
    //TODO
}

size_t dr_get_entry_unitsize(IN LPDRMETAENTRY a_pstEntry) {
    return a_pstEntry->m_unitsize;
}

int dr_get_entry_type(IN LPDRMETAENTRY a_pstEntry) {
    return a_pstEntry->m_type;
}

int dr_get_entry_count(IN LPDRMETAENTRY a_pstEntry) {
    return a_pstEntry->m_count;
}

LPDRMETA dr_get_entry_type_meta(IN LPDRMETALIB a_pstLib, IN LPDRMETAENTRY a_pstEntry) {
    return (LPDRMETA)((char * )a_pstEntry - a_pstEntry->m_self_to_meta_pos);
}

LPDRMETAENTRY dr_get_entry_by_path(IN LPDRMETA a_pstMeta, IN const char* a_pszEntryPath) {
    char * base;
    LPDRMETALIB pstLib;
    LPDRMETA pstCurMeta;
    const char * nameBegin;
    const char * nameEnd;

    base = (char *)(a_pstMeta) - a_pstMeta->m_self_pos;
    pstLib = ((LPDRMETALIB)base) - 1;

    pstCurMeta = a_pstMeta;
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

    return dr_get_entryptr_by_name(pstCurMeta, nameBegin);
}

const char *dr_get_entry_customattr(IN LPDRMETALIB a_pstLib, IN LPDRMETAENTRY a_pstEntry) {
    //TODO
}

int dr_get_macro_value(OUT int *a_piID, IN LPDRMETALIB a_pstLib, IN const  char *a_pszName) {
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

int dr_get_metalib_macro_num(IN LPDRMETALIB a_pstLib) {
    assert(a_pstLib);

    return a_pstLib->m_macro_count;
}

LPDRMACRO dr_get_metalib_macro_by_index(IN LPDRMETALIB a_pstLib, IN int a_iIdx) {
    assert(a_pstLib);

    if (a_iIdx < 0 || a_iIdx >= a_pstLib->m_macro_count) {
        return 0;
    }
    else {
        char * base = (char *)(a_pstLib + 1);
        return (struct tagDRMacro*)(base + a_pstLib->m_startpos_macro) + a_iIdx;
    }
}

const char* dr_get_macro_name_by_ptr(IN LPDRMETALIB a_pstLib, IN LPDRMACRO a_pstMacro) {
    char * base;

    assert(a_pstLib);
    base = (char *)(a_pstLib + 1);

    return (char *)base + a_pstMacro->m_name_pos;
}

int dr_get_macro_value_by_ptr(OUT int *a_piID, IN LPDRMACRO a_pstMacro) {
    assert(a_piID);
    assert(a_pstMacro);

    *a_piID = a_pstMacro->m_value;
    return 0;
}

const char* dr_get_macro_desc_by_ptr(IN LPDRMETALIB a_pstLib, IN LPDRMACRO a_pstMacro) {
    assert(a_pstLib);
    if (a_pstMacro->m_desc_pos < 0) {
        return "";
    }
    else {
        char * base = (char *)(a_pstLib + 1);
        return (char *)(base) + a_pstMacro->m_desc_pos;
    }
}

LPDRMACROSGROUP dr_get_macro_macrosgroup(IN LPDRMETALIB a_pstLib, IN LPDRMACRO a_pstMacro) {
    //TODO
}

int dr_get_macrosgroup_num(IN LPDRMETALIB a_pstLib) {
    //TODO
}

LPDRMACROSGROUP dr_get_macrosgroup_by_index(IN LPDRMETALIB a_pstLib, IN int a_iIdx) {
    //TODO
}

const char* dr_get_macrosgroup_name(IN LPDRMACROSGROUP a_pstGroup) {
    //TODO
}

int dr_get_macrosgroup_macro_num(IN LPDRMACROSGROUP a_pstGroup) {
    //TODO
}

LPDRMACRO dr_get_macrosgroup_macro_by_index(IN LPDRMETALIB a_pstLib, IN LPDRMACROSGROUP a_pstGroup, IN int a_iIdx) {
    //TODO
}
