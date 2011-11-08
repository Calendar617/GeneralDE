#include <limits.h>
#include <stdio.h>
#include <strings.h>
#include "../dr_ctype_ops.h"

static int dr_type_read_char_from_string(void * output, const char * input) {
    if (input[1] != 0 || input[0] == 0) {
        return -1;
    }
    else {
        *((char*)output) = input[0];
        return 0;
    }
}

static int dr_type_read_int8_from_string(void * output, const char * input) {
    long buf;
    char * endptr;

    buf = strtol(input, &endptr, 10);
    if (*endptr != 0 || buf < SCHAR_MIN || buf > SCHAR_MAX) {
        return -1;
    }
    else {
        *((int8_t*)output) = buf;
        return 0;
    }
}

static int dr_type_read_uint8_from_string(void * output, const char * input) {
    long buf;
    char * endptr;

    buf = strtol(input, &endptr, 10);
    if (*endptr != 0 || buf < 0 || buf > UCHAR_MAX) {
        return -1;
    }
    else {
        *((uint8_t*)output) = buf;
        return 0;
    }
}

static int dr_type_read_int16_from_string(void * output, const char * input) {
    long buf;
    char * endptr;

    buf = strtol(input, &endptr, 10);
    if (*endptr != 0 || buf < SHRT_MIN || buf > SHRT_MAX) {
        return -1;
    }
    else {
        int16_t d = buf;
        memcpy(output, &d, sizeof(d));
        return 0;
    }
}

static int dr_type_read_uint16_from_string(void * output, const char * input) {
    long buf;
    char * endptr;

    buf = strtol(input, &endptr, 10);
    if (*endptr != 0 || buf < 0 || buf > USHRT_MAX) {
        return -1;
    }
    else {
        uint16_t d = buf;
        memcpy(output, &d, sizeof(d));
        return 0;
    }
}

static int dr_type_read_int32_from_string(void * output, const char * input) {
    int32_t buf;
    char * endptr;

    buf = strtol(input, &endptr, 10);
    if (*endptr != 0) {
        return -1;
    }
    else {
        int32_t d = buf;
        memcpy(output, &d, sizeof(d));
        return 0;
    }
}

static int dr_type_read_uint32_from_string(void * output, const char * input) {
    int64_t buf;
    char * endptr;

    buf = strtoll(input, &endptr, 10);
    if (*endptr != 0 || buf < 0 || buf > UINT_MAX) {
        return -1;
    }
    else {
        uint32_t d = buf;
        memcpy(output, &d, sizeof(d));
        return 0;
    }
}

static int dr_type_read_int64_from_string(void * output, const char * input) {
    int64_t buf;
    char * endptr;

    buf = strtoll(input, &endptr, 10);
    if (*endptr != 0) {
        return -1;
    }
    else {
        memcpy(output, &buf, sizeof(buf));
        return 0;
    }
}

static int dr_type_read_uint64_from_string(void * output, const char * input) {
}

struct tagDRCTypeInfo g_dr_ctypeinfos[] = {
    {CPE_DR_TYPE_UNION, "union", -1, NULL}
    , {CPE_DR_TYPE_STRUCT, "struct", -1, NULL}
    , {CPE_DR_TYPE_CHAR, "char", 1, dr_type_read_char_from_string}
    , {CPE_DR_TYPE_UCHAR, "uchar", 1, dr_type_read_char_from_string}
    , {CPE_DR_TYPE_BYTE, "byte", 1, dr_type_read_int8_from_string}
    , {CPE_DR_TYPE_SHORT, "short", 2, dr_type_read_int16_from_string}
    , {CPE_DR_TYPE_USHORT, "ushort", 2, dr_type_read_uint16_from_string}
    , {CPE_DR_TYPE_INT, "int", 4, dr_type_read_int32_from_string}
    , {CPE_DR_TYPE_UINT, "uint", 4, dr_type_read_uint32_from_string}
    , {CPE_DR_TYPE_LONG, "long", 4, dr_type_read_int32_from_string}
    , {CPE_DR_TYPE_ULONG, "ulong", 4, dr_type_read_uint32_from_string}
    , {CPE_DR_TYPE_LONGLONG, "longlong", 8, dr_type_read_int64_from_string}
    , {CPE_DR_TYPE_ULONGLONG, "ulonglong", 8, dr_type_read_int64_from_string}
    , {CPE_DR_TYPE_DATE, "date", 4, NULL}
    , {CPE_DR_TYPE_TIME, "time", 4, NULL}
    , {CPE_DR_TYPE_DATETIME, "datetime", 8, NULL}
    , {CPE_DR_TYPE_MONEY, "money", 4, NULL}
    , {CPE_DR_TYPE_FLOAT, "float", 4, NULL}
    , {CPE_DR_TYPE_DOUBLE, "double", 8, NULL}
    , {CPE_DR_TYPE_IP, "ip", 4, NULL}
    , {CPE_DR_TYPE_WCHAR, "wchar", 2, NULL}
    , {CPE_DR_TYPE_STRING, "string", -1, NULL}
    , {CPE_DR_TYPE_WSTRING, "wstring", -1, NULL}
    , {CPE_DR_TYPE_VOID, "void", -1, NULL}

    /*same name types*/
    , {CPE_DR_TYPE_SMALLINT, "smallint", 2, dr_type_read_int16_from_string}
    , {CPE_DR_TYPE_SMALLUINT, "smalluint", 2, dr_type_read_uint16_from_string}
    , {CPE_DR_TYPE_CHAR, "int8", 1, dr_type_read_int8_from_string}
    , {CPE_DR_TYPE_UCHAR, "uint8", 1, dr_type_read_uint8_from_string}
    , {CPE_DR_TYPE_CHAR, "tinyint", 1, dr_type_read_int8_from_string}
    , {CPE_DR_TYPE_UCHAR, "tinyuint", 1, dr_type_read_uint8_from_string}
    , {CPE_DR_TYPE_LONGLONG, "bigint", 8, dr_type_read_int64_from_string}
    , {CPE_DR_TYPE_ULONGLONG, "biguint", 8, dr_type_read_uint64_from_string}
    , {CPE_DR_TYPE_SHORT, "int16", 2, dr_type_read_int16_from_string}
    , {CPE_DR_TYPE_USHORT, "uint16", 2, dr_type_read_uint16_from_string}
    , {CPE_DR_TYPE_INT, "int32", 4, dr_type_read_int32_from_string}
    , {CPE_DR_TYPE_UINT, "uint32", 4, dr_type_read_uint32_from_string}
    , {CPE_DR_TYPE_LONGLONG, "int64", 8, dr_type_read_int64_from_string}
    , {CPE_DR_TYPE_ULONGLONG, "uint64", 8, dr_type_read_uint64_from_string}
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
