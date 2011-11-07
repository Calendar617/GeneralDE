#ifndef CPE_DR_METALIB_MANAGE_H
#define CPE_DR_METALIB_MANAGE_H
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_external.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	
/** @defgroup CPE_DR_META_MANAGE CPE_DR_Ԫ����(meta)����
*  @{
*/


/** ����Ԫ��������Ԫ���ݿ��в��Ҵ�Ԫ����
*@param[in] a_pstLib Ԫ���ݿ�ָ��
*@param[in] a_pszName Ԫ�������ִ�ָ��
*
*@return ����ҵ��򷵻�Ԫ���ݵĳ�Ա�����򷵻�NULL
*
* @pre \e a_pstLib ����Ϊ NULL
* @pre \e a_pszName ����Ϊ NULL
*/
CPE_DR_API LPDRMETA dr_get_meta_by_name(IN LPDRMETALIB a_pstLib, IN const char* a_pszName);

/**����Ԫ���ݵ�ID��Ԫ���ݿ��в��Ҵ�Ԫ����
*@param[in] a_pstLib Ԫ���ݿ�ָ��
*@param[in] a_iID Ԫ���ݵ�ID
*
*@return ����ҵ��򷵻�Ԫ���ݵĳ�Ա�����򷵻�NULL
*
* @pre \e a_pstLib ����Ϊ NULL
*/
CPE_DR_API LPDRMETA dr_get_meta_by_id(IN LPDRMETALIB a_pstLib, IN int a_iID);


/** ��ȡԪ���������Ļ�׼�汾��
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return success: Ԫ���������Ļ�׼�汾�� failed: -1
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
CPE_DR_API int dr_get_meta_based_version(IN LPDRMETA a_pstMeta);



/**
*��ȡԪ���������ĵ�ǰ�汾��
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return success:Ԫ���������ĵ�ǰ�汾�� failed: -1*
* @pre \e a_pstMeta ����Ϊ NULL
*/
CPE_DR_API int dr_get_meta_current_version(IN LPDRMETA a_pstMeta);

/**
*��ȡԪ���������Ļ�����������ID
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return ��������ID: CPE_DR_TYPE_UNION �� CPE_DR_TYPE_STRUCT
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
CPE_DR_API int dr_get_meta_type(IN LPDRMETA a_pstMeta);

/**��ȡԪ��������������
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return Ԫ���������ַ�����ָ��
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
CPE_DR_API const char *dr_get_meta_name(IN LPDRMETA a_pstMeta);

CPE_DR_API const char *dr_get_meta_desc(IN LPDRMETA a_pstMeta);

CPE_DR_API int dr_get_meta_id(IN LPDRMETA a_pstMeta);

/**
*��ȡ����Ԫ��������(meta)�Ĵ洢�ռ�
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return success: ����Ԫ��������(meta)�Ĵ洢���ֽ��� failed : -1
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
CPE_DR_API size_t dr_get_meta_size(IN LPDRMETA a_pstMeta);

/**
*��ȡ����Ԫ�������ͳ�ԱԪ�صĸ���
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return ���Idֵ
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
CPE_DR_API int dr_get_entry_num(IN LPDRMETA a_pstMeta);


/** ����targetԪ��·����ȡ��Ԫ����meta�е�ƫ��
*@param[out] a_pstRedirector ��Ԫ�ص�ƫ��
*@param[in] a_pstMeta pstEntry���ڵ��Զ���ṹ
*@param[in] a_iEntry ��ǰiEntry��Ԫ���в���
*@param[in] a_pszName Ҫ���������
*
*@return �ɹ�����CPE_DR_SUCCESS�����򷵻ش������
*
*@pre \e a_piOff ����ΪNULL
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_piUnit ����ΪNULL
*@pre \e a_pszName ����ΪNULL
*@pre \e �����Ȼ�ȡԪ�ص�type,count ,refer����
*/
int dr_sizeinfo_to_off(LPDRSIZEINFO a_pstRedirector, LPDRMETA a_pstMeta, int a_iEntry, const char* a_pszName);
#define dr_sizeinfo_to_off_i   dr_sizeinfo_to_off

