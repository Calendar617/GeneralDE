#include <ctype.h>
#include <limits.h>
#include <string.h>
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

int dr_type_printf_int8_to_stream(write_stream_t stream, const void * data) {
    return stream_printf(stream, "%d", *((int8_t*)data));
}
int dr_type_printf_int16_to_stream(write_stream_t stream, const void * data) {
    return stream_printf(stream, "%d", *((int16_t*)data));
}
int dr_type_printf_int32_to_stream(write_stream_t stream, const void * data) {
    return stream_printf(stream, "%d", *((int32_t*)data));
}
int dr_type_printf_int64_to_stream(write_stream_t stream, const void * data) {
    return stream_printf(stream, "%I64d", *((int64_t*)data));
}
int dr_type_printf_uint8_to_stream(write_stream_t stream, const void * data) {
    return stream_printf(stream, "%u", *((uint8_t*)data));
}
int dr_type_printf_uint16_to_stream(write_stream_t stream, const void * data) {
    return stream_printf(stream, "%u", *((uint16_t*)data));
}
int dr_type_printf_uint32_to_stream(write_stream_t stream, const void * data) {
    return stream_printf(stream, "%u", *((uint32_t*)data));
}
int dr_type_printf_uint64_to_stream(write_stream_t stream, const void * data) {
    return stream_printf(stream, "%I64u", *((uint64_t*)data));
}

int dr_type_printf_char_to_stream(write_stream_t stream, const void * data) {
    char tmp = *((char*)data);
    return stream_printf(stream, "%c", tmp);
}

int dr_type_printf_uchar_to_stream(write_stream_t stream, const void * data) {
    unsigned char tmp = *((unsigned char*)data);
    return stream_printf(stream, "%c", tmp);
}

struct DRTypeNameMap {
    const int m_typeId;
    const char * m_typeName;
};

struct tagDRCTypeInfo g_dr_ctypeinfos[] = {
    {CPE_DR_TYPE_UNION, "union", -1, NULL, NULL}
    , {CPE_DR_TYPE_STRUCT, "struct", -1, NULL, NULL}
    , {CPE_DR_TYPE_CHAR, "char", 1, dr_type_read_char_from_string, dr_type_printf_char_to_stream}
    , {CPE_DR_TYPE_UCHAR, "uchar", 1, dr_type_read_char_from_string, dr_type_printf_uchar_to_stream}
    , {CPE_DR_TYPE_INT8, "int8", 1, dr_type_read_int8_from_string, dr_type_printf_int8_to_stream}
    , {CPE_DR_TYPE_INT16, "int16", 2, dr_type_read_int16_from_string, dr_type_printf_int16_to_stream}
    , {CPE_DR_TYPE_UINT16, "uint16", 2, dr_type_read_uint16_from_string, dr_type_printf_uint16_to_stream}
    , {CPE_DR_TYPE_INT32, "int32", 4, dr_type_read_int32_from_string, dr_type_printf_int32_to_stream}
    , {CPE_DR_TYPE_UINT32, "uint32", 4, dr_type_read_uint32_from_string, dr_type_printf_uint32_to_stream}
    , {CPE_DR_TYPE_INT32, "int32", 4, dr_type_read_int32_from_string, dr_type_printf_int32_to_stream}
    , {CPE_DR_TYPE_UINT32, "uint32", 4, dr_type_read_uint32_from_string, dr_type_printf_uint32_to_stream}
    , {CPE_DR_TYPE_INT64, "int64", 8, dr_type_read_int64_from_string, dr_type_printf_int64_to_stream}
    , {CPE_DR_TYPE_UINT64, "uint64", 8, dr_type_read_uint64_from_string, dr_type_printf_uint64_to_stream}
    , {CPE_DR_TYPE_DATE, "date", 4, NULL, NULL}
    , {CPE_DR_TYPE_TIME, "time", 4, NULL, NULL}
    , {CPE_DR_TYPE_DATETIME, "datetime", 8, NULL, NULL}
    , {CPE_DR_TYPE_MONEY, "money", 4, NULL, NULL}
    , {CPE_DR_TYPE_FLOAT, "float", 4, NULL, NULL}
    , {CPE_DR_TYPE_DOUBLE, "double", 8, NULL, NULL}
    , {CPE_DR_TYPE_IP, "ip", 4, NULL, NULL}
    , {CPE_DR_TYPE_CHAR, "char", 1, dr_type_read_char_from_string, dr_type_printf_char_to_stream}
    , {CPE_DR_TYPE_STRING, "string", -1, NULL, NULL}
    , {CPE_DR_TYPE_STRING, "string", -1, NULL, NULL}
    , {CPE_DR_TYPE_VOID, "void", -1, NULL, NULL}
    , {CPE_DR_TYPE_UINT8, "uint8", 1, dr_type_read_uint8_from_string, dr_type_printf_uint8_to_stream}
};

static const int g_dr_ctypeinfos_count
= sizeof(g_dr_ctypeinfos) / sizeof(struct tagDRCTypeInfo);

const struct tagDRCTypeInfo *
dr_find_ctype_info_by_name(const char * name) {
    int i = 0;

    for(i = 0; i < g_dr_ctypeinfos_count; ++i) {
        if (strcmp(g_dr_ctypeinfos[i].m_name, name) == 0) {
            return &g_dr_ctypeinfos[i];
        }
    }

    return NULL;
}

const struct tagDRCTypeInfo *
dr_find_ctype_info_by_type(int typeId) {
    if (typeId < 0 || typeId > CPE_DR_TYPE_MAX) {
        return NULL;
    }
    else {
        return &g_dr_ctypeinfos[typeId];
    }
}

const char * dr_type_name(int typeId) {
    if (typeId < 0 || typeId > CPE_DR_TYPE_MAX) {
        return "unknown";
    }
    else {
        return g_dr_ctypeinfos[typeId].m_name;
    }
}

int dr_type_size(int typeId) {
    if (typeId < 0 || typeId > CPE_DR_TYPE_MAX) {
        return -1;
    }
    else {
        return g_dr_ctypeinfos[typeId].m_size;
    }
}
