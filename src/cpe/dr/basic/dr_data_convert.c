#include <limits.h>
#include "cpe/dr/dr_ctypes_op.h"
#include "../dr_ctype_ops.h"


struct DRCtypeTypeReadOps {
    int (*to_int8)(int8_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
    int (*to_uint8)(uint8_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
    int (*to_int16)(int16_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
    int (*to_uint16)(uint16_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
    int (*to_int32)(int32_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
    int (*to_uint32)(uint32_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
    int (*to_int64)(int64_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
    int (*to_uint64)(uint64_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
};

#define DEF_CVT_FUN_ASSIGN_CHECK_NONE(__from, __to)                     \
int __from ## _to_ ## __to(__to ## _t * result, const void * input,     \
                               LPDRMETAENTRY entry, error_monitor_t em) { \
    *result = *(const __from ## _t *)input;                             \
    return 0;                                                           \
}

#define DEF_CVT_FUN_ASSIGN_CHECK_MIN(__from, __to, __min, __valuefmt)   \
int __from ## _to_ ## __to(__to ## _t * result, const void * input,     \
                           LPDRMETAENTRY entry, error_monitor_t em) {   \
    int r = 0;                                                          \
    __from ## _t tmp = *(const __from ## _t *)input;                    \
    if (tmp < __min) {                                                  \
        CPE_ERROR(em, "convert %s to %s, value(" __valuefmt ") "        \
                  "less than " __valuefmt,                              \
                  #__from, #__to, tmp, __min);                          \
        r = -1;                                                         \
    }                                                                   \
    *result = *(const __from ## _t *)input;                             \
    return r;                                                           \
}

#define DEF_CVT_FUN_ASSIGN_CHECK_RANGE(__from, __to, __min, __max, __valuefmt) \
int __from ## _to_ ## __to(__to ## _t * result, const void * input,     \
                           LPDRMETAENTRY entry, error_monitor_t em) {   \
    int r = 0;                                                          \
    __from ## _t tmp = *(const __from ## _t *)input;                    \
    if (tmp < __min) {                                                  \
        CPE_ERROR(em, "convert %s to %s, value(" __valuefmt ") "        \
                  "less than " __valuefmt,                              \
                  #__from, #__to, tmp, __min);                          \
        r = -1;                                                         \
    }                                                                   \
    if (tmp > __max) {                                                  \
        CPE_ERROR(em, "convert %s to %s, value(" __valuefmt ") "        \
                  "bigger than " __valuefmt,                            \
                  #__from, #__to, tmp, __max);                          \
        r = -1;                                                         \
    }                                                                   \
    *result = *(const __from ## _t *)input;                             \
    return r;                                                           \
}

#define DEF_CVT_FUN_ASSIGN_CHECK_MAX(__from, __to, __max, __valuefmt)   \
int __from ## _to_ ## __to(__to ## _t * result, const void * input,     \
                           LPDRMETAENTRY entry, error_monitor_t em) {   \
    int r = 0;                                                          \
    __from ## _t tmp = *(const __from ## _t *)input;                    \
    if (tmp > __max) {                                                  \
        CPE_ERROR(em, "convert %s to %s, value(" __valuefmt ") "        \
                  "bigger than " __valuefmt,                            \
                  #__from, #__to, tmp, __max);                          \
        r = -1;                                                         \
    }                                                                   \
    *result = *(const __from ## _t *)input;                             \
    return r;                                                           \
}

DEF_CVT_FUN_ASSIGN_CHECK_NONE(int8, int8);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int8, uint8, 0, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(int8, int16);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int8, uint16, 0, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(int8, int32);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int8, uint32, 0, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(int8, int64);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int8, uint64, 0, "%d");

DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint8, int8, SCHAR_MAX, "%u");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint8, uint8);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint8, int16);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint8, uint16);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint8, int32);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint8, uint32);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint8, int64);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint8, uint64);

DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int16, int8, SCHAR_MIN, SCHAR_MAX, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int16, uint8, 0, UCHAR_MAX, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(int16, int16);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int16, uint16, 0, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(int16, int32);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int16, uint32, 0, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(int16, int64);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int16, uint64, 0, "%d");

DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint16, int8, SCHAR_MAX, "%u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint16, uint8, UCHAR_MAX, "%u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint16, int16, SHRT_MAX, "%u");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint16, uint16);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint16, int32);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint16, uint32);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint16, int64);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint16, uint64);

DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int32, int8, SCHAR_MIN, SCHAR_MAX, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int32, uint8, 0, UCHAR_MAX, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int32, int16, SHRT_MIN, SHRT_MAX, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int32, uint16, 0, USHRT_MAX, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(int32, int32);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int32, uint32, 0, "%d");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(int32, int64);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int32, uint64, 0, "%d");

DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint32, int8, SCHAR_MAX, "%u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint32, uint8, UCHAR_MAX, "%u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint32, int16, SHRT_MAX, "%u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint32, uint16, USHRT_MAX, "%u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint32, int32, INT_MAX, "%u");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint32, uint32);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint32, int64);
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint32, uint64);

DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int64, int8, SCHAR_MIN, SCHAR_MAX, "%I64d");
DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int64, uint8, 0, UCHAR_MAX, "%I64d");
DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int64, int16, SHRT_MIN, SHRT_MAX, "%I64d");
DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int64, uint16, 0, USHRT_MAX, "%I64d");
DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int64, int32, INT_MIN, INT_MAX, "%I64d");
DEF_CVT_FUN_ASSIGN_CHECK_RANGE(int64, uint32, 0, UINT_MAX, "%I64d");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(int64, int64);
DEF_CVT_FUN_ASSIGN_CHECK_MIN(int64, uint64, 0, "%I64d");

DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint64, int8, SCHAR_MAX, "%I64u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint64, uint8, UCHAR_MAX, "%I64u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint64, int16, SHRT_MAX, "%I64u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint64, uint16, USHRT_MAX, "%I64u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint64, int32, INT_MAX, "%I64u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint64, uint32, UINT_MAX, "%I64u");
DEF_CVT_FUN_ASSIGN_CHECK_MAX(uint64, int64, 9223372036854775807LL, "%I64u");
DEF_CVT_FUN_ASSIGN_CHECK_NONE(uint64, uint64);

struct DRCtypeTypeReadOps g_dr_ctype_read_ops[] = {
     /*CPE_DR_TYPE_UNION*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_STRUCT*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_CHAR*/
    { int8_to_int8, int8_to_uint8
      , int8_to_int16, int8_to_uint16
      , int8_to_int32, int8_to_uint32
      , int8_to_int64, int8_to_uint64
    }
    , /*CPE_DR_TYPE_UCHAR*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_INT8*/
    { int8_to_int8, int8_to_uint8
      , int8_to_int16, int8_to_uint16
      , int8_to_int32, int8_to_uint32
      , int8_to_int64, int8_to_uint64
    }
    , /*CPE_DR_TYPE_INT16*/
    { int16_to_int8, int16_to_uint8
      , int16_to_int16, int16_to_uint16
      , int16_to_int32, int16_to_uint32
      , int16_to_int64, int16_to_uint64
    }
    , /*CPE_DR_TYPE_UINT16*/
    { uint16_to_int8, uint16_to_uint8
      , uint16_to_int16, uint16_to_uint16
      , uint16_to_int32, uint16_to_uint32
      , uint16_to_int64, uint16_to_uint64
    }
    , /*CPE_DR_TYPE_INT32*/
    { int32_to_int8, int32_to_uint8
      , int32_to_int16, int32_to_uint16
      , int32_to_int32, int32_to_uint32
      , int32_to_int64, int32_to_uint64
    }
    , /*CPE_DR_TYPE_UINT32*/
    { uint32_to_int8, uint32_to_uint8
      , uint32_to_int16, uint32_to_uint16
      , uint32_to_int32, uint32_to_uint32
      , uint32_to_int64, uint32_to_uint64
    }
    , /*CPE_DR_TYPE_INT32*/
    { int32_to_int8, int32_to_uint8
      , int32_to_int16, int32_to_uint16
      , int32_to_int32, int32_to_uint32
      , int32_to_int64, int32_to_uint64
    }
    , /*CPE_DR_TYPE_UINT32*/
    { uint32_to_int8, uint32_to_uint8
      , uint32_to_int16, uint32_to_uint16
      , uint32_to_int32, uint32_to_uint32
      , uint32_to_int64, uint32_to_uint64
    }
    , /*CPE_DR_TYPE_INT64*/
    { int64_to_int8, int64_to_uint8
      , int64_to_int16, int64_to_uint16
      , int64_to_int32, int64_to_uint32
      , int64_to_int64, int64_to_uint64
    }
    , /*CPE_DR_TYPE_UINT64*/
    { uint64_to_int8, uint64_to_uint8
      , uint64_to_int16, uint64_to_uint16
      , uint64_to_int32, uint64_to_uint32
      , uint64_to_int64, uint64_to_uint64
    }
    , /*CPE_DR_TYPE_DATE*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_TIME*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_DATETIME*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_MONEY*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_FLOAT*/
    { int8_to_int8, int8_to_uint8
      , int8_to_int16, int8_to_uint16
      , int8_to_int32, int8_to_uint32
      , int8_to_int64, int8_to_uint64
    }
    , /*CPE_DR_TYPE_DOUBLE*/
    { int8_to_int8, int8_to_uint8
      , int8_to_int16, int8_to_uint16
      , int8_to_int32, int8_to_uint32
      , int8_to_int64, int8_to_uint64
    }
    , /*CPE_DR_TYPE_IP*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_WCHAR*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_STRING*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_WSTRING*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_VOID*/
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
    , /*CPE_DR_TYPE_UINT8*/
    { uint8_to_int8, uint8_to_uint8
      , uint8_to_int16, uint8_to_uint16
      , uint8_to_int32, uint8_to_uint32
      , uint8_to_int64, uint8_to_uint64
    }
};

