/**
*
* @file     dr_XMLtags.h 
* @brief    Ԫ��������XML��ǩ����
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-03-22 
*
*
* Copyright (c)  2007, ��Ѷ�Ƽ����޹�˾���������з���
* All rights reserved.
*
*/



#ifndef CPE_DR_XMLTAGS_H
#define CPE_DR_XMLTAGS_H

/**@defgroup XMLTag CPE_DR_Ԫ��������XML��ǩ����
* @{
* @note ����XML��ǩ���������˵�����ǵ�ǰ�汾��tagsetversion = 1)����ı�ǩ����
*/

/** @name Ԫ��������Ԫ����
* @{
*/
#define CPE_DR_TAG_METALIB            "metalib"           /**< Ԫ��������XML���ĸ�Ԫ�� */
#define CPE_DR_TAG_INCLUDE            "include"           /**< �ļ��������� */
#define CPE_DR_TAG_MACRO              "macro"             /**< �궨��Ԫ�� */
#define CPE_DR_TAG_MACROSGROUP        "macrosgroup"       /**< �궨���� */
#define CPE_DR_TAG_STRUCT             "struct"            /**< �ṹ��Ԫ�� */
#define CPE_DR_TAG_UNION              "union"             /**< ������Ԫ�� */
#define CPE_DR_TAG_ENTRY              "entry"             /**< Ԫ���ݳ�Ա */
#define CPE_DR_TAG_INDEX              "index"             /**< sql�������� */
/**
*֮ǰXML��ǩ����tagsetversion=0)�����Ԫ���ݳ�Ա
* @deprecated ��Ԫ���ѱ�����CPE_DR_TAG_FIELD��ȡ�����°�XML�ļ��в���ʹ�á�
* @see CPE_DR_TAG_FIELD
*/
#define CPE_DR_TAG_ITEM              "item"            

/**
*֮ǰXML��ǩ����tagsetversion=0)�����Ԫ����������Ԫ��
* @deprecated ��Ԫ���ѱ�����CPE_DR_TAG_METALIB��ȡ�����°�XML�ļ��в���ʹ�á�
* @see CPE_DR_TAG_METALIB
*/
#define CPE_DR_TAG_TYPELIB             "typelib"

/** 
*֮ǰXML��ǩ����tagsetversion=0)����ĺ궨����ı�ǩ
* @deprecated Ŀǰû�к궨����ĸ���˱�ǩֱ��Ԫ�ر�CPE_DR_TAG_MACRO��ȡ�����°�XML�ļ��в���ʹ�á�
*  @par ʹ��˵��:
*  @code
    //tagsetversion=0 �ı�ǩ����
    <macros>
        <macro name="SS_Version" id="99" />
	<macro name="SS_ID_REQ" id="1" />
	<macro name="SS_ID_RES" id="2" />
    </macros>

    //�µĶ���
    <macro name="SS_Version" id="99" />
    <macro name="SS_ID_REQ" id="1" />
    <macro name="SS_ID_RES" id="2" />    
*  @endcode
* @see CPE_DR_TAG_MACRO
*/
#define CPE_DR_TAG_MACROS              "macros" 

/** 
*֮ǰXML��ǩ����tagsetversion=0)������Զ����������͵ı�ǩ
* @deprecated Ŀǰ�Ѿ���CPE_DR_TAG_STRUCT��CPE_DR_TAG_UNION��ȡ�����°�XML�ļ��в���ʹ�á�
*  @par ʹ��˵��:
*  @code
    //struct�ĵȼ۴����ɵĶ��巽ʽΪ��
    <type class="struct" >
        ����
    </type>
    
    //��class��ȱʡֵΪ"struct"��
    <type >
        ����
    </type>

    //�������巽ʽ�����µı�ǩ��������·�ʽ�ȼۣ�
    <struct>
        ����
    </struct>

    //union�ĵȼ۴����ɵĶ��巽ʽΪ��
    <type class="union" >
        ����
    </type>
    
    //�������巽ʽ�����µı�ǩ��������·�ʽ�ȼۣ�
    <union>
        ����
    </union>
*  @endcode
* @see CPE_DR_TAG_STRUCT
* @see CPE_DR_TAG_UNION
*/
#define CPE_DR_TAG_COMPOSITE_TYPE                "type" 
/** 
*֮ǰXML��ǩ����tagsetversion=0)������Զ����������͵ı�ǩ
* @deprecated Ŀǰ�Ѿ���CPE_DR_TAG_STRUCT��CPE_DR_TAG_UNION��ȡ�����°�XML�ļ��в���ʹ�á�
*@ see CPE_DR_TAG_COMPOSITE_TYPE
*/
#define CPE_DR_TAG_CLASS                         "class"       
/** @} */ //Ԫ��������Ԫ����


