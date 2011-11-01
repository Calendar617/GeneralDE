/**
*
* @file     dr_XMLtags.h 
* @brief    元数据描述XML标签定义
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-03-22 
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/



#ifndef CPE_DR_XMLTAGS_H
#define CPE_DR_XMLTAGS_H

/**@defgroup XMLTag CPE_DR_元数据描述XML标签定义
* @{
* @note 以下XML标签定义除特殊说明都是当前版本（tagsetversion = 1)定义的标签符号
*/

/** @name 元数数描述元素名
* @{
*/
#define CPE_DR_TAG_METALIB            "metalib"           /**< 元数据描述XML树的根元素 */
#define CPE_DR_TAG_INCLUDE            "include"           /**< 文件包含定义 */
#define CPE_DR_TAG_MACRO              "macro"             /**< 宏定义元素 */
#define CPE_DR_TAG_MACROSGROUP        "macrosgroup"       /**< 宏定义组 */
#define CPE_DR_TAG_STRUCT             "struct"            /**< 结构体元素 */
#define CPE_DR_TAG_UNION              "union"             /**< 共用体元素 */
#define CPE_DR_TAG_ENTRY              "entry"             /**< 元数据成员 */
#define CPE_DR_TAG_INDEX              "index"             /**< sql建表索引 */
/**
*之前XML标签集（tagsetversion=0)定义的元数据成员
* @deprecated 此元素已被由于CPE_DR_TAG_FIELD所取代，新版XML文件中不再使用。
* @see CPE_DR_TAG_FIELD
*/
#define CPE_DR_TAG_ITEM              "item"            

/**
*之前XML标签集（tagsetversion=0)定义的元数据描述根元素
* @deprecated 此元素已被由于CPE_DR_TAG_METALIB所取代，新版XML文件中不再使用。
* @see CPE_DR_TAG_METALIB
*/
#define CPE_DR_TAG_TYPELIB             "typelib"

/** 
*之前XML标签集（tagsetversion=0)定义的宏定义组的标签
* @deprecated 目前没有宏定义组的概念，此标签直接元素被CPE_DR_TAG_MACRO所取代，新版XML文件中不再使用。
*  @par 使用说明:
*  @code
    //tagsetversion=0 的标签定义
    <macros>
        <macro name="SS_Version" id="99" />
	<macro name="SS_ID_REQ" id="1" />
	<macro name="SS_ID_RES" id="2" />
    </macros>

    //新的定义
    <macro name="SS_Version" id="99" />
    <macro name="SS_ID_REQ" id="1" />
    <macro name="SS_ID_RES" id="2" />    
*  @endcode
* @see CPE_DR_TAG_MACRO
*/
#define CPE_DR_TAG_MACROS              "macros" 

/** 
*之前XML标签集（tagsetversion=0)定义的自定义数据类型的标签
* @deprecated 目前已经被CPE_DR_TAG_STRUCT和CPE_DR_TAG_UNION所取代，新版XML文件中不再使用。
*  @par 使用说明:
*  @code
    //struct的等价处理。旧的定义方式为：
    <type class="struct" >
        ……
    </type>
    
    //或（class的缺省值为"struct"）
    <type >
        ……
    </type>

    //上述定义方式按照新的标签定义和如下方式等价：
    <struct>
        ……
    </struct>

    //union的等价处理。旧的定义方式为：
    <type class="union" >
        ……
    </type>
    
    //上述定义方式按照新的标签定义和如下方式等价：
    <union>
        ……
    </union>
*  @endcode
* @see CPE_DR_TAG_STRUCT
* @see CPE_DR_TAG_UNION
*/
#define CPE_DR_TAG_COMPOSITE_TYPE                "type" 
/** 
*之前XML标签集（tagsetversion=0)定义的自定义数据类型的标签
* @deprecated 目前已经被CPE_DR_TAG_STRUCT和CPE_DR_TAG_UNION所取代，新版XML文件中不再使用。
*@ see CPE_DR_TAG_COMPOSITE_TYPE
*/
#define CPE_DR_TAG_CLASS                         "class"       
/** @} */ //元数数描述元素名


/**@name 元数据元数通用属性
* @{
*/ 
#define CPE_DR_TAG_ID                 "id"                /**< 元素的数值标识 */
#define CPE_DR_TAG_VERSION            "version"           /**< 元素的版本 */

/** 成员变量元素的字符标识 
*@note 除union结构外，同一父元素下的所有子元素的name属性的值不可重复。
*/
#define CPE_DR_TAG_NAME               "name"              
#define CPE_DR_TAG_CNNAME             "cname"            /**< 元素的中文标识 */
#define CPE_DR_TAG_DESCIPTION               "desc"              /**< 元素描述串*/
#define CPE_DR_TAG_DOT                '.'                  /**< 元素path属性分隔符*/
/** @} */ // 元数据元数通用属性