/**���ݳ�Ա�ڽṹ���е�·��(Path)��Ϣ��ȡ�˳�Ա���ڽṹ���׵�ַ��ƫ��ֵ
*@param[in] a_pstMeta ��Ҫ��λ�Ľṹ��Ԫ�����������
*@param[in,out] a_ppstEntry �����Ӧ��Ա�����������ָ��
*@param[out] a_piHOff �����Ӧ��Ա��ƫ��ֵ
*@param[in] a_pszPath  ����˳�Ա·����Ϣ�Ļ�����
*
*return  �ɹ�����0�����򷵻ظ���
*
*@pre \e a_pszBuff ����ΪNULL
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_iBuff �������0
*@pre \e a_iOff ������ڵ���0
*@pre \e a_pszPath ����ΪNULL
*/
CPE_DR_API int dr_entry_path_to_off(IN LPDRMETA a_pstMeta, INOUT LPDRMETAENTRY *a_ppstEntry, OUT DROFF *a_piHOff, IN const char *a_pszPath);


/**���ݳ�Ա�ڽṹ���е�ƫ�ƶ�λ�˳�Ա�ڽṹ�е�·����Ϣ
*@param[in] a_pstMeta ��Ҫ��λ�Ľṹ��Ԫ�����������
*@param[in] a_iOff  ��Ա��ƫ��
*
*@note ·����Ϣ�������ھ�̬�������У��ٴε��ñ��ӿ�ʱ���ϴε��û�ȡ��·����Ϣ������д
*
*return  �ɹ�����0�����򷵻ظ���
*
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_iOff ������ڵ���0
*/
CPE_DR_API char *dr_entry_off_to_path(IN LPDRMETA a_pstMeta, IN int a_iOff, char * a_pBuf, size_t a_iBufSize);


/**��ѯ�˽ṹ���Ƿ�����������������Եĳ�Ա
*@param[in] a_pstMeta �ṹ���Ա��Ԫ�����������
*
*@pre \e a_pstMeta ����ΪNULL
*
*@return ������������Գ�Ա���򷵻ط���ֵ�����򷵻�0
*/
CPE_DR_API int dr_do_have_autoincrement_entry(IN LPDRMETA a_pstMeta);

/** @}*/ /* CPE_DR_META_MANAGE CPE_DR_Ԫ����(meta)����*/


/** @defgroup CPE_DR_ENTRY_MANAGE CPE_DR_���ݳ�Ա(entry)����
*  @{
*/

/**��ȡԪ��������������
*@param[in] a_pstEntry Ԫ����������ָ��
*
*@return Ԫ���������ַ�����ָ��
*
* @pre \e a_pstEntry ����Ϊ NULL
*/
CPE_DR_API const char *dr_get_entry_name(IN LPDRMETAENTRY a_pstEntry);

/**��ȡԪ����������cname
 *@param[in] a_pstEntry Ԫ����������ָ��
 *
 *@return Ԫ���������ַ�����ָ��
 *
 * @pre \e a_pstEntry ����Ϊ NULL
 */
CPE_DR_API const char *dr_get_entry_cname(IN LPDRMETAENTRY a_pstEntry);

/**��ȡԪ����������desc�����ַ���
 *@param[in] a_pstEntry Ԫ����������ָ��
 *
 *@return Ԫ����desc�����ַ�����ָ��
 *
 * @pre \e a_pstEntry ����Ϊ NULL
 */
CPE_DR_API const char *dr_get_entry_desc(IN LPDRMETAENTRY a_pstEntry);

/**��ȡԪ���������󶨵�macrosgroup��ָ��
 *@param[in] a_pstEntry Ԫ����������ָ��
 *
 *@return ���Ԫ���ݰ���macrosgroup, ����Ԫ���ݰ󶨵�macrosgroup��ָ��; ���򷵻�NULL
 *
 * @pre \e a_pstEntry ����Ϊ NULL
 */
CPE_DR_API LPDRMACROSGROUP dr_get_entry_macrosgroup(IN LPDRMETAENTRY a_pstEntry);

/**����Ԫ���ݳ�Ա����Ԫ���ݿ��в��Ҵ˳�Ա�ľ��
 *@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
 *@param[in] a_pszName entry������
 *
 *@return �������ָ�����ֵĳ�Ա���򷵻ش˳�Ա�ľ�������򷵻�NULL
 *
 *@pre \e a_pstMeta ����ΪNULL
 *@pre \e a_pszName ����Ϊ NULL
 */
CPE_DR_API LPDRMETAENTRY dr_get_entryptr_by_name(IN LPDRMETA a_pstMeta, IN const char* a_pszName);

