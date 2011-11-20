#include <ctype.h>
#include <limits.h>
#include <string.h>
#include "cpe/dr/dr_data.h"
#include "../dr_internal_types.h"

struct DRCtypeTypeFromStringOps {
    int (* set_from_string)(void * output, LPDRMETAENTRY entry, const char * input, error_monitor_t em);
};

static int dr_set_char_from_string(void * output, LPDRMETAENTRY entry, const char * input, error_monitor_t em) {
    if (input[1] != 0 || input[0] == 0) {
        return -1;
    }
    else {
        *((char*)output) = input[0];
        return 0;
    }
}

static int dr_set_string_from_string(void * output, LPDRMETAENTRY entry, const char * input, error_monitor_t em) {
    size_t len;
    if (entry == NULL) {
        return -1;
    }

    len = strlen(input);
    if (len + 1 > entry->m_unitsize) {
        len = entry->m_unitsize - 1;
    }

    memcpy(output, input, len);
    *((char *)output + len) = 0;
    return 0;
}

#define DR_TYPE_BUILD_READ_INT_FUN(__bit, __min, __max)                 \
static int dr_set_int ## __bit ## _from_string(void * output, LPDRMETAENTRY entry, const char * s, error_monitor_t em) { \
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
static int dr_set_uint ## __bit ## _from_string(                        \
    void * output, LPDRMETAENTRY entry,                                 \
    const char * s, error_monitor_t em)                                 \
{                                                                       \
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


struct DRCtypeTypeFromStringOps g_dr_from_string_ops[] = {
    {/*CPE_DR_TYPE_UNION*/ NULL}
    , {/*CPE_DR_TYPE_STRUCT*/ NULL}
    , {/*CPE_DR_TYPE_CHAR*/ dr_set_char_from_string}
    , {/*CPE_DR_TYPE_UCHAR*/ dr_set_char_from_string}
    , {/*CPE_DR_TYPE_INT8*/ dr_set_int8_from_string}
    , {/*CPE_DR_TYPE_INT16*/ dr_set_int16_from_string}
    , {/*CPE_DR_TYPE_UINT16*/ dr_set_uint16_from_string}
    , {/*CPE_DR_TYPE_INT32*/ dr_set_int32_from_string}
    , {/*CPE_DR_TYPE_UINT32*/ dr_set_uint32_from_string}
    , {/*CPE_DR_TYPE_INT32*/ dr_set_int32_from_string}
    , {/*CPE_DR_TYPE_UINT32*/ dr_set_uint32_from_string}
    , {/*CPE_DR_TYPE_INT64*/ dr_set_int64_from_string}
    , {/*CPE_DR_TYPE_UINT64*/ dr_set_uint64_from_string}
    , {/*CPE_DR_TYPE_DATE*/ NULL}
    , {/*CPE_DR_TYPE_TIME*/ NULL}
    , {/*CPE_DR_TYPE_DATETIME*/ NULL}
    , {/*CPE_DR_TYPE_MONEY*/ NULL}
    , {/*CPE_DR_TYPE_FLOAT*/ NULL}
    , {/*CPE_DR_TYPE_DOUBLE*/ NULL}
    , {/*CPE_DR_TYPE_IP*/ NULL}
    , {/*CPE_DR_TYPE_CHAR*/ dr_set_char_from_string}
    , {/*CPE_DR_TYPE_STRING*/ dr_set_string_from_string}
    , {/*CPE_DR_TYPE_STRING*/ dr_set_string_from_string}
    , {/*CPE_DR_TYPE_VOID*/ NULL}
    , {/*CPE_DR_TYPE_UINT8*/ dr_set_uint8_from_string}
};

int dr_set_from_string(void * output, LPDRMETAENTRY entry, const char * input, error_monitor_t em) {
    if (entry == NULL) {
        return -1;
    }

    if (entry->m_type < 0 || entry->m_type > sizeof(g_dr_from_string_ops) / sizeof(struct DRCtypeTypeFromStringOps) ) {
        CPE_ERROR(em, "set %d from string, type is unknown", entry->m_type);
        return -1;
    }

    if (g_dr_from_string_ops[entry->m_type].set_from_string) {
        return g_dr_from_string_ops[entry->m_type].set_from_string(output, entry, input, em);
    }
    else {
        CPE_ERROR(em, "set %d from string, type not support", entry->m_type);
        return -1;
    }
}

int dr_ctype_set_from_string(void * output, int type, const char * input, error_monitor_t em) {
    if (type < 0 || type > sizeof(g_dr_from_string_ops) / sizeof(struct DRCtypeTypeFromStringOps) ) {
        CPE_ERROR(em, "set %d from string, type is unknown", type);
        return -1;
    }

    if (g_dr_from_string_ops[type].set_from_string) {
        return g_dr_from_string_ops[type].set_from_string(output, NULL, input, em);
    }
    else {
        return -1;
    }
}