/**@name Ԫ����Ԫ��ͨ������
* @{
*/ 
#define CPE_DR_TAG_ID                 "id"                /**< Ԫ�ص���ֵ��ʶ */
#define CPE_DR_TAG_VERSION            "version"           /**< Ԫ�صİ汾 */

/** ��Ա����Ԫ�ص��ַ���ʶ 
*@note ��union�ṹ�⣬ͬһ��Ԫ���µ�������Ԫ�ص�name���Ե�ֵ�����ظ���
*/
#define CPE_DR_TAG_NAME               "name"              
#define CPE_DR_TAG_CNNAME             "cname"            /**< Ԫ�ص����ı�ʶ */
#define CPE_DR_TAG_DESCIPTION               "desc"              /**< Ԫ��������*/
#define CPE_DR_TAG_DOT                '.'                  /**< Ԫ��path���Էָ���*/
/** @} */ // Ԫ����Ԫ��ͨ������



/** @name metalibԪ�ص���������
* @{
*/
#define CPE_DR_TAG_TAGSET_VERSION         "tagsetversion"   /**< ������XML�ļ���Ԫ����������ǩ���弯�İ汾*/
/** @ } */  // metalibԪ�ص���������

/** @name indexԪ�ص���������
* @{
*/
#define CPE_DR_TAG_INDEX_COLUMN   "column"                 /**< ָ����������ʹ�õ����ݱ����*/
#define CPE_DR_TAG_INDEX_TYPE     "index_type"             /**< ָ���洢���������ݽṹ����*/
#define CPE_DR_TAG_INDEX_UNIQUE   "UNIQUE"                 /**< ָ�����ݱ�������unique����*/
/** @ } */  // indexԪ�ص���������

/** @name entryԪ�ص���������
* @{
*/
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

/** 
*֮ǰXML��ǩ����tagsetversion=0)�����ָ�����ݳ�Ա������ȵĵı�ǩ
* @deprecated Ŀǰ�Ѿ���CPE_DR_TAG_SIZEINFO��ȡ�����°�XML�ļ��в���ʹ�á�
*  @par �ȼ۴���:
*  @code
    //�ɵĶ��巽ʽΪ��
    <struct name="type1" >
	<entry name="item1" type="int" />
	<entry name="item2" type="int" />
    </struct>

    <struct name="type2" >
	<entry name="BodySize" type="int" />
	<entry name="Body" type="type1" target="BodySize" />
    </struct>


    //�������巽ʽ�����µı�ǩ��������·�ʽ�ȼۣ�
    <struct name="type1" >
	<entry name="item1" type="int" />
	<entry name="item2" type="int" />
    </struct>

    <struct name="type2" >
	<entry name="BodySize" type="int" />
	<entry name="Body" type="type1" sizeinfo="BodySize" />
    </struct>
 
*  @endcode
* @see CPE_DR_TAG_SIZEINFO
*/
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
/** @}*/  //entryԪ�ص���������




/** @name metalibԪ�ص��������Ե�����ֵ��ǩ
* @{
*/
#define CPE_DR_TAG_FALSE              "false"              /**< unique,notnull������ֵ֮һ*/
#define CPE_DR_TAG_TRUE               "true"               /**< unique,notnull������ֵ֮һ*/
#define CPE_DR_TAG_ASC               "asc"             /**< sortmethod������ֵ֮һ����ʾ����������*/
#define CPE_DR_TAG_DESC              "desc"            /**< sortmethod������ֵ֮һ����ʾ����������*/
#define CPE_DR_TAG_NO                "no"              /**< sortmethod������ֵ֮һ����ʾ������*/
#define CPE_DR_TAG_SPLITTABLERULE_BY_MOD		"modulebyfactor"  /**< �ֱ����֮һ����ʾʹ�÷ֱ�����ȡģ�ķ�ʽ�����зֱ�*/
/* @ }*/ 




