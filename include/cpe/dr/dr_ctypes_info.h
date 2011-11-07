#ifndef CPE_DR_CTYPES_H
#define CPE_DR_CTYPES_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cpe/pal/socket.h"
#include "cpe/dr/dr_define.h"
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_external.h"

/** @defgroup CPE_DR_CTYPE_INFO CPE_DR_�����������ʹ���
*  @{
*/

/** @name ������������ID
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
*��numbers-and-dots ��ʽ��IP��ַ��ת����dr�Զ����ʽ��IP��ַ��������
*@param[out]	a_piTDRIp ����dr�����IP��ַ���ݵ�ָ��
*@param[in]	a_pszIP �洢numbers-and-dots ��ʽ��IP��ַ����ָ��
*
*@retval 0   ����ɹ�
*@retval  non-zero  ����ʧ��
*
* @pre \e a_piTDRIp ����Ϊ NULL
* @pre \e a_pszIP ����Ϊ NULL
*
*/
CPE_DR_API int dr_ineta_to_drip(OUT dr_ip_t *a_piTDRIp, IN const char *a_pszIP);

/**
*��dr�Զ����ʽ��IP��ַ����ת����struct in_addr�ṹ����
*@param[out]	a_pInp ���struct in_addr�ṹ������
*@param[in]	a_iTDRIp dr�Զ����ʽ��IP��ַ����
*
*@retval 0   ����ɹ�
*@retval  non-zero  ����ʧ��
*
* @pre \e a_pInp ����Ϊ NULL
*
*/
CPE_DR_API int dr_drip_to_inetaddr(OUT struct in_addr *a_pInp, IN dr_ip_t a_iTDRIp);

/**
*��dr�Զ����ʽ��IP��ַ����ת����numbers-and-dots ��ʽ��IP��ַ��
*@param[in]	a_iTDRIp dr�Զ����ʽ��IP��ַ����
*
*@return �������ɹ�,����ip��ַ����ָ��,���򷵻�NULL
*@note IP��ַ��������dr����ľ�̬������,���˺����´ε���ʱ,�����ݻᱻ����
*
*/
CPE_DR_API char *dr_drip_to_ineta(IN dr_ip_t a_iTDRIp);

/**
*��dr�Զ����ʽ��IP��ַ����ת����numbers-and-dots ��ʽ��IP��ַ��
*@param[in]	a_iTDRIp dr�Զ����ʽ��IP��ַ����
*@param[in] a_pszDstBuff	����IP��ַ��Ϣ���ַ���������
*@param[in] a_iBuffSize �������Ŀ��ô�С
*
*@return �������ɹ�,����a_pszDstBuff��ָ��,���򷵻�NULL

*
*/
CPE_DR_API const char *dr_drip_ntop(IN dr_ip_t a_iTDRIp, IN char *a_pszDstBuff, IN int a_iBuffSize);
/**
*��'YYYY-MM-DD' ��ʽ��date�ַ���ת����dr�Զ����ʽ����������
*@param[out]	a_piTdrDate ����dr������������ݵ�ָ��
*@param[in]	a_pszDate �����ַ���
*
*@return  �ɹ�����0, ���򷵻ط�0ֵ
*
*@pre \e a_piTdrDate ����ΪNULL
*@pre \e a_pszDate ����ΪNULL
*/	
CPE_DR_API int dr_str_to_drdate(OUT dr_date_t *a_piTdrDate, IN const char *a_pszDate);

/**��dr�Զ����ʽ����������ת����'YYYY-MM-DD' ��ʽ��date�ַ���
*@param[in]	a_piTdrDate ����dr������������ݵ�ָ��
*
*@return �������ɹ�,���������ַ�����ָ��,���򷵻ؿմ�
*@note ���ص������ַ���������dr����ľ�̬������,���˺����´ε���ʱ,�����ݻᱻ����
*
*@pre \e a_piTdrDate ����ΪNULL
*
*/	
CPE_DR_API char *dr_drdate_to_str(IN const dr_date_t *a_piTdrDate);

/**��dr�Զ����ʽ����������ת����'YYYY-MM-DD' ��ʽ��date�ַ��� ���ӿ����̰߳�ȫ�汾
*@param[in]	a_piTdrDate ����dr������������ݵ�ָ��
*@param[in] a_pszDstBuf	����ʱ����Ϣ���ַ���������
*@param[in] a_iBuffSize �������Ŀ��ô�С
*
*@return �������ɹ�,���������ַ�����ָ��,���򷵻ؿմ�
*@note ���ص������ַ���������dr����ľ�̬������,���˺����´ε���ʱ,�����ݻᱻ����
*
*@pre \e a_piTdrDate ����ΪNULL
*
*/	
CPE_DR_API const char *dr_drdate_to_str_r(IN const dr_date_t *a_piTdrDate, 
										 IN char *a_pszDstBuf, IN int a_iBuffSize);


