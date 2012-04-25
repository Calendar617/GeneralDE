#ifndef CPE_DR_PBUF_INTERNAL_TYPES_H
#define CPE_DR_PBUF_INTERNAL_TYPES_H
#include "cpe/dr/dr_types.h"

#define CPE_PBC_ARRAY_CAP 64

#define CPE_PBC_EXIST -1
#define CPE_PBC_INT 1
#define CPE_PBC_REAL 2
#define CPE_PBC_BOOL 3
#define CPE_PBC_ENUM 4
#define CPE_PBC_STRING 5
#define CPE_PBC_MESSAGE 6
#define CPE_PBC_FIXED64 7
#define CPE_PBC_FIXED32 8
#define CPE_PBC_BYTES 9
#define CPE_PBC_INT64 10
#define CPE_PBC_UINT 11
#define CPE_PBC_REPEATED 128

#define PTYPE_DOUBLE   1
#define PTYPE_FLOAT    2
#define PTYPE_INT64    3   // Not ZigZag encoded.  Negative numbers take 10 bytes.  Use TYPE_SINT64 if negative values are likely.
#define PTYPE_UINT64   4
#define PTYPE_INT32    5   // Not ZigZag encoded.  Negative numbers take 10 bytes.  Use TYPE_SINT32 if negative values are likely.
#define PTYPE_FIXED64  6
#define PTYPE_FIXED32  7
#define PTYPE_BOOL     8
#define PTYPE_STRING   9
#define PTYPE_GROUP    10  // Tag-delimited aggregate.
#define PTYPE_MESSAGE  11  // Length-delimited aggregate.
#define PTYPE_BYTES    12
#define PTYPE_UINT32   13
#define PTYPE_ENUM     14
#define PTYPE_SFIXED32 15
#define PTYPE_SFIXED64 16
#define PTYPE_SINT32   17  // Uses ZigZag encoding.
#define PTYPE_SINT64   18  // Uses ZigZag encoding.

#endif