#define CPE_READOPS_COUNT (sizeof(g_dr_ctype_read_ops) / sizeof(struct DRCtypeTypeReadOps))

#define CPE_DEF_READ_FUN(__to)                                          \
    int dr_ctype_try_read_ ## __to(                                     \
        __to ## _t * result,                                            \
        const void * input, int type, error_monitor_t em)               \
    {                                                                   \
        if (type < 0 || type > CPE_READOPS_COUNT) {                     \
            CPE_ERROR(em, "read from %d, type is unknown", type);       \
            return -1;                                                  \
        }                                                               \
        else {                                                          \
            if (g_dr_ctype_read_ops[type].to_ ## __to) {                \
                return g_dr_ctype_read_ops[type].to_ ## __to (          \
                    result, input, NULL, em);                           \
            }                                                           \
            else {                                                      \
                CPE_ERROR(em, "read from %d, type not support to "      \
                          #__to, type);                                 \
                return -1;                                              \
            }                                                           \
        }                                                               \
    }                                                                   \
    __to ## _t dr_ctype_read_ ## __to(const void * input, int type) {   \
        if (type < 0 || type > CPE_READOPS_COUNT) {                     \
            return (__to ## _t)0;                                       \
        }                                                               \
        else {                                                          \
            if (g_dr_ctype_read_ops[type].to_ ## __to) {                \
                __to ## _t tmp = 0;                                     \
                g_dr_ctype_read_ops[type].to_ ## __to (                 \
                    &tmp, input, NULL, NULL);                           \
                return tmp;                                             \
            }                                                           \
            else {                                                      \
                return (__to ## _t)0;                                   \
            }                                                           \
        }                                                               \
    }                                                                   \
    int dr_try_read_ ## __to(                                           \
        __to ## _t * result,                                            \
        const void * input, LPDRMETAENTRY entry, error_monitor_t em)    \
    {                                                                   \
        if (entry == NULL) {                                            \
            return -1;                                                  \
        }                                                               \
        if (entry->m_type < 0 || entry->m_type > CPE_READOPS_COUNT) {   \
            CPE_ERROR(em, "read from %d, type is unknown",              \
                      entry->m_type);                                   \
            return -1;                                                  \
        }                                                               \
        else {                                                          \
            if (g_dr_ctype_read_ops[entry->m_type].to_ ## __to) {       \
                return g_dr_ctype_read_ops[entry->m_type].to_ ## __to ( \
                    result, input, entry, em);                          \
            }                                                           \
            else {                                                      \
                CPE_ERROR(em, "read from %d, type not support to "      \
                          #__to, entry->m_type);                        \
                return -1;                                              \
            }                                                           \
        }                                                               \
    }                                                                   \
    __to ## _t dr_read_ ## __to(                                        \
        const void * input, LPDRMETAENTRY entry)                        \
    {                                                                   \
        if (entry == NULL ||                                            \
            entry->m_type < 0 || entry->m_type > CPE_READOPS_COUNT) {   \
            return (__to ## _t)0;                                       \
        }                                                               \
        else {                                                          \
            if (g_dr_ctype_read_ops[entry->m_type].to_ ## __to) {       \
                __to ## _t tmp = 0;                                     \
                g_dr_ctype_read_ops[entry->m_type].to_ ## __to (        \
                    &tmp, input, entry, NULL);                          \
                return tmp;                                             \
            }                                                           \
            else {                                                      \
                return (__to ## _t)0;                                   \
            }                                                           \
        }                                                               \
    }                                                                   \


CPE_DEF_READ_FUN(int8);
CPE_DEF_READ_FUN(uint8);
CPE_DEF_READ_FUN(int16);
CPE_DEF_READ_FUN(uint16);
CPE_DEF_READ_FUN(int32);
CPE_DEF_READ_FUN(uint32);
CPE_DEF_READ_FUN(int64);
CPE_DEF_READ_FUN(uint64);
