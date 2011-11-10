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

#define DR_TYPE_BUILD_READ_INT_FUN(__bit, __min, __max)                 \
static int dr_type_read_int ## __bit ## _from_string(void * output, const char * s) { \
    int ## __bit ## _t acc;                                                        \
    int base;                                                           \
    char c;                                                             \
    uint ## __bit ## _t cutoff;                                         \
    int ## __bit ## _t neg, any, cutlim;                                \
                                                                        \
    do {                                                                \
        c = *s++;                                                       \
    } while (isspace((unsigned char)c));                                \
                                                                        \
    if (c == '-') {                                                     \
        neg = 1;                                                        \
        c = *s++;                                                       \
    }                                                                   \
    else {                                                              \
    neg = 0;                                                            \
    if (c == '+') {                                                     \
        c = *s++;                                                       \
    }                                                                   \
    }                                                                   \
                                                                        \
    base = 10;                                                          \
    if (c == '0' && (*s == 'x' || *s == 'X') &&                         \
                 ((s[1] >= '0' && s[1] <= '9') ||                       \
                  (s[1] >= 'A' && s[1] <= 'F') ||                       \
                  (s[1] >= 'a' && s[1] <= 'f')))                        \
    {                                                                   \
    c = s[1];                                                           \
    s += 2;                                                             \
    base = 16;                                                          \
    }                                                                   \
                                                                        \
    acc = any = 0;                                                      \
                                                                        \
    cutoff = neg                                                        \
        ? (uint ## __bit ## _t)-(__min + __max) + __max                 \
        : __max;                                                        \
    cutlim = cutoff % base;                                             \
    cutoff /= base;                                                     \
                                                                        \
    for ( ; ; c = *s++) {                                               \
    if (c >= '0' && c <= '9')                                           \
        c -= '0';                                                       \
    else if (c >= 'A' && c <= 'Z')                                      \
        c -= 'A' - 10;                                                  \
    else if (c >= 'a' && c <= 'z')                                      \
        c -= 'a' - 10;                                                  \
    else                                                                \
        break;                                                          \
                                                                        \
    if (c >= base) {                                                    \
        break;                                                          \
    }                                                                   \
                                                                        \
    if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {     \
        any = -1;                                                       \
    }                                                                   \
    else {                                                              \
    any = 1;                                                            \
    acc *= base;                                                        \
    acc += c;                                                           \
    }                                                                   \
    }                                                                   \
                                                                        \
    if (any < 0) { /*overflow*/                                         \
        return -1;                                                      \
    } else if (!any) { /*no data*/                                      \
        return -1;                                                      \
    }                                                                   \
                                                                        \
    if (neg) {                                                          \
        acc = -acc;                                                     \
    }                                                                   \
                                                                        \
    memcpy(output, &acc, sizeof(acc));                                  \
                                                                        \
    return *(s - 1) == 0 ? 0 : -1;                                      \
}

#define DR_TYPE_BUILD_READ_UINT_FUN(__bit, __max)                       \
    static int dr_type_read_uint ## __bit ## _from_string(void * output, const char * s) { \
        uint ## __bit ## _t acc;                                        \
        int base;                                                       \
        char c;                                                         \
        uint ## __bit ##_t cutoff;                                      \
        int ## __bit ## _t any, cutlim;                                 \
                                                                        \
        do {                                                            \
            c = *s++;                                                   \
        } while (isspace((unsigned char)c));                            \
                                                                        \
        if (c == '-') {                                                 \
            return -1;                                                  \
        }                                                               \
                                                                        \
        if (c == '+') {                                                 \
            c = *s++;                                                   \
        }                                                               \
                                                                        \
        base = 10;                                                      \
        if (c == '0' && (*s == 'x' || *s == 'X') &&                     \
            ((s[1] >= '0' && s[1] <= '9') ||                            \
             (s[1] >= 'A' && s[1] <= 'F') ||                            \
             (s[1] >= 'a' && s[1] <= 'f')))                             \
        {                                                               \
            c = s[1];                                                   \
            s += 2;                                                     \
            base = 16;                                                  \
        }                                                               \
                                                                        \
        acc = any = 0;                                                  \
                                                                        \
        cutoff = __max;                                                 \
        cutlim = cutoff % base;                                         \
        cutoff /= base;                                                 \
                                                                        \
        for ( ; ; c = *s++) {                                           \
            if (c >= '0' && c <= '9')                                   \
                c -= '0';                                               \
            else if (c >= 'A' && c <= 'Z')                              \
                c -= 'A' - 10;                                          \
            else if (c >= 'a' && c <= 'z')                              \
                c -= 'a' - 10;                                          \
            else                                                        \
                break;                                                  \
                                                                        \
            if (c >= base) {                                            \
                break;                                                  \
            }                                                           \
                                                                        \
            if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) { \
                any = -1;                                               \
            }                                                           \
            else {                                                      \
                any = 1;                                                \
                acc *= base;                                            \
                acc += c;                                               \
            }                                                           \
        }                                                               \
                                                                        \
        if (any < 0) { /*overflow*/                                     \
            return -1;                                                  \
        } else if (!any) { /*no data*/                                  \
            return -1;                                                  \
        }                                                               \
                                                                        \
        memcpy(output, &acc, sizeof(acc));                              \
                                                                        \
        return *(s - 1) == 0 ? 0 : -1;                                  \
}


DR_TYPE_BUILD_READ_INT_FUN(8, CHAR_MIN, CHAR_MAX)
DR_TYPE_BUILD_READ_UINT_FUN(8, UCHAR_MAX)
DR_TYPE_BUILD_READ_INT_FUN(16, SHRT_MIN, SHRT_MAX)
DR_TYPE_BUILD_READ_UINT_FUN(16, USHRT_MAX)
DR_TYPE_BUILD_READ_INT_FUN(32, INT_MIN, INT_MAX)
DR_TYPE_BUILD_READ_UINT_FUN(32, UINT_MAX)
DR_TYPE_BUILD_READ_INT_FUN(64, LONG_MIN, LONG_MAX)
DR_TYPE_BUILD_READ_UINT_FUN(64, ULONG_MAX)


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
