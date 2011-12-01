#ifndef CPE_CFG_TYPES_H
#define CPE_CFG_TYPES_H
#include "cpe/pal/types.h"
#include "cpe/dr/dr_ctypes_info.h"

typedef struct gd_cfg * gd_cfg_t;

#define CPE_CFG_TYPE_SEQUENCE    CPE_DR_TYPE_UNION
#define CPE_CFG_TYPE_STRUCT      CPE_DR_TYPE_STRUCT
#define CPE_CFG_TYPE_INT8        CPE_DR_TYPE_INT8
#define CPE_CFG_TYPE_UINT8       CPE_DR_TYPE_UINT8
#define CPE_CFG_TYPE_INT16       CPE_DR_TYPE_INT16
#define CPE_CFG_TYPE_UINT16      CPE_DR_TYPE_UINT16
#define CPE_CFG_TYPE_INT32       CPE_DR_TYPE_INT32
#define CPE_CFG_TYPE_UINT32      CPE_DR_TYPE_UINT32
#define CPE_CFG_TYPE_INT64       CPE_DR_TYPE_INT64
#define CPE_CFG_TYPE_UINT64      CPE_DR_TYPE_UINT64
#define CPE_CFG_TYPE_STRING      CPE_DR_TYPE_STRING

#endif