/** @name macro�궨��Ԫ�ص���������
* @{
*/
#define CPE_DR_TAG_MACRO_VALUE            "value"           /**< macroԪ�ص����ԣ���ʾ�궨���ֵ */

/** 
*֮ǰXML��ǩ����tagsetversion=0)����ı�ʾ�궨��ֵ�����Ա�ǩ
* @deprecated Ŀǰ�Ѿ���CPE_DR_TAG_MACRO_VALUE���°�XML�ļ��в���ʹ�á�
*  @par �ȼ۴���:
*  @code
    //�ɵĶ��巽ʽΪ��
    <macro name ="LOGIN_NAME_LEN" id ="32" >
        ����
    //�����µı�ǩ��������·�ʽ�ȼۣ�
    <macro name ="LOGIN_NAME_LEN" value ="32" >  
*  @endcode
* @see CPE_DR_TAG_MACRO_VALUE
*/
#define CPE_DR_TAG_MACRO_ID                "id"
/** @}*/  //macro�궨��Ԫ�ص���������


/** @name structԪ�ص���������
* @{
*/
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
/** 
*ָ������ζԸ�Ԫ���ݽṹ�Ĵ��������Ϣ���д��
* @deprecated �Ѿ���sizeinfo���������
*/
#define CPE_DR_TAG_SIZETYPE            "sizetype"          /**< ָ������ζԸ�Ԫ���ݽṹ�Ĵ��������Ϣ���д��*/
/*@}*/

/** @name include Ԫ�ص���������
* @{
*/
#define CPE_DR_TAG_INCLUDE_FILE            "file"           /**< includԪ�ص����ԣ���ʾ�������ļ� */

/*@}*/

/* Integer-type attribute */







/** @name ����������������ֵ
* @{
*/
#define	CPE_DR_TAG_TINYINT				"tinyint"
#define	CPE_DR_TAG_TINYUINT			"tinyuint"
#define	CPE_DR_TAG_SMALLINT			"smallint"
#define	CPE_DR_TAG_SMALLUINT			"smalluint"
#define CPE_DR_TAG_INT                "int"
#define CPE_DR_TAG_UINT               "uint"
#define	CPE_DR_TAG_BIGINT				"bigint"
#define	CPE_DR_TAG_BIGUINT				"biguint"
#define	CPE_DR_TAG_INT8				"int8"
#define	CPE_DR_TAG_INT16				"int16"
#define	CPE_DR_TAG_INT32				"int32"
#define	CPE_DR_TAG_INT64				"int64"
#define	CPE_DR_TAG_UINT8				"uint8"
#define	CPE_DR_TAG_UINT16				"uint16"
#define	CPE_DR_TAG_UINT32				"uint32"
#define	CPE_DR_TAG_UINT64				"uint64"
#define CPE_DR_TAG_DATE               "date"
#define CPE_DR_TAG_TIME               "time"
#define CPE_DR_TAG_DATETIME           "datetime"
#define CPE_DR_TAG_MONEY              "money"
#define CPE_DR_TAG_FLOAT              "float"
#define CPE_DR_TAG_DOUBLE             "double"
#define CPE_DR_TAG_STRING             "string"
#define CPE_DR_TAG_BYTE				"byte"
#define CPE_DR_TAG_IP					"ip"        /**<IPv4��ַ*/
#define CPE_DR_TAG_WCHAR				"wchar"
#define CPE_DR_TAG_WSTRING				"wstring"
#define CPE_DR_TAG_DECIMAL				"decimal"
#define CPE_DR_TAG_VOID				"void"

/** 
*֮ǰXML��ǩ����tagsetversion=0)����ı�ʾ�궨��ֵ�����Ա�ǩ
* @deprecated �°�XML�ļ��в���ʹ�á�
*/
#define CPE_DR_TAG_CHAR               "char"
#define CPE_DR_TAG_UCHAR              "uchar"
#define CPE_DR_TAG_SHORT              "short"
#define CPE_DR_TAG_USHORT             "ushort"
#define CPE_DR_TAG_LONG               "long"
#define CPE_DR_TAG_ULONG              "ulong"
#define CPE_DR_TAG_LONGLONG           "longlong"
#define CPE_DR_TAG_ULONGLONG          "ulonglong"
/** @}*/ // ����������������ֵ


/**@} */ // Ԫ����XML��ǩ����


#endif /* CPE_DR_XMLTAGS_H */