/** @name metalib元素的特殊属性
* @{
*/
#define CPE_DR_TAG_TAGSET_VERSION         "tagsetversion"   /**< 描述此XML文件中元数据描述标签定义集的版本*/
/** @ } */  // metalib元素的特殊属性

/** @name index元素的特殊属性
* @{
*/
#define CPE_DR_TAG_INDEX_COLUMN   "column"                 /**< 指定建立索引使用的数据表的列*/
#define CPE_DR_TAG_INDEX_TYPE     "index_type"             /**< 指定存储索引的数据结构类型*/
#define CPE_DR_TAG_INDEX_UNIQUE   "UNIQUE"                 /**< 指定数据表索引的unique属性*/
/** @ } */  // index元素的特殊属性

/** @name entry元素的特殊属性
* @{
*/
#define CPE_DR_TAG_TYPE               "type"              /**< 元数据的字段的类型 */
#define CPE_DR_TAG_COUNT               "count"         /**< count属性*/
#define CPE_DR_TAG_IO                  "io"            /**< io属性标签*/
#define CPE_DR_TAG_REFER               "refer"         /**< refer属性*/
#define CPE_DR_TAG_THIS		"this"              /**< 指代当前元素*/
#define CPE_DR_TAG_POINTER_TYPE             '*'         /**< 指针类型*/
#define CPE_DR_TAG_REFER_TYPE              '@'         /**<引用类型*/
#define CPE_DR_TAG_SELECT                  "select"    /**< union类型用来选择成员的属性*/
#define CPE_DR_TAG_SIZEINFO                "sizeinfo"  /**< 指明了如何对该元数据成员的打包长度信息进行打包*/
#define CPE_DR_TAG_DATASET		"dataset"				/**<数据区*/

/** 
*之前XML标签集（tagsetversion=0)定义的指定数据成员打包长度的的标签
* @deprecated 目前已经被CPE_DR_TAG_SIZEINFO所取代，新版XML文件中不再使用。
*  @par 等价处理:
*  @code
    //旧的定义方式为：
    <struct name="type1" >
	<entry name="item1" type="int" />
	<entry name="item2" type="int" />
    </struct>

    <struct name="type2" >
	<entry name="BodySize" type="int" />
	<entry name="Body" type="type1" target="BodySize" />
    </struct>


    //上述定义方式按照新的标签定义和如下方式等价：
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
#define CPE_DR_TAG_UNIQUE             "unique"            /**< 是否唯一，取值false或true,缺省false，不唯一 */
#define CPE_DR_TAG_NOTNULL            "notnull"           /* 是否为空，取值false或true,缺省false，可以为空 */
#define CPE_DR_TAG_SIZE                "size"              /**< 指定entry单元的大小*/
#define CPE_DR_TAG_SORTMETHOD          "sortmethod"        /**< 排序方法*/
#define CPE_DR_TAG_SORTKEY              "sortkey"           /**< 排序关键字*/
#define CPE_DR_TAG_DEFAULT_VALUE            "defaultvalue"           /**< 变量默认值 */
#define CPE_DR_TAG_MINID              "minid"             /* 选择 union entry的最小id */
#define CPE_DR_TAG_MAXID              "maxid"             /* 选择 union entry的最大id */
#define CPE_DR_TAG_EXTENDTOTABLE		"extendtotable"		/**<将struct成员展开保存到数据库表中*/
#define CPE_DR_TAG_IO_NO_LIMIT	"nolimit" /*如果io属性值为此值，则表明数据输入输出没有限制*/
#define CPE_DR_TAG_IO_NO_INPUT 	"noinput" /*如果io属性值为此值，则表明数据不从xml文件中输入*/
#define CPE_DR_TAG_IO_NO_OUTPUT 	"nooutput" /*如果io属性值为此值，则表明数据不输出到xml文件中*/
#define CPE_DR_TAG_IO_NO_IO 	"noio" /*如果io属性值为此值，则表明数据不能进行xml文件输入输出*/
#define CPE_DR_TAG_BIND_MACROSGROUP		"bindmacrosgroup"				/**<绑定的宏定义组*/
#define CPE_DR_TAG_AUTOINCREMENT	"autoincrement"		/**<auto increment属性*/
#define CPE_DR_TAG_CUSTOMATTR		"customattr"		/**<自定义属性*/
/** @}*/  //entry元素的特殊属性




