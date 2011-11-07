#ifndef CPE_DR_TYPES_H
#define CPE_DR_TYPES_H

#include "cpe/pal/types.h"
#include "cpe/dr/dr_define.h"

/** @defgroup CPE_DR_TYPE CPE_DR_主要类型定义
 *  @{
 */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#if defined(_WIN64) || (__WORDSIZE == 64)
    //64位系统
    #define CPE_DR_PALTFROM_ALIGN_VALUE		8	/**<平台缺省字节对齐值*/
    #define CPE_DR_SIZEOF_POINTER 8         /**<指针变量存储的字节数*/
    #define  CPE_DR_POINTER_TYPE_ALIGN 8	/*指针数据类型对齐方式*/
#else
    //32位系统
    #define CPE_DR_PALTFROM_ALIGN_VALUE		4	/**<平台缺省字节对齐值*/
    #define CPE_DR_SIZEOF_POINTER 4         /**<指针变量存储的字节数*/
    #define  CPE_DR_POINTER_TYPE_ALIGN 4	/*指针数据类型对齐方式*/
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
typedef uint16_t dr_wchar_t;  /**<Wchar基本数据类型*/
typedef uint32_t dr_date_t;	/**<data基本数据类型*/
typedef uint32_t dr_time_t;	/**<time基本数据类型*/
typedef uint64_t dr_datetime_t; /**<datetime基本数据类型*/
typedef uint32_t dr_ip_t;  /**<IPv4数据类型*/

/** a offset-alike type   */
typedef intptr_t DRPTR;  

/** a index-alike type   */  
typedef int DRIDX;     

/**a offset-alike type, describe the position in a meta: struct or union */
typedef ptrdiff_t DROFF;


/**
 * This is the pointer type delcaration for DR MetaLib.
 */
typedef struct tagDRMetaLib	*LPDRMETALIB;


/**
 * This is the type delcaration for DR MetaLib param.
 */
typedef struct tagDRLibParam	DRLIBPARAM;

/**
 * This is the pointer type delcaration for DR MetaLib param.
 */
typedef struct tagDRLibParam	*LPDRLIBPARAM;




/**
 * This is the pointer type delcaration for DR Meta.
 */
typedef struct tagDRMeta	*LPDRMETA;



/**
 * This is the pointer type delcaration for DR MetaEntry.
 */
typedef struct tagDRMetaEntry	*LPDRMETAENTRY;




/**
 * This is the type delcaration for DR DRData.
 */
typedef struct tagDRData	DRDATA;
/**
 * This is the pointer type delcaration for DR DRData.
 */
typedef struct tagDRData	*LPDRDATA;


/**
 * This is the pointer type delcaration for DR Macro.
 */
typedef struct tagDRMacro		*LPDRMACRO;


/**
 * This is the pointer type delcaration for DR Macro.
 */
typedef struct tagDRMacrosGroup		*LPDRMACROSGROUP;


/**生成c/c++ 头文件规则结构声明
*/
typedef struct tagDRHppRule DRHPPRULE;
typedef struct tagDRHppRule *LPDRHPPRULE;

/**初始化元数据库的结构体
*/
struct tagDRLibParam
{
	int iID;        /**< 元数库的ID*/ 

	int iTagSetVersion; /**< 元数据描述XML标签集的版本*/

	size_t iSize;		/**< 元数据的内存大小，tdr_init_lib将根据此数据分配空间*/
	size_t iMacrosGroupSize;      /**< 自定义数据类型所占的空间大小*/
	size_t iMetaSize;      /**< 自定义数据类型所占的空间大小*/
	size_t iStrBufSize;    /**< 字符串缓冲区所占的大小*/

	int iMaxMacros;     /**< 此元数据库包含的最大宏定义数*/
	int iMaxMetas;	/**< 此元数据库包含的最大结构体数*/

	int iMaxMacrosGroupNum; /*最多能容纳的宏定义组数目, Add at DR build Version: 0x00000008*/

	int iRes;           /**< 预留位，暂未使用*/
	int  iVersion;     /**< 元数库的版本*/

	char szName[CPE_DR_NAME_LEN];  /**< 元数据库名字*/

};

/**
sizeinfo/sizetype属性
*/
struct tagDRSizeInfo
{
    DROFF iNOff;   /**<网络传输信息偏移*/
    DROFF iHOff;    /**<本地存储信息偏移*/
    size_t iUnitSize;
    int idxSizeType;	/*保存sizeinfo 用简单类型打包方法时使用类型，在CPE_DR_BUILD version 5时加入*/
};
typedef struct tagDRSizeInfo DRSIZEINFO;
typedef struct tagDRSizeInfo *LPDRSIZEINFO;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @}*/ // CPE_DR_TYPE DR主要类型定义
#endif /* CPE_DR_TYPES_H */
