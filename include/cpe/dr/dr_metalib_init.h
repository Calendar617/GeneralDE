#ifndef CPE_DR_XML_METALIB_INIT_H
#define CPE_DR_XML_METALIB_INIT_H


#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_define.h"
#include "cpe/dr/dr_external.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup CPE_DR_METALIB_MNG CPE_DR_Ԫ���ݿ�(metalib)����
 *  @{
 */

/**
 *�ͷ�Ԫ���ݿ����Դ������ָ��Ԫ�����ָ����ΪNULL��
 */
CPE_DR_API void dr_free_lib(INOUT LPDRMETALIB* ppstLib);


/**
*��Ԫ���������⸴�Ƶ�ָ����������
*@param[out] a_pvDst ���渴��Ԫ����������Ļ�������ַ
*@param[in,out] a_piBuff ���뻺�����Ĵ�С�� ������ƺ�Ԫ���ݿ�Ĵ�С
*@param[in] a_pstSrcLib ������Ԫ���ݿ��ָ��
*
*@return ���Ƴɹ����ظ���Ԫ���ݿ��ָ�룬���򷵻�NULL
*@note ���ָ���������Ĵ�С��ʵ����Ҫ��ҪС,���ƻ�ʧ��
* @pre \e a_pvDst ����Ϊ NULL
* @pre \e a_piBuff ����ΪNULL��������ָ�������Ĵ�С�������0
* @pre \e a_pstSrcLib ����ΪNULL
*/
CPE_DR_API LPDRMETALIB dr_clone_lib(OUT void* a_pvDst, INOUT int* a_piBuff, IN LPDRMETALIB a_pstSrcLib);


/**
*��ȡԪ������ռ�ڴ��ֽ���
*@return success:Ԫ������ռ�ڴ��ֽ��� failed: -1
* @pre \e ppstLib ����Ϊ NULL
*/
CPE_DR_API size_t dr_size(IN LPDRMETALIB pstLib);

/**��ȡԪ���������������
*@param[in] a_pstLib Ԫ�����������ָ��
*
*@return success:Ԫ����������������ַ�����ָ��  failed: ""
*
* @pre \e a_pstLib ����Ϊ NULL
*/
CPE_DR_API const char *dr_get_metalib_name(IN LPDRMETALIB a_pstLib);

/**��ȡԪ����������İ汾
*@param[in] a_pstLib Ԫ�����������ָ��
*
*@return success: Ԫ����������İ汾 failed: 0
*
* @pre \e a_pstLib ����Ϊ NULL
*/
CPE_DR_API int dr_get_metalib_version(IN LPDRMETALIB a_pstLib);

/**��ȡԪ����������Ĺ����汾
*@param[in] a_pstLib Ԫ�����������ָ��
*
*@return cuccess: Ԫ����������Ĺ����汾 failed : -1
*
* @pre \e a_pstLib ����Ϊ NULL
*/
CPE_DR_API int dr_get_metalib_build_version(IN LPDRMETALIB a_pstLib);

/** @}*/ 

#ifdef __cplusplus
}
#endif /* __cplusplus */


 
#endif  /* CPE_DR_XML_METALIB_INIT_H */