/**
*��'HHH:MM:SS' ��ʽ��time�ַ���ת����dr�Զ����ʽ��ʱ������
*@param[out]	a_piTdrTime ����dr�����ʱ��time����
*@param[in]	a_pszTime ʱ���ַ���
*
*@note ֧��3���ַ�����ʾСʱֵ��Ŀ��: time�������Ͳ����������ڱ�ʾһ���ʱ��(��һ�����ᳬ�� 24 Сʱ)�����ҿ���������ʾ��������ʱ��������¼�֮���ʱ����(����ܱ� 24 Сʱ��������һ����ֵ)
*@return  �ɹ�����0, ���򷵻ط�0ֵ
*
*@pre \e a_piTdrTime ����ΪNULL
*@pre \e a_pszTime ����ΪNULL
*/	
CPE_DR_API int dr_str_to_drtime(OUT dr_time_t *a_piTdrTime, IN const char *a_pszTime);

/**
*��dr�Զ����ʽ��ʱ������ת����'HHH:MM:SS' ��ʽ��time�ַ���
*@param[in]	a_piTdrTime ����dr�����ʱ�����ݵ�ָ��
*
*@return �������ɹ�,����ʱ���ַ�����ָ��,���򷵻ؿմ�
*@note ���ص�ʱ���ַ���������dr����ľ�̬������,���˺����´ε���ʱ,�����ݻᱻ����
*
*@pre \e a_piTdrTime ����ΪNULL
*
*/	
CPE_DR_API char *dr_drtime_to_str(IN dr_time_t *a_piTdrTime);

/**
*��dr�Զ����ʽ��ʱ������ת����'HHH:MM:SS' ��ʽ��time�ַ���.�̰߳�ȫ�汾
*@param[in]	a_piTdrTime ����dr�����ʱ�����ݵ�ָ��
*@param[in] a_pszDstBuf	����ʱ����Ϣ���ַ���������
*@param[in] a_iBuffSize �������Ŀ��ô�С
*
*@return �������ɹ�,����ʱ���ַ�����ָ��,���򷵻ؿմ�
*@note ���ص�ʱ���ַ���������dr����ľ�̬������,���˺����´ε���ʱ,�����ݻᱻ����
*
*@pre \e a_piTdrTime ����ΪNULL
*
*/	
CPE_DR_API char *dr_drtime_to_str_r(IN dr_time_t *a_piTdrTime, 
								   IN char *a_pszDstBuf, IN int a_iBuffSize);


/**
*��'YYYY-MM-DD HHH:MM:SS' ��ʽ������/ʱ���ַ���ת����dr�Զ����ʽ������ʱ������
*@param[out]	a_piTdrDateTime ����dr�����ʱ��time����
*@param[in]	a_pszDateTime ʱ���ַ���
*
*@return  �ɹ�����0, ���򷵻ط�0ֵ
*
*@pre \e a_piTdrDateTime ����ΪNULL
*@pre \e a_pszDateTime ����ΪNULL
*/	
CPE_DR_API int dr_str_to_drdatetime(OUT dr_datetime_t *a_piTdrDateTime, IN const char *a_pszDateTime);

/**
*convert the number of seconds elapsed since midnight (00:00:00), January 1, 1970, Coordinated Universal Time (UTC)  to dr_datetime_t
*@param[out]	a_piTdrDateTime ����dr�����ʱ��time����
*@param[in]	a_tTimer  ʱ��
*
*@return  �ɹ�����0, ���򷵻ط�0ֵ
*
*@pre \e a_piTdrDateTime ����ΪNULL
*/	
CPE_DR_API int dr_utctime_to_drdatetime(OUT dr_datetime_t *a_piTdrDateTime, IN time_t a_tTimer);

/**
*convert dr_datetime_t to the number of seconds elapsed since midnight (00:00:00), January 1, 1970, Coordinated Universal Time (UTC)  
*@param[out]	a_ptTimer  ����ʱ���ָ��
*@param[in]	a_iTdrDateTime ����dr�����ʱ��time����
*
*@return  �ɹ�����0, ���򷵻ط�0ֵ
*
*@pre \e a_piTdrDateTime ����ΪNULL
*/	
CPE_DR_API int dr_drdatetime_to_utctime(OUT  time_t *a_ptTimer, IN dr_datetime_t a_iTdrDateTime);