/**����Ԫ���ݳ�Ա����Ԫ���ݿ��в��Ҵ˳�Ա�����ڸ�Ԫ���ݳ�Ա�����е��±���ʽ���أ�
*@param[out] a_piIdx ����˳�Ա������
*@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
*@param[in] a_pszName Ԫ�������ִ�ָ��
*
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*
* @pre \e a_piIdx ����Ϊ NULL
*@pre \e a_pstMeta ����ΪNULL
* @pre \e a_pszName ����Ϊ NULL
*/
CPE_DR_API int dr_get_entry_by_name(OUT int* a_piIdx, IN LPDRMETA a_pstMeta, IN const char* a_pszName);

/**����Ԫ���ݳ�Ա��ID��Ԫ���ݿ��в��Ҵ˳�Ա�����ڸ�Ԫ���ݳ�Ա�����е��±���ʽ���أ�
*���Ԫ������union�ṹ�������a_iId��ĳ��Ա[minid, maxid]����֮�䣬��ѡ��˳�Ա������
*ѡ��ȱʡ��Ա��������ڵĻ���
*@param[out] a_piIdx ����˳�Ա������
*@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
*@param[in] a_iId Ԫ���ݵ�ID
*
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������
*@retval 0   ����ɹ�
*
* @pre \e a_piIdx ����Ϊ NULL
*@pre \e a_pstMeta ����ΪNULL
*/
CPE_DR_API int dr_get_entry_by_id(OUT int* a_piIdx, IN LPDRMETA a_pstMeta, IN int a_iId);

/**����Ԫ���ݳ�Ա��Ԫ���ݽṹ�е�������ȡ�˳�Ա�����ľ��
*@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
*@param[in] a_idxEntry entry������
*
*@return ���������ȷ,�򷵻ش˳�Ա�����ľ��,���򷵻�NULL
*
*@pre \e a_pstMeta ����ΪNULL
*@see dr_get_meta_by_id
*@see dr_get_entry_by_id
*/
CPE_DR_API LPDRMETAENTRY dr_get_entry_by_index(IN LPDRMETA a_pstMeta, IN int a_idxEntry);

/**��ȡentryԪ��id����ֵ
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*@note ͨ������dr_get_entry_by_index���Ի�ȡentryԪ�������ṹ��ָ��
*
*@return entryԪ��id����ֵ
*
*@pre \e a_pstEntry ����ΪNULL
*@see dr_get_entry_by_index
*/
CPE_DR_API int dr_get_entry_id(IN LPDRMETAENTRY a_pstEntry);

/**��ȡentryԪ��id����ֵ��Ӧ�ĺ궨�������
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*@param[in] a_pstLib Ԫ�����������ָ��
*
*@note ͨ������dr_get_entry_by_index���Ի�ȡentryԪ�������ṹ��ָ��
*
*@return ����˳�Ա��id����ֵΪ�궨��,�򷵻ش˺궨�������;���򷵻ؿմ�""
*
*@pre \e a_pstEntry ����ΪNULL
*@pre \e a_pstLib ����ΪNULL
*@see dr_get_entry_by_index
*/
CPE_DR_API const char *dr_get_entry_id_name(IN LPDRMETALIB a_pstLib, IN LPDRMETAENTRY a_pstEntry);


/**��ȡ����entry��Ա�ı��ش洢�ռ��С
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*@note ͨ������dr_get_entry_by_index���Ի�ȡentryԪ�������ṹ��ָ��
*
*@return entryԪ�ص��������ı��ش洢�ռ��С
*
*@pre \e a_pstEntry ����ΪNULL
*@see dr_get_entry_by_index
*/
CPE_DR_API size_t dr_get_entry_unitsize(IN LPDRMETAENTRY a_pstEntry);

/**��ȡentry����������ID
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*
*@return ���س�Ա����������ID������DR֧�ֵ�����ID������μ�dr_ctypes_info.h��"������������"
*
*/
CPE_DR_API int dr_get_entry_type(IN LPDRMETAENTRY a_pstEntry);

/**��ȡentry�����������Ϣ
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*
*@retval 0	��ʾ�˳�ԱΪ�ɱ䳤������
*@retval 1	��ʾ�˳�Ա��������
*@retval >1 ��ʾ�˳�ԱΪ�̶����ȵ�����
*
*/
CPE_DR_API int dr_get_entry_count(IN LPDRMETAENTRY a_pstEntry);

