#ifndef CPE_DR_TYPES_H
#define CPE_DR_TYPES_H

#include "cpe/pal/types.h"
#include "cpe/dr/dr_define.h"

/** @defgroup CPE_DR_TYPE CPE_DR_��Ҫ���Ͷ���
 *  @{
 */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#if defined(_WIN64) || (__WORDSIZE == 64)
    //64λϵͳ
    #define CPE_DR_PALTFROM_ALIGN_VALUE		8	/**<ƽ̨ȱʡ�ֽڶ���ֵ*/
    #define CPE_DR_SIZEOF_POINTER 8         /**<ָ������洢���ֽ���*/
    #define  CPE_DR_POINTER_TYPE_ALIGN 8	/*ָ���������Ͷ��뷽ʽ*/
#else
    //32λϵͳ
    #define CPE_DR_PALTFROM_ALIGN_VALUE		4	/**<ƽ̨ȱʡ�ֽڶ���ֵ*/
    #define CPE_DR_SIZEOF_POINTER 4         /**<ָ������洢���ֽ���*/
    #define  CPE_DR_POINTER_TYPE_ALIGN 4	/*ָ���������Ͷ��뷽ʽ*/
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
typedef uint16_t dr_wchar_t;  /**<Wchar������������*/
typedef uint32_t dr_date_t;	/**<data������������*/
typedef uint32_t dr_time_t;	/**<time������������*/
typedef uint64_t dr_datetime_t; /**<datetime������������*/
typedef uint32_t dr_ip_t;  /**<IPv4��������*/

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


/**����c/c++ ͷ�ļ�����ṹ����
*/
typedef struct tagDRHppRule DRHPPRULE;
typedef struct tagDRHppRule *LPDRHPPRULE;

/**��ʼ��Ԫ���ݿ�Ľṹ��
*/
struct tagDRLibParam
{
	int iID;        /**< Ԫ�����ID*/ 

	int iTagSetVersion; /**< Ԫ��������XML��ǩ���İ汾*/

	size_t iSize;		/**< Ԫ���ݵ��ڴ��С��tdr_init_lib�����ݴ����ݷ���ռ�*/
	size_t iMacrosGroupSize;      /**< �Զ�������������ռ�Ŀռ��С*/
	size_t iMetaSize;      /**< �Զ�������������ռ�Ŀռ��С*/
	size_t iStrBufSize;    /**< �ַ�����������ռ�Ĵ�С*/

	int iMaxMacros;     /**< ��Ԫ���ݿ���������궨����*/
	int iMaxMetas;	/**< ��Ԫ���ݿ���������ṹ����*/

	int iMaxMacrosGroupNum; /*��������ɵĺ궨������Ŀ, Add at DR build Version: 0x00000008*/

	int iRes;           /**< Ԥ��λ����δʹ��*/
	int  iVersion;     /**< Ԫ����İ汾*/

	char szName[CPE_DR_NAME_LEN];  /**< Ԫ���ݿ�����*/

};

/**
sizeinfo/sizetype����
*/
struct tagDRSizeInfo
{
    DROFF iNOff;   /**<���紫����Ϣƫ��*/
    DROFF iHOff;    /**<���ش洢��Ϣƫ��*/
    size_t iUnitSize;
    int idxSizeType;	/*����sizeinfo �ü����ʹ������ʱʹ�����ͣ���CPE_DR_BUILD version 5ʱ����*/
};
typedef struct tagDRSizeInfo DRSIZEINFO;
typedef struct tagDRSizeInfo *LPDRSIZEINFO;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @}*/ // CPE_DR_TYPE DR��Ҫ���Ͷ���
#endif /* CPE_DR_TYPES_H */
