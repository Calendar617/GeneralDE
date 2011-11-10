#ifndef CPE_DR_XMLTAGS_H
#define CPE_DR_XMLTAGS_H

#define CPE_DR_TAG_METALIB            "metalib"           /**< Ԫ��������XML���ĸ�Ԫ�� */
#define CPE_DR_TAG_INCLUDE            "include"           /**< �ļ��������� */
#define CPE_DR_TAG_MACRO              "macro"             /**< �궨��Ԫ�� */
#define CPE_DR_TAG_MACROSGROUP        "macrosgroup"       /**< �궨���� */
#define CPE_DR_TAG_STRUCT             "struct"            /**< �ṹ��Ԫ�� */
#define CPE_DR_TAG_UNION              "union"             /**< ������Ԫ�� */
#define CPE_DR_TAG_ENTRY              "entry"             /**< Ԫ���ݳ�Ա */
#define CPE_DR_TAG_INDEX              "index"             /**< sql�������� */

#define CPE_DR_TAG_ITEM              "item"            

#define CPE_DR_TAG_TYPELIB             "typelib"

#define CPE_DR_TAG_MACROS              "macros" 
#define CPE_DR_TAG_COMPOSITE_TYPE                "type" 
#define CPE_DR_TAG_CLASS                         "class"       
#define CPE_DR_TAG_ID                 "id"                /**< Ԫ�ص���ֵ��ʶ */
#define CPE_DR_TAG_VERSION            "version"           /**< Ԫ�صİ汾 */
#define CPE_DR_TAG_NAME               "name"              
#define CPE_DR_TAG_CNNAME             "cname"            /**< Ԫ�ص����ı�ʶ */
#define CPE_DR_TAG_DESCIPTION               "desc"              /**< Ԫ��������*/
#define CPE_DR_TAG_DOT                '.'                  /**< Ԫ��path���Էָ���*/
#define CPE_DR_TAG_TAGSET_VERSION         "tagsetversion"   /**< ������XML�ļ���Ԫ����������ǩ���弯�İ汾*/

#define CPE_DR_TAG_INDEX_COLUMN   "column"                 /**< ָ����������ʹ�õ����ݱ����*/
#define CPE_DR_TAG_INDEX_TYPE     "index_type"             /**< ָ���洢���������ݽṹ����*/
#define CPE_DR_TAG_INDEX_UNIQUE   "UNIQUE"                 /**< ָ�����ݱ�������unique����*/

#define CPE_DR_TAG_TYPE               "type"              /**< Ԫ���ݵ��ֶε����� */
#define CPE_DR_TAG_COUNT               "count"         /**< count����*/
#define CPE_DR_TAG_IO                  "io"            /**< io���Ա�ǩ*/
#define CPE_DR_TAG_REFER               "refer"         /**< refer����*/
#define CPE_DR_TAG_THIS		"this"              /**< ָ����ǰԪ��*/
#define CPE_DR_TAG_POINTER_TYPE             '*'         /**< ָ������*/
#define CPE_DR_TAG_REFER_TYPE              '@'         /**<��������*/
#define CPE_DR_TAG_SELECT                  "select"    /**< union��������ѡ���Ա������*/
#define CPE_DR_TAG_SIZEINFO                "sizeinfo"  /**< ָ������ζԸ�Ԫ���ݳ�Ա�Ĵ��������Ϣ���д��*/
#define CPE_DR_TAG_DATASET		"dataset"				/**<������*/

