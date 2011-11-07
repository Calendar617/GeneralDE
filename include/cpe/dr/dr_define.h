#ifndef CPE_DR_DEFINE_H
#define CPE_DR_DEFINE_H


/** @name ������������/�������
 *  @{
 */
/*��ʾ�ò���ֻ���������*/
#ifndef IN
#define IN
#endif


/*��ʾ�ò���ֻ���������*/
#ifndef OUT
#define OUT
#endif


/*��ʾ�ò���������������������������*/
#ifndef INOUT
#define INOUT
#endif

/** @}*/ // ������������/�������

/** @name Ԫ������������
 *  @{
 */
#define CPE_DR_MAGIC		0x02D6      /**< Ԫ���ݶ������ļ��е�"ħ��"*/

#define CPE_DR_MAX_LEVEL    32          /*����������Ƕ�����*/

#define	CPE_DR_MAX_VERSION		0x7fffffff		/**< Ԫ���ݿ����汾��*/

#define CPE_DR_NAME_LEN		128		/**< Ԫ�������ִ�����ֽ���*/
#define CPE_DR_DESC_LEN		1024		/**< Ԫ��������������ֽ���*/
#define CPE_DR_MACRO_LEN	128		/**< Ԫ���ݺ궨�����ִ�����ֽ���*/
#define CPE_DR_CHNAME_LEN	512		/**< Ԫ����������������ֽ���*/
#define CPE_DR_DEFAULT_VALUE_LEN	1024	/**< Ԫ����ȱʡֵ����ֽ���*/
/** @}*/ // Ԫ������������

#define CPE_DR_MAX_FILES_IN_ONE_PARSE		128   /**< һ�ν��������XML�ļ���*/





/** @name ֧�ֵ�Ԫ����XML��ǩ���İ汾��Ϣ
 *  @{
 */
#define CPE_DR_XML_TAGSET_VERSION_0             0      /**< ��0�汾*/
#define CPE_DR_SUPPORTING_MIN_XMLTAGSET_VERSION	CPE_DR_XML_TAGSET_VERSION_0       /**< ֧�ֵ����ٰ汾��*/
#define CPE_DR_XML_TAGSET_VERSION_1             1      /**< ��1�汾*/
#define CPE_DR_SUPPORTING_MAX_XMLTAGSET_VERSION	CPE_DR_XML_TAGSET_VERSION_1      /**< ֧�ֵ����汾��*/
/** @}*/ // ֧�ֵ�Ԫ����XML��ǩ���İ汾��Ϣ






/** @name Ԫ��������c����ͷ�ļ�����(���������ͨ����&���ķ�ʽ���)
*  @{
*/
/**��Ա����������ȱʡ�����������ǰ׺�����û������ǰ׺���Ա������ĸ���Сд��
*����ĳ��Ա��DR�е���������ֵΪ"name",������Ϊ"string",����c����ͷ�ļ�������Ϊ:
*	szName
*/
#define CPE_DR_HPPRULE_DEFAULT   0x0000

#define CPE_DR_HPPRULE_NO_TYPE_PREFIX	0x0001	/**<��Ա���������򣺲��������ǰ׺*/

/**��Ա��������������Զ���ǰ׺��������ĳ��Ա��DR�е���������ֵΪ"name",������Ϊ"string",�Զ���ǰ׺Ϊ"m_"
*����c����ͷ�ļ�������Ϊ:	char*	m_Name
*/
#define CPE_DR_HPPRULE_ADD_CUSTOM_PREFIX 0x0002

#define CPE_DR_HPPRULE_NO_LOWERCASE_PREFIX	0x0004	/**<��Ա���������򣺲�ǿ�ƽ���������ĸ���Сд*/

#define CPE_DR_HPPRULE_NO_TYPE_DECLARE		0x0008 /**<ͷ�ļ��в��������ǰ׺*/

