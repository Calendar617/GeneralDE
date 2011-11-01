#include "../dr_ctype_ops.h"

struct tagDRCTypeInfo g_dr_ctypeinfos[] = {
    {CPE_DR_TYPE_UNION, "union", -1}
    , {CPE_DR_TYPE_STRUCT, "struct", -1}
    , {CPE_DR_TYPE_CHAR, "char", 1}
    , {CPE_DR_TYPE_UCHAR, "uchar", 1}
    , {CPE_DR_TYPE_BYTE, "byte", 1}
    , {CPE_DR_TYPE_SHORT, "short", 2}
    , {CPE_DR_TYPE_USHORT, "ushort", 2}
    , {CPE_DR_TYPE_INT, "int", 4}
    , {CPE_DR_TYPE_UINT, "uint", 4}
    , {CPE_DR_TYPE_LONG, "long", 4}
    , {CPE_DR_TYPE_ULONG, "ulong", 4}
    , {CPE_DR_TYPE_LONGLONG, "longlong", 4}
    , {CPE_DR_TYPE_ULONGLONG, "ulonglong", 4}
    , {CPE_DR_TYPE_DATE, "date", 4}
    , {CPE_DR_TYPE_TIME, "time", 4}
    , {CPE_DR_TYPE_DATETIME, "datetime", 4}
    , {CPE_DR_TYPE_MONEY, "money", 4}
    , {CPE_DR_TYPE_FLOAT, "float", 4}
    , {CPE_DR_TYPE_DOUBLE, "double", 4}
    , {CPE_DR_TYPE_IP, "ip", 4}
    , {CPE_DR_TYPE_WCHAR, "wchar", 4}
    , {CPE_DR_TYPE_STRING, "string", -1}
    , {CPE_DR_TYPE_WSTRING, "wstring", -1}
    , {CPE_DR_TYPE_VOID, "void", -1}

    /*same name types*/
    , {CPE_DR_TYPE_SMALLINT, "smallint", 2}
    , {CPE_DR_TYPE_SMALLUINT, "smalluint", 2}
};

static const int g_dr_ctypeinfos_count
= sizeof(g_dr_ctypeinfos) / sizeof(struct tagDRCTypeInfo);

const struct tagDRCTypeInfo *
dr_find_ctype_info_by_name(const char * name, size_t n) {
    int i = 0;

    if (n < 0) {
        for(i = 0; i < g_dr_ctypeinfos_count; ++i) {
            if (strcmp(g_dr_ctypeinfos[i].m_name, name) == 0) {
                return &g_dr_ctypeinfos[i];
            }
        }
    }
    else {
        for(i = 0; i < g_dr_ctypeinfos_count; ++i) {
            if (strncmp(g_dr_ctypeinfos[i].m_name, name, n) == 0
                && g_dr_ctypeinfos[i].m_name[n] == 0)
            {
                return &g_dr_ctypeinfos[i];
            }
        }
    }

    return NULL;
}

const struct tagDRCTypeInfo *
dr_find_ctype_info_by_type(int typeId) {
    if (typeId < 0 || typeId > CPE_DR_TYPE_VOID) {
        return NULL;
    }
    else {
        return &g_dr_ctypeinfos[typeId];
    }
}
