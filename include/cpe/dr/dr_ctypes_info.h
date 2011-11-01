#ifndef CPE_DR_CTYPES_H
#define CPE_DR_CTYPES_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cpe/pal/socket.h"
#include "cpe/dr/dr_define.h"
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_external.h"

/** @defgroup CPE_DR_CTYPE_INFO CPE_DR_基本数据类型处理
*  @{
*/

/** @name 基本数据类型ID
*  @{
*/
#define CPE_DR_TYPE_UNKOWN         -1
#define CPE_DR_TYPE_MIN		0x00
#define CPE_DR_TYPE_UNION		CPE_DR_TYPE_MIN	
#define CPE_DR_TYPE_STRUCT		0x01
#define	CPE_DR_TYPE_COMPOSITE	CPE_DR_TYPE_STRUCT
#define CPE_DR_TYPE_CHAR		0x02
#define CPE_DR_TYPE_UCHAR		0x03
#define CPE_DR_TYPE_BYTE		0x04
#define CPE_DR_TYPE_SMALLINT		0x05
#define CPE_DR_TYPE_SHORT		0x05
#define CPE_DR_TYPE_SMALLUINT		0x06
#define CPE_DR_TYPE_USHORT		0x06
#define CPE_DR_TYPE_INT		0x07
#define CPE_DR_TYPE_UINT		0x08
#define CPE_DR_TYPE_LONG		0x09
#define CPE_DR_TYPE_ULONG		0x0a
#define CPE_DR_TYPE_LONGLONG	0x0b
#define CPE_DR_TYPE_ULONGLONG	0x0c
#define CPE_DR_TYPE_DATE		0x0d
#define CPE_DR_TYPE_TIME		0x0e
#define CPE_DR_TYPE_DATETIME	0x0f
#define CPE_DR_TYPE_MONEY		0x10
#define CPE_DR_TYPE_FLOAT		0x11
#define CPE_DR_TYPE_DOUBLE		0x12
#define CPE_DR_TYPE_IP			0x13
#define CPE_DR_TYPE_WCHAR		0x14
#define CPE_DR_TYPE_STRING		0x15
#define CPE_DR_TYPE_WSTRING	0x16
#define CPE_DR_TYPE_VOID		0x17
#define CPE_DR_TYPE_MAX		CPE_DR_TYPE_VOID
/**   @}*/



/**
*将numbers-and-dots 格式的IP地址串转换成dr自定义格式的IP地址数据类型
*@param[out]	a_piTDRIp 保存dr定义的IP地址数据的指针
*@param[in]	a_pszIP 存储numbers-and-dots 格式的IP地址串的指针
*
*@retval 0   处理成功
*@retval  non-zero  处理失败
*
* @pre \e a_piTDRIp 不能为 NULL
* @pre \e a_pszIP 不能为 NULL
*
*/
CPE_DR_API int dr_ineta_to_drip(OUT dr_ip_t *a_piTDRIp, IN const char *a_pszIP);

/**
*将dr自定义格式的IP地址数据转换成struct in_addr结构数据
*@param[out]	a_pInp 输出struct in_addr结构的数据
*@param[in]	a_iTDRIp dr自定义格式的IP地址数据
*
*@retval 0   处理成功
*@retval  non-zero  处理失败
*
* @pre \e a_pInp 不能为 NULL
*
*/
CPE_DR_API int dr_drip_to_inetaddr(OUT struct in_addr *a_pInp, IN dr_ip_t a_iTDRIp);

/**
*将dr自定义格式的IP地址数据转换成numbers-and-dots 格式的IP地址串
*@param[in]	a_iTDRIp dr自定义格式的IP地址数据
*
*@return 如果处理成功,返回ip地址串首指针,否则返回NULL
*@note IP地址串保存在dr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*/
CPE_DR_API char *dr_drip_to_ineta(IN dr_ip_t a_iTDRIp);

/**
*将dr自定义格式的IP地址数据转换成numbers-and-dots 格式的IP地址串
*@param[in]	a_iTDRIp dr自定义格式的IP地址数据
*@param[in] a_pszDstBuff	保存IP地址信息的字符串缓冲区
*@param[in] a_iBuffSize 缓冲区的可用大小
*
*@return 如果处理成功,返回a_pszDstBuff的指针,否则返回NULL

*
*/
CPE_DR_API const char *dr_drip_ntop(IN dr_ip_t a_iTDRIp, IN char *a_pszDstBuff, IN int a_iBuffSize);
/**
*将'YYYY-MM-DD' 格式的date字符串转换成dr自定义格式的日期数据
*@param[out]	a_piTdrDate 保存dr定义的日期数据的指针
*@param[in]	a_pszDate 日期字符串
*
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrDate 不能为NULL
*@pre \e a_pszDate 不能为NULL
*/	
CPE_DR_API int dr_str_to_drdate(OUT dr_date_t *a_piTdrDate, IN const char *a_pszDate);