#define CPE_DR_HPPRULE_CUSTOMED_PREFIX     0x0010 /**<ʹ��ǰ׺�����ļ�*/
/** @ }*/

#define CPE_DR_MAX_CUSTOM_NAME_PREFIX_LEN	9		/**<�û��Զ�������ǰ׺����󳤶�*/

#define CPE_DR_MAX_CUMTOMATTR_VALUE_LEN	2048	/**<���ݳ�Ա�û��Զ�������ֵ��󳤶�*/

/**@name IO����ֵ
*@ {*/
#define CPE_DR_IO_NOLIMIT		0x00  /**<��ʾ��IO���/���û������*/
#define CPE_DR_IO_NOINPUT		0x01    /**<��ʾ�˳�Ա��������*/
#define CPE_DR_IO_NOOUTPUT		0x02    /**<��ʾ�˳�Ա�������*/
#define CPE_DR_IO_NOIO		0x03    /**< ��ʾ�˳�Ա����������/���*/
#define CPE_DR_IO_MIN_VALUE      CPE_DR_IO_NOLIMIT
#define CPE_DR_IO_MAX_VALUE CPE_DR_IO_NOIO
/*@ }*/

/** @name �������XML��ʽ
*  @{
*/

enum tagDRXmlDataFormat
{
	/**
	�������ļ��������ǣ��ṹ���Ա���ݶ���Ϊ�ӽڵ㣬���ڸ����������͵ĳ�Ա��
	�ӽڵ��Խṹ������������
	*@deprecated Ŀǰ�Ѳ��Ƽ�ʹ�ã��Ƽ�ʹ�����¶����xml���ݸ�ʽ
	*/
	CPE_DR_XML_DATA_FORMAT_LIST_MATA_NAME =1,

	/**
	�������ļ��������ǣ��ṹ���Ա���ݶ���Ϊ�ӽڵ㣬���ڸ����������͵ĳ�Ա��
	�ӽڵ��Գ�Ա����������
	*/
	CPE_DR_XML_DATA_FORMAT_LIST_ENTRY_NAME = 2,

	/**
	�������ļ��������ǣ����ڷ����������͵ĳ�Ա���ӽڵ��Գ�Ա����������
	�ṹ���Ա���ݼ򵥷����ݳ�Ա�����Եķ�ʽ�洢��������Ա�������ӽڵ�ķ�ʽ�洢��
	*/
	CPE_DR_XML_DATA_FORMAT_ATTR_ENTRY_NAME = 3,
};


/**<CPE_DR_XML_DATA_FORMAT_LIST_MATA_NAME�궨��ָ����xml���ݸ�ʽ*/
#define CPE_DR_IO_OLD_XML_VERSION CPE_DR_XML_DATA_FORMAT_LIST_MATA_NAME

/**<CPE_DR_XML_DATA_FORMAT_LIST_ENTRY_NAMEָ����xml���ݸ�ʽ*/
#define CPE_DR_IO_NEW_XML_VERSION CPE_DR_XML_DATA_FORMAT_LIST_ENTRY_NAME


/** @} */

/**@name ���򷽷�
*@ {*/
enum tagDRSortMethod
{
	CPE_DR_SORTMETHOD_NONE_SORT =	0x00,  /**< ������*/
	CPE_DR_SORTMETHOD_ASC_SORT	=	0x01,    /**<��������*/
	CPE_DR_SORTMETHOD_DSC_SORT =0x02,    /**<��������*/
};
/*@ }*/

/*const of type */
#define  CPE_DR_INVALID_INDEX		-1
#define  CPE_DR_INVALID_PTR                -1
#define  CPE_DR_INVALID_OFFSET             -1

#define CPE_DR_INVALID_ID     -1           /**< ��ЧIDֵ*/

#define CPE_DR_INVALID_VERSION -1          /**< ��Ч�汾��*/

#endif /* CPE_DR_DEFINE_H */
