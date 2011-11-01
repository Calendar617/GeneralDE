#ifndef CPE_DR_METALIB_MANAGE_H
#define CPE_DR_METALIB_MANAGE_H
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_external.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	
/** @defgroup CPE_DR_META_MANAGE CPE_DR_元数据(meta)管理
*  @{
*/


/** 根据元数据名在元数据库中查找此元数据
*@param[in] a_pstLib 元数据库指针
*@param[in] a_pszName 元数据名字串指针
*
*@return 如果找到则返回元数据的成员，否则返回NULL
*
* @pre \e a_pstLib 不能为 NULL
* @pre \e a_pszName 不能为 NULL
*/
CPE_DR_API LPDRMETA dr_get_meta_by_name(IN LPDRMETALIB a_pstLib, IN const char* a_pszName);

/**根据元数据的ID在元数据库中查找此元数据
*@param[in] a_pstLib 元数据库指针
*@param[in] a_iID 元数据的ID
*
*@return 如果找到则返回元数据的成员，否则返回NULL
*
* @pre \e a_pstLib 不能为 NULL
*/
CPE_DR_API LPDRMETA dr_get_meta_by_id(IN LPDRMETALIB a_pstLib, IN int a_iID);


/** 获取元数据描述的基准版本号
*@param[in] a_pstMeta 元数据描述的指针
*
*@return success: 元数据描述的基准版本号 failed: -1
*
* @pre \e a_pstMeta 不能为 NULL
*/
CPE_DR_API int dr_get_meta_based_version(IN LPDRMETA a_pstMeta);



/**
*获取元数据描述的当前版本号
*@param[in] a_pstMeta 元数据描述的指针
*
*@return success:元数据描述的当前版本号 failed: -1*
* @pre \e a_pstMeta 不能为 NULL
*/
CPE_DR_API int dr_get_meta_current_version(IN LPDRMETA a_pstMeta);

/**
*获取元数据描述的基本数据类型ID
*@param[in] a_pstMeta 元数据描述的指针
*
*@return 数据类型ID: CPE_DR_TYPE_UNION 或 CPE_DR_TYPE_STRUCT
*
* @pre \e a_pstMeta 不能为 NULL
*/
CPE_DR_API int dr_get_meta_type(IN LPDRMETA a_pstMeta);

/**获取元数据描述的名字
*@param[in] a_pstMeta 元数据描述的指针
*
*@return 元数据名字字符串的指针
*
* @pre \e a_pstMeta 不能为 NULL
*/
CPE_DR_API const char *dr_get_meta_name(IN LPDRMETA a_pstMeta);

CPE_DR_API const char *dr_get_meta_desc(IN LPDRMETA a_pstMeta);

CPE_DR_API int dr_get_meta_id(IN LPDRMETA a_pstMeta);

/**
*获取复合元数据类型(meta)的存储空间
*@param[in] a_pstMeta 元数据描述的指针
*
*@return success: 复合元数据类型(meta)的存储的字节数 failed : -1
*
* @pre \e a_pstMeta 不能为 NULL
*/
CPE_DR_API size_t dr_get_meta_size(IN LPDRMETA a_pstMeta);

/**
*获取复合元数据类型成员元素的个数
*@param[in] a_pstMeta 元数据描述的指针
*
*@return 最大Id值
*
* @pre \e a_pstMeta 不能为 NULL
*/
CPE_DR_API int dr_get_entry_num(IN LPDRMETA a_pstMeta);


/** 根据target元素路径获取此元素在meta中的偏移
*@param[out] a_pstRedirector 此元素的偏移
*@param[in] a_pstMeta pstEntry所在的自定义结构
*@param[in] a_iEntry 在前iEntry个元素中查找
*@param[in] a_pszName 要处理的名字
*
*@return 成功返回CPE_DR_SUCCESS，否则返回错误代码
*
*@pre \e a_piOff 不能为NULL
*@pre \e a_pstMeta 不能为NULL
*@pre \e a_piUnit 不能为NULL
*@pre \e a_pszName 不能为NULL
*@pre \e 必须先获取元素的type,count ,refer属性
*/
int dr_sizeinfo_to_off(LPDRSIZEINFO a_pstRedirector, LPDRMETA a_pstMeta, int a_iEntry, const char* a_pszName);
#define dr_sizeinfo_to_off_i   dr_sizeinfo_to_off