/**将dr自定义格式的日期数据转换成'YYYY-MM-DD' 格式的date字符串
*@param[in]	a_piTdrDate 保存dr定义的日期数据的指针
*
*@return 如果处理成功,返回日期字符串的指针,否则返回空串
*@note 返回的日期字符串保存在dr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrDate 不能为NULL
*
*/	
CPE_DR_API char *dr_drdate_to_str(IN const dr_date_t *a_piTdrDate);

/**将dr自定义格式的日期数据转换成'YYYY-MM-DD' 格式的date字符串 本接口是线程安全版本
*@param[in]	a_piTdrDate 保存dr定义的日期数据的指针
*@param[in] a_pszDstBuf	保存时间信息的字符串缓冲区
*@param[in] a_iBuffSize 缓冲区的可用大小
*
*@return 如果处理成功,返回日期字符串的指针,否则返回空串
*@note 返回的日期字符串保存在dr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrDate 不能为NULL
*
*/	
CPE_DR_API const char *dr_drdate_to_str_r(IN const dr_date_t *a_piTdrDate, 
										 IN char *a_pszDstBuf, IN int a_iBuffSize);


/**
*将'HHH:MM:SS' 格式的time字符串转换成dr自定义格式的时间数据
*@param[out]	a_piTdrTime 保存dr定义的时间time数据
*@param[in]	a_pszTime 时间字符串
*
*@note 支持3个字符来表示小时值的目的: time数据类型不仅可以用于表示一天的时间(这一定不会超过 24 小时)，而且可以用来表示所经过的时间或两个事件之间的时间间隔(这可能比 24 小时大许多或是一个负值)
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrTime 不能为NULL
*@pre \e a_pszTime 不能为NULL
*/	
CPE_DR_API int dr_str_to_drtime(OUT dr_time_t *a_piTdrTime, IN const char *a_pszTime);

/**
*将dr自定义格式的时间数据转换成'HHH:MM:SS' 格式的time字符串
*@param[in]	a_piTdrTime 保存dr定义的时间数据的指针
*
*@return 如果处理成功,返回时间字符串的指针,否则返回空串
*@note 返回的时间字符串保存在dr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrTime 不能为NULL
*
*/	
CPE_DR_API char *dr_drtime_to_str(IN dr_time_t *a_piTdrTime);

/**
*将dr自定义格式的时间数据转换成'HHH:MM:SS' 格式的time字符串.线程安全版本
*@param[in]	a_piTdrTime 保存dr定义的时间数据的指针
*@param[in] a_pszDstBuf	保存时间信息的字符串缓冲区
*@param[in] a_iBuffSize 缓冲区的可用大小
*
*@return 如果处理成功,返回时间字符串的指针,否则返回空串
*@note 返回的时间字符串保存在dr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrTime 不能为NULL
*
*/	
CPE_DR_API char *dr_drtime_to_str_r(IN dr_time_t *a_piTdrTime, 
								   IN char *a_pszDstBuf, IN int a_iBuffSize);


/**
*将'YYYY-MM-DD HHH:MM:SS' 格式的日期/时间字符串转换成dr自定义格式的日期时间数据
*@param[out]	a_piTdrDateTime 保存dr定义的时间time数据
*@param[in]	a_pszDateTime 时间字符串
*
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrDateTime 不能为NULL
*@pre \e a_pszDateTime 不能为NULL
*/	
CPE_DR_API int dr_str_to_drdatetime(OUT dr_datetime_t *a_piTdrDateTime, IN const char *a_pszDateTime);

/**
*convert the number of seconds elapsed since midnight (00:00:00), January 1, 1970, Coordinated Universal Time (UTC)  to dr_datetime_t
*@param[out]	a_piTdrDateTime 保存dr定义的时间time数据
*@param[in]	a_tTimer  时间
*
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrDateTime 不能为NULL
*/	
CPE_DR_API int dr_utctime_to_drdatetime(OUT dr_datetime_t *a_piTdrDateTime, IN time_t a_tTimer);