/** @name metalib元素的特殊属性的属性值标签
* @{
*/
#define CPE_DR_TAG_FALSE              "false"              /**< unique,notnull的属性值之一*/
#define CPE_DR_TAG_TRUE               "true"               /**< unique,notnull的属性值之一*/
#define CPE_DR_TAG_ASC               "asc"             /**< sortmethod的属性值之一，表示按升序排序*/
#define CPE_DR_TAG_DESC              "desc"            /**< sortmethod的属性值之一，表示按将序排序*/
#define CPE_DR_TAG_NO                "no"              /**< sortmethod的属性值之一，表示不排序*/
#define CPE_DR_TAG_SPLITTABLERULE_BY_MOD		"modulebyfactor"  /**< 分表规则之一，表示使用分表因子取模的方式来进行分表*/
/* @ }*/ 




/** @name macro宏定义元素的特殊属性
* @{
*/
#define CPE_DR_TAG_MACRO_VALUE            "value"           /**< macro元素的属性，表示宏定义的值 */

/** 
*之前XML标签集（tagsetversion=0)定义的表示宏定义值的属性标签
* @deprecated 目前已经被CPE_DR_TAG_MACRO_VALUE，新版XML文件中不再使用。
*  @par 等价处理:
*  @code
    //旧的定义方式为：
    <macro name ="LOGIN_NAME_LEN" id ="32" >
        ……
    //按照新的标签定义和如下方式等价：
    <macro name ="LOGIN_NAME_LEN" value ="32" >  
*  @endcode
* @see CPE_DR_TAG_MACRO_VALUE
*/
#define CPE_DR_TAG_MACRO_ID                "id"
/** @}*/  //macro宏定义元素的特殊属性


/** @name struct元素的特殊属性
* @{
*/
#define CPE_DR_TAG_SIZE                "size"              /**< 指定entry单元的大小*/
#define CPE_DR_TAG_ALIGN               "align"             /**<字节对齐方式，对应于编译器的\#pragma pack指令*/
#define CPE_DR_TAG_VERSIONINDICATOR    "versionindicator"  /**<versionindicator属性指明哪个元数据成员会成为本元数据结构的版本指示器*/


#define CPE_DR_TAG_PRIMARY_KEY			"primarykey"		/**<指明结构体对应数据库表的主键信息*/
#define CPE_DR_TAG_SPLITTABLEFACTOR	"splittablefactor"	/**<指明结构体对应数据库表的分表因子*/
#define CPE_DR_TAG_SPLITTABLEKEY	"splittablekey"	/**<指明结构体对应数据库表的分表主键*/
#define CPE_DR_TAG_SPLITTABLERULE	"splittablerule"	/**<指明结构体对应数据库表的分表规则*/
#define CPE_DR_TAG_STRICTINPUT	"strictinput"	/**<指明结构体输入时采用严格输入模式*/
#define CPE_DR_TAG_DEPENDONSTRUCT	"dependonstruct"	/**<指明结构体依赖于某个元数据，在O/Rmapping中使用*/
#define CPE_DR_TAG_UNIQUEENTRYNAME	"uniqueentryname" /**<指明结构体成员展开存储是结构体成员名是否唯一*/
/** 
*指明了如何对该元数据结构的打包长度信息进行打包
* @deprecated 已经被sizeinfo属性所替代
*/
#define CPE_DR_TAG_SIZETYPE            "sizetype"          /**< 指明了如何对该元数据结构的打包长度信息进行打包*/
/*@}*/

/** @name include 元素的特殊属性
* @{
*/
#define CPE_DR_TAG_INCLUDE_FILE            "file"           /**< includ元素的属性，表示包含的文件 */

/*@}*/

/* Integer-type attribute */







/** @name 内置数据类型属性值
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
#define CPE_DR_TAG_IP					"ip"        /**<IPv4地址*/
#define CPE_DR_TAG_WCHAR				"wchar"
#define CPE_DR_TAG_WSTRING				"wstring"
#define CPE_DR_TAG_DECIMAL				"decimal"
#define CPE_DR_TAG_VOID				"void"

/** 
*之前XML标签集（tagsetversion=0)定义的表示宏定义值的属性标签
* @deprecated 新版XML文件中不再使用。
*/
#define CPE_DR_TAG_CHAR               "char"
#define CPE_DR_TAG_UCHAR              "uchar"
#define CPE_DR_TAG_SHORT              "short"
#define CPE_DR_TAG_USHORT             "ushort"
#define CPE_DR_TAG_LONG               "long"
#define CPE_DR_TAG_ULONG              "ulong"
#define CPE_DR_TAG_LONGLONG           "longlong"
#define CPE_DR_TAG_ULONGLONG          "ulonglong"
/** @}*/ // 内置数据类型属性值


/**@} */ // 元数据XML标签定义


#endif /* CPE_DR_XMLTAGS_H */