/**根据成员在结构体中的路径(Path)信息获取此成员基于结构体首地址的偏移值
*@param[in] a_pstMeta 需要定位的结构的元数据描述句柄
*@param[in,out] a_ppstEntry 保存对应成员的描述句柄的指针
*@param[out] a_piHOff 保存对应成员的偏移值
*@param[in] a_pszPath  保存此成员路径信息的缓冲区
*
*return  成功返回0，否则返回负数
*
*@pre \e a_pszBuff 不能为NULL
*@pre \e a_pstMeta 不能为NULL
*@pre \e a_iBuff 必须大于0
*@pre \e a_iOff 必须大于等于0
*@pre \e a_pszPath 不能为NULL
*/
CPE_DR_API int dr_entry_path_to_off(IN LPDRMETA a_pstMeta, INOUT LPDRMETAENTRY *a_ppstEntry, OUT DROFF *a_piHOff, IN const char *a_pszPath);


/**根据成员在结构体中的偏移定位此成员在结构中的路径信息
*@param[in] a_pstMeta 需要定位的结构的元数据描述句柄
*@param[in] a_iOff  成员的偏移
*
*@note 路径信息串保存在静态缓冲区中，再次调用本接口时，上次调用获取的路径信息将被回写
*
*return  成功返回0，否则返回负数
*
*@pre \e a_pstMeta 不能为NULL
*@pre \e a_iOff 必须大于等于0
*/
CPE_DR_API char *dr_entry_off_to_path(IN LPDRMETA a_pstMeta, IN int a_iOff, char * a_pBuf, size_t a_iBufSize);


/**查询此结构体是否包含定义了自增属性的成员
*@param[in] a_pstMeta 结构体成员的元数据描述句柄
*
*@pre \e a_pstMeta 不能为NULL
*
*@return 如果有自增属性成员，则返回非零值，否则返回0
*/
CPE_DR_API int dr_do_have_autoincrement_entry(IN LPDRMETA a_pstMeta);

/** @}*/ /* CPE_DR_META_MANAGE CPE_DR_元数据(meta)管理*/


/** @defgroup CPE_DR_ENTRY_MANAGE CPE_DR_数据成员(entry)管理
*  @{
*/

/**获取元数据描述的名字
*@param[in] a_pstEntry 元数据描述的指针
*
*@return 元数据名字字符串的指针
*
* @pre \e a_pstEntry 不能为 NULL
*/
CPE_DR_API const char *dr_get_entry_name(IN LPDRMETAENTRY a_pstEntry);

/**获取元数据描述的cname
 *@param[in] a_pstEntry 元数据描述的指针
 *
 *@return 元数据名字字符串的指针
 *
 * @pre \e a_pstEntry 不能为 NULL
 */
CPE_DR_API const char *dr_get_entry_cname(IN LPDRMETAENTRY a_pstEntry);

/**获取元数据描述的desc属性字符串
 *@param[in] a_pstEntry 元数据描述的指针
 *
 *@return 元数据desc属性字符串的指针
 *
 * @pre \e a_pstEntry 不能为 NULL
 */
CPE_DR_API const char *dr_get_entry_desc(IN LPDRMETAENTRY a_pstEntry);

/**获取元数据描述绑定的macrosgroup的指针
 *@param[in] a_pstEntry 元数据描述的指针
 *
 *@return 如果元数据绑定了macrosgroup, 返回元数据绑定的macrosgroup的指针; 否则返回NULL
 *
 * @pre \e a_pstEntry 不能为 NULL
 */
CPE_DR_API LPDRMACROSGROUP dr_get_entry_macrosgroup(IN LPDRMETAENTRY a_pstEntry);

/**根据元数据成员名在元数据库中查找此成员的句柄
 *@param[in] a_pstMeta 待查找元数据的指针
 *@param[in] a_pszName entry的名字
 *
 *@return 如果存在指定名字的成员，则返回此成员的句柄；否则返回NULL
 *
 *@pre \e a_pstMeta 不能为NULL
 *@pre \e a_pszName 不能为 NULL
 */
