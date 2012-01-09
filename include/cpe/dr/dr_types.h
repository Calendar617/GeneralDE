#ifndef CPE_DR_TYPES_H
#define CPE_DR_TYPES_H
#include "cpe/pal/types.h"
#include "cpe/dr/dr_define.h"

#ifdef __cplusplus
extern "C" {
#endif


#if defined(_WIN64) || (__WORDSIZE == 64)
    //64位系统
    #define CPE_DR_PALTFROM_ALIGN_VALUE		8
    #define CPE_DR_SIZEOF_POINTER 8
    #define  CPE_DR_POINTER_TYPE_ALIGN 8
#else
    //32位系统
    #define CPE_DR_PALTFROM_ALIGN_VALUE
    #define CPE_DR_SIZEOF_POINTER 4
    #define CPE_DR_POINTER_TYPE_ALIGN 4
#endif


#if defined(WIN32) &&  _MSC_VER < 1300
#define CPE_DR_OS_SWAP64(x) \
    ((((x) & (uint64_t)0xff00000000000000) >> 56)                                   \
    | (((x) & (uint64_t)0x00ff000000000000) >> 40)                                 \
    | (((x) & (uint64_t)0x0000ff0000000000) >> 24)                                 \
    | (((x) & (uint64_t)0x000000ff00000000) >> 8)                                  \
    | (((x) & (uint64_t)0x00000000ff000000) << 8)                                  \
    | (((x) & (uint64_t)0x0000000000ff0000) << 24)                                 \
    | (((x) & (uint64_t)0x000000000000ff00) << 40)                                 \
    | (((x) & (uint64_t)0x00000000000000ff) << 56))
#else
#define CPE_DR_OS_SWAP64(x) \
    ((((x) & (uint64_t)0xff00000000000000LL) >> 56)                                   \
    | (((x) & (uint64_t)0x00ff000000000000LL) >> 40)                                 \
    | (((x) & (uint64_t)0x0000ff0000000000LL) >> 24)                                 \
    | (((x) & (uint64_t)0x000000ff00000000LL) >> 8)                                  \
    | (((x) & (uint64_t)0x00000000ff000000) << 8)                                  \
    | (((x) & (uint64_t)0x0000000000ff0000) << 24)                                 \
    | (((x) & (uint64_t)0x000000000000ff00) << 40)                                 \
    | (((x) & (uint64_t)0x00000000000000ff) << 56))

#endif /*#if defined(WIN32) &&  _MSC_VER < 1300*/


#define CPE_DR_OS_SWAP32(x) \
    ((((x) & 0xff000000) >> 24)                                  \
    | (((x) & 0x00ff0000) >> 8)                                 \
    | (((x) & 0x0000ff00) << 8)                                 \
    | (((x) & 0x000000ff) << 24))

#define CPE_DR_OS_SWAP16(x) \
    ((((x) & 0xff00) >> 8)                                  \
    | (((x) & 0x00ff) << 8))


typedef int64_t dr_longlong;
typedef uint64_t dr_ulonglong;
typedef uint16_t dr_wchar_t;
typedef uint32_t dr_date_t;
typedef uint32_t dr_time_t;
typedef uint64_t dr_datetime_t;
typedef uint32_t dr_ip_t;

typedef struct tagDRMetaLib	*LPDRMETALIB;
typedef struct tagDRLibParam	DRLIBPARAM;
typedef struct tagDRLibParam	*LPDRLIBPARAM;
typedef struct tagDRMeta	*LPDRMETA;
typedef struct tagDRMetaEntry	*LPDRMETAENTRY;
typedef struct tagDRData	DRDATA;
typedef struct tagDRData	*LPDRDATA;
typedef struct tagDRMacro		*LPDRMACRO;
typedef struct tagDRMacrosGroup		*LPDRMACROSGROUP;


struct tagDRLibParam
{
	int iID;

	int iTagSetVersion;

	size_t iSize;
	size_t iMacrosGroupSize;
	size_t iMetaSize;
	size_t iStrBufSize;

	int iMaxMacros;
	int iMaxMetas;

	int iMaxMacrosGroupNum;

	int iRes;
	int  iVersion;

	char szName[CPE_DR_NAME_LEN];
};

#ifdef __cplusplus
}
#endif

#endif