#define CPE_DR_TAG_TARGET                  "target"
#define CPE_DR_TAG_UNIQUE             "unique"            /**< �Ƿ�Ψһ��ȡֵfalse��true,ȱʡfalse����Ψһ */
#define CPE_DR_TAG_NOTNULL            "notnull"           /* �Ƿ�Ϊ�գ�ȡֵfalse��true,ȱʡfalse������Ϊ�� */
#define CPE_DR_TAG_SIZE                "size"              /**< ָ��entry��Ԫ�Ĵ�С*/
#define CPE_DR_TAG_SORTMETHOD          "sortmethod"        /**< ���򷽷�*/
#define CPE_DR_TAG_SORTKEY              "sortkey"           /**< ����ؼ���*/
#define CPE_DR_TAG_DEFAULT_VALUE            "defaultvalue"           /**< ����Ĭ��ֵ */
#define CPE_DR_TAG_MINID              "minid"             /* ѡ�� union entry����Сid */
#define CPE_DR_TAG_MAXID              "maxid"             /* ѡ�� union entry�����id */
#define CPE_DR_TAG_EXTENDTOTABLE		"extendtotable"		/**<��struct��Աչ�����浽���ݿ����*/
#define CPE_DR_TAG_IO_NO_LIMIT	"nolimit" /*���io����ֵΪ��ֵ������������������û������*/
#define CPE_DR_TAG_IO_NO_INPUT 	"noinput" /*���io����ֵΪ��ֵ����������ݲ���xml�ļ�������*/
#define CPE_DR_TAG_IO_NO_OUTPUT 	"nooutput" /*���io����ֵΪ��ֵ����������ݲ������xml�ļ���*/
#define CPE_DR_TAG_IO_NO_IO 	"noio" /*���io����ֵΪ��ֵ����������ݲ��ܽ���xml�ļ��������*/
#define CPE_DR_TAG_BIND_MACROSGROUP		"bindmacrosgroup"				/**<�󶨵ĺ궨����*/
#define CPE_DR_TAG_AUTOINCREMENT	"autoincrement"		/**<auto increment����*/
#define CPE_DR_TAG_CUSTOMATTR		"customattr"		/**<�Զ�������*/

#define CPE_DR_TAG_FALSE              "false"              /**< unique,notnull������ֵ֮һ*/
#define CPE_DR_TAG_TRUE               "true"               /**< unique,notnull������ֵ֮һ*/
#define CPE_DR_TAG_ASC               "asc"             /**< sortmethod������ֵ֮һ����ʾ����������*/
#define CPE_DR_TAG_DESC              "desc"            /**< sortmethod������ֵ֮һ����ʾ����������*/
#define CPE_DR_TAG_NO                "no"              /**< sortmethod������ֵ֮һ����ʾ������*/
#define CPE_DR_TAG_SPLITTABLERULE_BY_MOD		"modulebyfactor"  /**< �ֱ����֮һ����ʾʹ�÷ֱ�����ȡģ�ķ�ʽ�����зֱ�*/



#define CPE_DR_TAG_MACRO_VALUE            "value"           /**< macroԪ�ص����ԣ���ʾ�궨���ֵ */
#define CPE_DR_TAG_MACRO_ID                "id"

#define CPE_DR_TAG_SIZE                "size"              /**< ָ��entry��Ԫ�Ĵ�С*/
#define CPE_DR_TAG_ALIGN               "align"             /**<�ֽڶ��뷽ʽ����Ӧ�ڱ�������\#pragma packָ��*/
#define CPE_DR_TAG_VERSIONINDICATOR    "versionindicator"  /**<versionindicator����ָ���ĸ�Ԫ���ݳ�Ա���Ϊ��Ԫ���ݽṹ�İ汾ָʾ��*/


#define CPE_DR_TAG_PRIMARY_KEY			"primarykey"		/**<ָ���ṹ���Ӧ���ݿ���������Ϣ*/
#define CPE_DR_TAG_SPLITTABLEFACTOR	"splittablefactor"	/**<ָ���ṹ���Ӧ���ݿ��ķֱ�����*/
#define CPE_DR_TAG_SPLITTABLEKEY	"splittablekey"	/**<ָ���ṹ���Ӧ���ݿ��ķֱ�����*/
#define CPE_DR_TAG_SPLITTABLERULE	"splittablerule"	/**<ָ���ṹ���Ӧ���ݿ��ķֱ����*/
#define CPE_DR_TAG_STRICTINPUT	"strictinput"	/**<ָ���ṹ������ʱ�����ϸ�����ģʽ*/
#define CPE_DR_TAG_DEPENDONSTRUCT	"dependonstruct"	/**<ָ���ṹ��������ĳ��Ԫ���ݣ���O/Rmapping��ʹ��*/
#define CPE_DR_TAG_UNIQUEENTRYNAME	"uniqueentryname" /**<ָ���ṹ���Աչ���洢�ǽṹ���Ա���Ƿ�Ψһ*/

#define CPE_DR_TAG_SIZETYPE            "sizetype"          /**< ָ������ζԸ�Ԫ���ݽṹ�Ĵ��������Ϣ���д��*/
#define CPE_DR_TAG_INCLUDE_FILE            "file"           /**< includԪ�ص����ԣ���ʾ�������ļ� */

#endif