CPE_DR_API LPDRMETAENTRY dr_get_entryptr_by_name(IN LPDRMETA a_pstMeta, IN const char* a_pszName);

/**根据元数据成员名在元数据库中查找此成员（以在该元数据成员数组中的下标形式返回）
*@param[out] a_piIdx 输出此成员的索引
*@param[in] a_pstMeta 待查找元数据的指针
*@param[in] a_pszName 元数据名字串指针
*
*@retval <0  处理失败，返回表示出错信息的错误代码
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*
* @pre \e a_piIdx 不能为 NULL
*@pre \e a_pstMeta 不能为NULL
* @pre \e a_pszName 不能为 NULL
*/
CPE_DR_API int dr_get_entry_by_name(OUT int* a_piIdx, IN LPDRMETA a_pstMeta, IN const char* a_pszName);

/**根据元数据成员的ID在元数据库中查找此成员（以在该元数据成员数组中的下标形式返回）
*如果元数据是union结构，则如果a_iId在某成员[minid, maxid]属性之间，则选择此成员，否则
*选择缺省成员（如果存在的话）
*@param[out] a_piIdx 输出此成员的索引
*@param[in] a_pstMeta 待查找元数据的指针
*@param[in] a_iId 元数据的ID
*
*@retval <0  处理失败，返回表示出错信息的错误代码
*@retval 0   处理成功
*
* @pre \e a_piIdx 不能为 NULL
*@pre \e a_pstMeta 不能为NULL
*/
CPE_DR_API int dr_get_entry_by_id(OUT int* a_piIdx, IN LPDRMETA a_pstMeta, IN int a_iId);

/**根据元数据成员在元数据结构中的索引获取此成员描述的句柄
*@param[in] a_pstMeta 待查找元数据的指针
*@param[in] a_idxEntry entry的索引
*
*@return 如果索引正确,则返回此成员描述的句柄,否则返回NULL
*
*@pre \e a_pstMeta 不能为NULL
*@see dr_get_meta_by_id
*@see dr_get_entry_by_id
*/
CPE_DR_API LPDRMETAENTRY dr_get_entry_by_index(IN LPDRMETA a_pstMeta, IN int a_idxEntry);

/**获取entry元素id属性值
*@param[in] a_pstEntry entry元素描述结构的指针
*
*@note 通过调用dr_get_entry_by_index可以获取entry元素描述结构的指针
*
*@return entry元素id属性值
*
*@pre \e a_pstEntry 不能为NULL
*@see dr_get_entry_by_index
*/
CPE_DR_API int dr_get_entry_id(IN LPDRMETAENTRY a_pstEntry);

/**获取entry元素id属性值对应的宏定义的名字
*@param[in] a_pstEntry entry元素描述结构的指针
*@param[in] a_pstLib 元数据描述库的指针
*
*@note 通过调用dr_get_entry_by_index可以获取entry元素描述结构的指针
*
*@return 如果此成员的id属性值为宏定义,则返回此宏定义的名字;否则返回空串""
*
*@pre \e a_pstEntry 不能为NULL
*@pre \e a_pstLib 不能为NULL
*@see dr_get_entry_by_index
*/
CPE_DR_API const char *dr_get_entry_id_name(IN LPDRMETALIB a_pstLib, IN LPDRMETAENTRY a_pstEntry);


/**获取单个entry成员的本地存储空间大小
*@param[in] a_pstEntry entry元素描述结构的指针
*
*@note 通过调用dr_get_entry_by_index可以获取entry元素描述结构的指针
*
*@return entry元素单个变量的本地存储空间大小
*
*@pre \e a_pstEntry 不能为NULL
*@see dr_get_entry_by_index
*/
CPE_DR_API size_t dr_get_entry_unitsize(IN LPDRMETAENTRY a_pstEntry);

/**获取entry的数据类型ID
*@param[in] a_pstEntry entry元素描述结构的指针
*
*
*@return 返回成员的数据类型ID，关于DR支持的数据ID定义请参见dr_ctypes_info.h中"基本数据类型"
*
*/
CPE_DR_API int dr_get_entry_type(IN LPDRMETAENTRY a_pstEntry);