/**
*convert dr_datetime_t to the number of seconds elapsed since midnight (00:00:00), January 1, 1970, Coordinated Universal Time (UTC)  
*@param[out]	a_ptTimer  保存时间的指针
*@param[in]	a_iTdrDateTime 保存dr定义的时间time数据
*
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrDateTime 不能为NULL
*/	
CPE_DR_API int dr_drdatetime_to_utctime(OUT  time_t *a_ptTimer, IN dr_datetime_t a_iTdrDateTime);


/**
*将dr自定义格式的日期/时间数据转换成'YYYY-MM-DD HHH:MM:SS' 格式的日期/时间字符串
*@param[in]	a_piTdrDateTime 保存dr定义的日期/时间数据的指针
*
*@return 如果处理成功,返回日期/时间字符串的指针,否则返回空串
*@note 返回的日期/时间字符串保存在dr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrDateTime 不能为NULL
*/	
CPE_DR_API char *dr_drdatetime_to_str(IN const dr_datetime_t *a_piTdrDateTime);

/**
*将dr自定义格式的日期/时间数据转换成'YYYY-MM-DD HHH:MM:SS' 格式的日期/时间字符串.线程安全版本
*@param[in]	a_piTdrDateTime 保存dr定义的日期/时间数据的指针
*@param[in] a_pszDstBuf	保存时间信息的字符串缓冲区
*@param[in] a_iBuffSize 缓冲区的可用大小
*
*@return 如果处理成功,返回日期/时间字符串的指针,否则返回空串
*@note 返回的日期/时间字符串保存在dr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrDateTime 不能为NULL
*/	
CPE_DR_API char *dr_drdatetime_to_str_r(IN const dr_datetime_t *a_piTdrDateTime, 
									   IN char *a_pszDstBuf, IN int a_iBuffSize);


/**
*比较两个日期/时间值的大小
*@param[in]	a_piTdrDateTime1 保存dr定义的日期/时间数据的指针
*@param[in]	a_piTdrDateTime2 保存dr定义的日期/时间数据的指针
*
*@retval >0 a_piTdrDateTime1比a_piTdrDateTime2大,即a_piTdrDateTime1时间晚于a_piTdrDateTime2
*@retval 0  时间相同
*@retval <0 a_piTdrDateTime1比a_piTdrDateTime2小,即a_piTdrDateTime1时间早于a_piTdrDateTime2
*
*@pre \e a_piTdrDateTime1 不能为NULL
*@pre \e a_piTdrDateTime2 不能为NULL
*/
CPE_DR_API int dr_compare_datetime(IN const dr_datetime_t *a_piTdrDateTime1, IN const dr_datetime_t *a_piTdrDateTime2);

/**比较两个日期值的大小
*@param[in]	a_piTdrDate1 保存dr定义的日期数据的指针
*@param[in]	a_piTdrDate2 保存dr定义的日期数据的指针
*
*@retval >0 a_piTdrDate1比a_piTdrDate2大,即a_piTdrDate1时间晚于a_piTdrDate2
*@retval 0  时间相同
*@retval <0 a_piTdrDate1比a_piTdrDate2小,即a_piTdrDate1时间早于a_piTdrDate2
*
*@pre \e a_piTdrDate1 不能为NULL
*@pre \e a_piTdrDate2 不能为NULL
*/
CPE_DR_API int dr_compare_date(IN const dr_date_t *a_piTdrDate1, IN const dr_date_t *a_piTdrDate2);

/**
*比较两个时间值的大小
*@param[in]	a_piTdrTime1 保存dr定义的时间数据的指针
*@param[in]	a_piTdrTime2 保存dr定义的时间数据的指针
*
*@retval >0 a_piTdrTime1比a_piTdrTime2大,即a_piTdrTime1时间晚于a_piTdrTime2
*@retval 0  时间相同
*@retval <0 a_piTdrTime1比a_piTdrTime2小,即a_piTdrTime1时间早于a_piTdrTime2
*
*@pre \e a_piTdrTime1 不能为NULL
*@pre \e a_piTdrTime2 不能为NULL
*/
CPE_DR_API int dr_compare_time(IN const dr_time_t *a_piTdrTime1, IN const dr_time_t *a_piTdrTime2);

/**  @}*/
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*CPE_DR_CTYPES_H*/
