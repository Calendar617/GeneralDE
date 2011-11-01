#ifndef CPE_DR_XML_METALIB_INIT_H
#define CPE_DR_XML_METALIB_INIT_H


#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_define.h"
#include "cpe/dr/dr_external.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup CPE_DR_METALIB_MNG CPE_DR_元数据库(metalib)管理
 *  @{
 */

/**
 *释放元数据库的资源，并将指向元数库的指针置为NULL。
 */
CPE_DR_API void dr_free_lib(INOUT LPDRMETALIB* ppstLib);


/**
*将元数据描述库复制到指定缓冲区中
*@param[out] a_pvDst 保存复制元数据描述库的缓冲区地址
*@param[in,out] a_piBuff 传入缓冲区的大小， 输出复制后元数据库的大小
*@param[in] a_pstSrcLib 待复制元数据库的指针
*
*@return 复制成功返回复制元数据库的指针，否则返回NULL
*@note 如果指定缓冲区的大小比实际需要的要小,则复制会失败
* @pre \e a_pvDst 不能为 NULL
* @pre \e a_piBuff 不能为NULL，且其所指缓冲区的大小必须大于0
* @pre \e a_pstSrcLib 不能为NULL
*/
CPE_DR_API LPDRMETALIB dr_clone_lib(OUT void* a_pvDst, INOUT int* a_piBuff, IN LPDRMETALIB a_pstSrcLib);


/**
*获取元数库所占内存字节数
*@return success:元数库所占内存字节数 failed: -1
* @pre \e ppstLib 不能为 NULL
*/
CPE_DR_API size_t dr_size(IN LPDRMETALIB pstLib);

/**获取元数据描述库的名字
*@param[in] a_pstLib 元数据描述库的指针
*
*@return success:元数据描述库的名字字符串的指针  failed: ""
*
* @pre \e a_pstLib 不能为 NULL
*/
CPE_DR_API const char *dr_get_metalib_name(IN LPDRMETALIB a_pstLib);

/**获取元数据描述库的版本
*@param[in] a_pstLib 元数据描述库的指针
*
*@return success: 元数据描述库的版本 failed: 0
*
* @pre \e a_pstLib 不能为 NULL
*/
CPE_DR_API int dr_get_metalib_version(IN LPDRMETALIB a_pstLib);

/**获取元数据描述库的构建版本
*@param[in] a_pstLib 元数据描述库的指针
*
*@return cuccess: 元数据描述库的构建版本 failed : -1
*
* @pre \e a_pstLib 不能为 NULL
*/
CPE_DR_API int dr_get_metalib_build_version(IN LPDRMETALIB a_pstLib);

/** @}*/ 

#ifdef __cplusplus
}
#endif /* __cplusplus */


 
#endif  /* CPE_DR_XML_METALIB_INIT_H */