/**获取entry的数组计数信息
*@param[in] a_pstEntry entry元素描述结构的指针
*
*
*@retval 0	表示此成员为可变长度数组
*@retval 1	表示此成员不是数据
*@retval >1 表示此成员为固定长度的数据
*
*/
CPE_DR_API int dr_get_entry_count(IN LPDRMETAENTRY a_pstEntry);

/**获取entry元素所指meta结构描述的指针
* 如果entry元素为复合数据类型时,通过此接口可以获取此entry复合数据类型的描述结构的指针
*@param[in] a_pstLib	元数据描述库的指针
*@param[in] a_pstEntry entry元素描述结构的指针
*
*
*@return 如果entry元素为复合数据类型时,返回所指复合数据类型描述的指针;否则返回NULL
*
*/
CPE_DR_API LPDRMETA dr_get_entry_type_meta(IN LPDRMETALIB a_pstLib, IN LPDRMETAENTRY a_pstEntry);

/**根据元数据成员描述路径在元数据库中查找此成员
*@param[in] a_pstMeta 待查找元数据的指针
*@param[in] a_pszEntryPath entry在结构中的名字路径名
*
*@return 如果存在指定名字的成员，则返回此成员的句柄；否则返回NULL
*
*@pre \e a_pstMeta 不能为NULL
*@pre \e a_pszEntryPath 不能为 NULL*/
CPE_DR_API LPDRMETAENTRY dr_get_entry_by_path(IN LPDRMETA a_pstMeta, IN const char* a_pszEntryPath);

/**获取成员的自定义属性值
*@param[in] a_pstEntry 成员的元数据描述句柄
*@param[in] a_pstLib	元数据描述库的指针
*
*@return 如果此成员定义了customattr属性,则返回此属性的首地址;否则返回""
*
*@pre \e a_pstEntry 不能为NULL
*@pre \e a_pstLib 不能为NULL
*/
CPE_DR_API const char *dr_get_entry_customattr(IN LPDRMETALIB a_pstLib, IN LPDRMETAENTRY a_pstEntry);


/** @}*/ /* CPE_DR_ENTRY_MANAGE CPE_DR_数据成员(entry)管理*/

/** @defgroup CPE_DR_MACRO_MANAGE CPE_DR_宏定义(macro)管理
*  @{
*/		 
/** 根据宏名获得其数值
*@param[out] a_piID 获得宏定义的数值
*@param[in] a_pstLib 元数据库
*@param[in] a_pszName 需查找宏的名字
*
*@return 成功返回0，否则返回非0
*
*@pre \e a_piID 不能为NULL
*@pre \e a_pstLib 不能为NULL
*@pre \e a_pszName 不能为NULL
*/
CPE_DR_API int dr_get_macro_value(OUT int *a_piID, IN LPDRMETALIB a_pstLib, IN const  char *a_pszName);

/** 获取a_pstLib中宏定义的数量（包括macrosgroup中定义的宏）
 *@param[in] a_pstLib 元数据库
 *
 *@return 库中定义的宏的数量
 *
 *@pre \e a_pstLib 不能为NULL
 */
CPE_DR_API int dr_get_metalib_macro_num(IN LPDRMETALIB a_pstLib);

/** 根据宏的index获得其指针
 *@param[in] a_pstLib 元数据库
 *@param[in] a_iIdx 需查找宏的index
 *
 *@return 成功返回宏指针，否则返回NULL
 *
 *@pre \e a_pstLib 不能为NULL
 *@pre \e a_iIdx 必须>=0，且小于a_pstLib中的宏的数量
 */
CPE_DR_API LPDRMACRO dr_get_metalib_macro_by_index(IN LPDRMETALIB a_pstLib, IN int a_iIdx);

/** 根据宏指针获得其名字
 *@param[in] a_pstLib 元数据库
 *@param[in] a_pstMacro 宏指针
 *
 *@return 指向宏名字的指针
 *
 *@pre \e a_pstLib 不能为NULL
 *@pre \e a_pstMacro 不能为NULL
 */
CPE_DR_API const char* dr_get_macro_name_by_ptr(IN LPDRMETALIB a_pstLib, IN LPDRMACRO a_pstMacro);

/** 根据宏指针获得其值
 *@param[out] a_piID 获得宏定义的数值
 *@param[in] a_pstMacro 宏指针
 *
 *@return 成功返回 0，否则返回 !0
 *
 *@pre \e a_piID 不能为NULL
 *@pre \e a_pstMacro 不能为NULL
 */
