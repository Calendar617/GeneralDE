#ifndef CPE_DR_TYPES_H
#define CPE_DR_TYPES_H
#include "cpe/pal/pal_types.h"
#include "cpe/dr/dr_define.h"

#ifdef __cplusplus
extern "C" {
#endif

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

typedef enum dr_metalib_source_type {
    dr_metalib_source_type_file
    , dr_metalib_source_type_memory
} dr_metalib_source_type_t;

typedef enum dr_metalib_source_format {
    dr_metalib_source_format_xml
} dr_metalib_source_format_t;

typedef struct dr_metalib_source * dr_metalib_source_t;
typedef struct dr_metalib_builder * dr_metalib_builder_t;
struct DRInBuildMetaLib;

#ifdef __cplusplus
}
#endif

#endif