/**��ȡentryԪ����ָmeta�ṹ������ָ��
* ���entryԪ��Ϊ������������ʱ,ͨ���˽ӿڿ��Ի�ȡ��entry�����������͵������ṹ��ָ��
*@param[in] a_pstLib	Ԫ�����������ָ��
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*
*@return ���entryԪ��Ϊ������������ʱ,������ָ������������������ָ��;���򷵻�NULL
*
*/
CPE_DR_API LPDRMETA dr_get_entry_type_meta(IN LPDRMETALIB a_pstLib, IN LPDRMETAENTRY a_pstEntry);

/**����Ԫ���ݳ�Ա����·����Ԫ���ݿ��в��Ҵ˳�Ա
*@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
*@param[in] a_pszEntryPath entry�ڽṹ�е�����·����
*
*@return �������ָ�����ֵĳ�Ա���򷵻ش˳�Ա�ľ�������򷵻�NULL
*
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_pszEntryPath ����Ϊ NULL*/
CPE_DR_API LPDRMETAENTRY dr_get_entry_by_path(IN LPDRMETA a_pstMeta, IN const char* a_pszEntryPath);

/**��ȡ��Ա���Զ�������ֵ
*@param[in] a_pstEntry ��Ա��Ԫ�����������
*@param[in] a_pstLib	Ԫ�����������ָ��
*
*@return ����˳�Ա������customattr����,�򷵻ش����Ե��׵�ַ;���򷵻�""
*
*@pre \e a_pstEntry ����ΪNULL
*@pre \e a_pstLib ����ΪNULL
*/
CPE_DR_API const char *dr_get_entry_customattr(IN LPDRMETALIB a_pstLib, IN LPDRMETAENTRY a_pstEntry);


/** @}*/ /* CPE_DR_ENTRY_MANAGE CPE_DR_���ݳ�Ա(entry)����*/

/** @defgroup CPE_DR_MACRO_MANAGE CPE_DR_�궨��(macro)����
*  @{
*/		 
/** ���ݺ����������ֵ
*@param[out] a_piID ��ú궨�����ֵ
*@param[in] a_pstLib Ԫ���ݿ�
*@param[in] a_pszName ����Һ������
*
*@return �ɹ�����0�����򷵻ط�0
*
*@pre \e a_piID ����ΪNULL
*@pre \e a_pstLib ����ΪNULL
*@pre \e a_pszName ����ΪNULL
*/
CPE_DR_API int dr_get_macro_value(OUT int *a_piID, IN LPDRMETALIB a_pstLib, IN const  char *a_pszName);

/** ��ȡa_pstLib�к궨�������������macrosgroup�ж���ĺ꣩
 *@param[in] a_pstLib Ԫ���ݿ�
 *
 *@return ���ж���ĺ������
 *
 *@pre \e a_pstLib ����ΪNULL
 */
CPE_DR_API int dr_get_metalib_macro_num(IN LPDRMETALIB a_pstLib);

/** ���ݺ��index�����ָ��
 *@param[in] a_pstLib Ԫ���ݿ�
 *@param[in] a_iIdx ����Һ��index
 *
 *@return �ɹ����غ�ָ�룬���򷵻�NULL
 *
 *@pre \e a_pstLib ����ΪNULL
 *@pre \e a_iIdx ����>=0����С��a_pstLib�еĺ������
 */
CPE_DR_API LPDRMACRO dr_get_metalib_macro_by_index(IN LPDRMETALIB a_pstLib, IN int a_iIdx);

/** ���ݺ�ָ����������
 *@param[in] a_pstLib Ԫ���ݿ�
 *@param[in] a_pstMacro ��ָ��
 *
 *@return ָ������ֵ�ָ��
 *
 *@pre \e a_pstLib ����ΪNULL
 *@pre \e a_pstMacro ����ΪNULL
 */
CPE_DR_API const char* dr_get_macro_name_by_ptr(IN LPDRMETALIB a_pstLib, IN LPDRMACRO a_pstMacro);

/** ���ݺ�ָ������ֵ
 *@param[out] a_piID ��ú궨�����ֵ
 *@param[in] a_pstMacro ��ָ��
 *
 *@return �ɹ����� 0�����򷵻� !0
 *
 *@pre \e a_piID ����ΪNULL
 *@pre \e a_pstMacro ����ΪNULL
 */