CPE_DR_API int dr_get_macro_value_by_ptr(OUT int *a_piID, IN LPDRMACRO a_pstMacro);

/** 根据宏指针获得其desc字符串
 *@param[in] a_pstLib 元数据库
 *@param[in] a_pstMacro 宏指针
 *
 *@return 指向宏的desc属性字符串的指针
 *
 *@pre \e a_pstLib 不能为NULL
 *@pre \e a_pstMacro 不能为NULL
 */
CPE_DR_API const char* dr_get_macro_desc_by_ptr(IN LPDRMETALIB a_pstLib, IN LPDRMACRO a_pstMacro);

/**获取宏所在的macrosgroup的指针
 *@param[in] a_pstLib 元数据库
 *@param[in] a_pstMacro 宏指针
 *
 *@return 如果宏属于某个macrosgroup, 返回指向该macrosgroup的指针; 否则返回NULL
 *
 *@pre \e a_pstLib 不能为NULL
 *@pre \e a_pstMacro 不能为 NULL
 *
 *@note 查找算法是简单的遍历，最差性能O(n*n)
 */
CPE_DR_API LPDRMACROSGROUP dr_get_macro_macrosgroup(IN LPDRMETALIB a_pstLib,
                                                   IN LPDRMACRO a_pstMacro);


/** @}*/ /* CPE_DR_MACRO_MANAGE CPE_DR_宏定义(macro)管理*/

/** @defgroup CPE_DR_MACROSGROUP_MANAGE CPE_DR_宏定义组(macrosgroup)管理
 *  @{
 */

/** 获取元数据库中定义的macrosgroup的数量
 *@param[in] a_pstLib 元数据库
 *
 *@return 元数据库中定义的macrosgroup的数量
 *
 *@pre \e a_pstLib 不能为NULL
 */
CPE_DR_API int dr_get_macrosgroup_num(IN LPDRMETALIB a_pstLib);

/** 根据宏定义组的index获取其指针
 *@param[in] a_pstLib 元数据库
 *@param[in] a_iIdx 宏定义组的index
 *
 *@return 指向宏定义组的指针
 *
 *@pre \e a_pstLib 不能为NULL
 *@pre \e a_iIdx 必须>=0, 且小于a_pstLib中的宏定义组的数量
 */
LPDRMACROSGROUP dr_get_macrosgroup_by_index(IN LPDRMETALIB a_pstLib, IN int a_iIdx);

/** 获取宏定义组a_pstGroup的名字
 *@param[in] a_pstGroup 宏定义组指针
 *
 *@return 指向宏定义组的名字字符串的指针
 *
 *@pre \e a_pstGroup 不能为NULL
 */
const char* dr_get_macrosgroup_name(IN LPDRMACROSGROUP a_pstGroup);

/** 获取宏定义组a_pstGroup中包含的宏定义的数量
 *@param[in] a_pstGroup 宏定义组指针
 *
 *@return 宏定义的数量
 *
 *@pre \e a_pstGroup 不能为NULL
 */
CPE_DR_API int dr_get_macrosgroup_macro_num(IN LPDRMACROSGROUP a_pstGroup);

/** 获取宏定义组a_pstGroup中index对应的宏定义
 *@param[in] a_pstLib 元数据库
 *@param[in] a_pstGroup 宏定义组指针
 *@param[in] a_iIdx 宏定义组的index
 *
 *@return 指向宏定义的指针
 *
 *@pre \e a_pstLib 不能为NULL
 *@pre \e a_pstGroup 不能为NULL
 *@pre \e a_iIdx 必须>=0, 且小于a_pstGroup中的宏定义的数量
 */
CPE_DR_API LPDRMACRO dr_get_macrosgroup_macro_by_index(IN LPDRMETALIB a_pstLib,
                                                      IN LPDRMACROSGROUP a_pstGroup,
                                                      IN int a_iIdx);

/** @}*/ /* CPE_DR_MACROSGROUP_MANAGE CPE_DR_宏定义组(macro)管理*/

#ifdef __cplusplus
}
#endif


#endif /* CPE_DR_METALIB_MANAGE_H */