/**
*��dr�Զ����ʽ������/ʱ������ת����'YYYY-MM-DD HHH:MM:SS' ��ʽ������/ʱ���ַ���
*@param[in]	a_piTdrDateTime ����dr���������/ʱ�����ݵ�ָ��
*
*@return �������ɹ�,��������/ʱ���ַ�����ָ��,���򷵻ؿմ�
*@note ���ص�����/ʱ���ַ���������dr����ľ�̬������,���˺����´ε���ʱ,�����ݻᱻ����
*
*@pre \e a_piTdrDateTime ����ΪNULL
*/	
CPE_DR_API char *dr_drdatetime_to_str(IN const dr_datetime_t *a_piTdrDateTime);

/**
*��dr�Զ����ʽ������/ʱ������ת����'YYYY-MM-DD HHH:MM:SS' ��ʽ������/ʱ���ַ���.�̰߳�ȫ�汾
*@param[in]	a_piTdrDateTime ����dr���������/ʱ�����ݵ�ָ��
*@param[in] a_pszDstBuf	����ʱ����Ϣ���ַ���������
*@param[in] a_iBuffSize �������Ŀ��ô�С
*
*@return �������ɹ�,��������/ʱ���ַ�����ָ��,���򷵻ؿմ�
*@note ���ص�����/ʱ���ַ���������dr����ľ�̬������,���˺����´ε���ʱ,�����ݻᱻ����
*
*@pre \e a_piTdrDateTime ����ΪNULL
*/	
CPE_DR_API char *dr_drdatetime_to_str_r(IN const dr_datetime_t *a_piTdrDateTime, 
									   IN char *a_pszDstBuf, IN int a_iBuffSize);


/**
*�Ƚ���������/ʱ��ֵ�Ĵ�С
*@param[in]	a_piTdrDateTime1 ����dr���������/ʱ�����ݵ�ָ��
*@param[in]	a_piTdrDateTime2 ����dr���������/ʱ�����ݵ�ָ��
*
*@retval >0 a_piTdrDateTime1��a_piTdrDateTime2��,��a_piTdrDateTime1ʱ������a_piTdrDateTime2
*@retval 0  ʱ����ͬ
*@retval <0 a_piTdrDateTime1��a_piTdrDateTime2С,��a_piTdrDateTime1ʱ������a_piTdrDateTime2
*
*@pre \e a_piTdrDateTime1 ����ΪNULL
*@pre \e a_piTdrDateTime2 ����ΪNULL
*/
CPE_DR_API int dr_compare_datetime(IN const dr_datetime_t *a_piTdrDateTime1, IN const dr_datetime_t *a_piTdrDateTime2);

/**�Ƚ���������ֵ�Ĵ�С
*@param[in]	a_piTdrDate1 ����dr������������ݵ�ָ��
*@param[in]	a_piTdrDate2 ����dr������������ݵ�ָ��
*
*@retval >0 a_piTdrDate1��a_piTdrDate2��,��a_piTdrDate1ʱ������a_piTdrDate2
*@retval 0  ʱ����ͬ
*@retval <0 a_piTdrDate1��a_piTdrDate2С,��a_piTdrDate1ʱ������a_piTdrDate2
*
*@pre \e a_piTdrDate1 ����ΪNULL
*@pre \e a_piTdrDate2 ����ΪNULL
*/
CPE_DR_API int dr_compare_date(IN const dr_date_t *a_piTdrDate1, IN const dr_date_t *a_piTdrDate2);

/**
*�Ƚ�����ʱ��ֵ�Ĵ�С
*@param[in]	a_piTdrTime1 ����dr�����ʱ�����ݵ�ָ��
*@param[in]	a_piTdrTime2 ����dr�����ʱ�����ݵ�ָ��
*
*@retval >0 a_piTdrTime1��a_piTdrTime2��,��a_piTdrTime1ʱ������a_piTdrTime2
*@retval 0  ʱ����ͬ
*@retval <0 a_piTdrTime1��a_piTdrTime2С,��a_piTdrTime1ʱ������a_piTdrTime2
*
*@pre \e a_piTdrTime1 ����ΪNULL
*@pre \e a_piTdrTime2 ����ΪNULL
*/
CPE_DR_API int dr_compare_time(IN const dr_time_t *a_piTdrTime1, IN const dr_time_t *a_piTdrTime2);

/**  @}*/
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*CPE_DR_CTYPES_H*/