CPE_DR_API int dr_get_macro_value_by_ptr(OUT int *a_piID, IN LPDRMACRO a_pstMacro);

/** ���ݺ�ָ������desc�ַ���
 *@param[in] a_pstLib Ԫ���ݿ�
 *@param[in] a_pstMacro ��ָ��
 *
 *@return ָ����desc�����ַ�����ָ��
 *
 *@pre \e a_pstLib ����ΪNULL
 *@pre \e a_pstMacro ����ΪNULL
 */
CPE_DR_API const char* dr_get_macro_desc_by_ptr(IN LPDRMETALIB a_pstLib, IN LPDRMACRO a_pstMacro);

/**��ȡ�����ڵ�macrosgroup��ָ��
 *@param[in] a_pstLib Ԫ���ݿ�
 *@param[in] a_pstMacro ��ָ��
 *
 *@return ���������ĳ��macrosgroup, ����ָ���macrosgroup��ָ��; ���򷵻�NULL
 *
 *@pre \e a_pstLib ����ΪNULL
 *@pre \e a_pstMacro ����Ϊ NULL
 *
 *@note �����㷨�Ǽ򵥵ı������������O(n*n)
 */
CPE_DR_API LPDRMACROSGROUP dr_get_macro_macrosgroup(IN LPDRMETALIB a_pstLib,
                                                   IN LPDRMACRO a_pstMacro);


/** @}*/ /* CPE_DR_MACRO_MANAGE CPE_DR_�궨��(macro)����*/

/** @defgroup CPE_DR_MACROSGROUP_MANAGE CPE_DR_�궨����(macrosgroup)����
 *  @{
 */

/** ��ȡԪ���ݿ��ж����macrosgroup������
 *@param[in] a_pstLib Ԫ���ݿ�
 *
 *@return Ԫ���ݿ��ж����macrosgroup������
 *
 *@pre \e a_pstLib ����ΪNULL
 */
CPE_DR_API int dr_get_macrosgroup_num(IN LPDRMETALIB a_pstLib);

/** ���ݺ궨�����index��ȡ��ָ��
 *@param[in] a_pstLib Ԫ���ݿ�
 *@param[in] a_iIdx �궨�����index
 *
 *@return ָ��궨�����ָ��
 *
 *@pre \e a_pstLib ����ΪNULL
 *@pre \e a_iIdx ����>=0, ��С��a_pstLib�еĺ궨���������
 */
LPDRMACROSGROUP dr_get_macrosgroup_by_index(IN LPDRMETALIB a_pstLib, IN int a_iIdx);

/** ��ȡ�궨����a_pstGroup������
 *@param[in] a_pstGroup �궨����ָ��
 *
 *@return ָ��궨����������ַ�����ָ��
 *
 *@pre \e a_pstGroup ����ΪNULL
 */
const char* dr_get_macrosgroup_name(IN LPDRMACROSGROUP a_pstGroup);

/** ��ȡ�궨����a_pstGroup�а����ĺ궨�������
 *@param[in] a_pstGroup �궨����ָ��
 *
 *@return �궨�������
 *
 *@pre \e a_pstGroup ����ΪNULL
 */
CPE_DR_API int dr_get_macrosgroup_macro_num(IN LPDRMACROSGROUP a_pstGroup);

/** ��ȡ�궨����a_pstGroup��index��Ӧ�ĺ궨��
 *@param[in] a_pstLib Ԫ���ݿ�
 *@param[in] a_pstGroup �궨����ָ��
 *@param[in] a_iIdx �궨�����index
 *
 *@return ָ��궨���ָ��
 *
 *@pre \e a_pstLib ����ΪNULL
 *@pre \e a_pstGroup ����ΪNULL
 *@pre \e a_iIdx ����>=0, ��С��a_pstGroup�еĺ궨�������
 */
CPE_DR_API LPDRMACRO dr_get_macrosgroup_macro_by_index(IN LPDRMETALIB a_pstLib,
                                                      IN LPDRMACROSGROUP a_pstGroup,
                                                      IN int a_iIdx);

/** @}*/ /* CPE_DR_MACROSGROUP_MANAGE CPE_DR_�궨����(macro)����*/

#ifdef __cplusplus
}
#endif


#endif /* CPE_DR_METALIB_MANAGE_H */
