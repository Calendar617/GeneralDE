#ifndef CPE_DR_XMLTAGS_H
#define CPE_DR_XMLTAGS_H

#define CPE_DR_TAG_METALIB            "metalib"           /**< 元数据描述XML树的根元素 */
#define CPE_DR_TAG_INCLUDE            "include"           /**< 文件包含定义 */
#define CPE_DR_TAG_MACRO              "macro"             /**< 宏定义元素 */
#define CPE_DR_TAG_MACROSGROUP        "macrosgroup"       /**< 宏定义组 */
#define CPE_DR_TAG_STRUCT             "struct"            /**< 结构体元素 */
#define CPE_DR_TAG_UNION              "union"             /**< 共用体元素 */
#define CPE_DR_TAG_ENTRY              "entry"             /**< 元数据成员 */
#define CPE_DR_TAG_INDEX              "index"             /**< sql建表索引 */

#define CPE_DR_TAG_ITEM              "item"            

#define CPE_DR_TAG_TYPELIB             "typelib"

#define CPE_DR_TAG_MACROS              "macros" 
#define CPE_DR_TAG_COMPOSITE_TYPE                "type" 
#define CPE_DR_TAG_CLASS                         "class"       
#define CPE_DR_TAG_ID                 "id"                /**< 元素的数值标识 */
#define CPE_DR_TAG_VERSION            "version"           /**< 元素的版本 */
#define CPE_DR_TAG_NAME               "name"              
#define CPE_DR_TAG_CNNAME             "cname"            /**< 元素的中文标识 */
#define CPE_DR_TAG_DESCIPTION               "desc"              /**< 元素描述串*/
#define CPE_DR_TAG_DOT                '.'                  /**< 元素path属性分隔符*/
#define CPE_DR_TAG_TAGSET_VERSION         "tagsetversion"   /**< 描述此XML文件中元数据描述标签定义集的版本*/

#define CPE_DR_TAG_INDEX_COLUMN   "column"                 /**< 指定建立索引使用的数据表的列*/
#define CPE_DR_TAG_INDEX_TYPE     "index_type"             /**< 指定存储索引的数据结构类型*/
#define CPE_DR_TAG_INDEX_UNIQUE   "UNIQUE"                 /**< 指定数据表索引的unique属性*/

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

#define CPE_DR_TAG_FALSE              "false"              /**< unique,notnull的属性值之一*/
#define CPE_DR_TAG_TRUE               "true"               /**< unique,notnull的属性值之一*/
#define CPE_DR_TAG_ASC               "asc"             /**< sortmethod的属性值之一，表示按升序排序*/
#define CPE_DR_TAG_DESC              "desc"            /**< sortmethod的属性值之一，表示按将序排序*/
#define CPE_DR_TAG_NO                "no"              /**< sortmethod的属性值之一，表示不排序*/
#define CPE_DR_TAG_SPLITTABLERULE_BY_MOD		"modulebyfactor"  /**< 分表规则之一，表示使用分表因子取模的方式来进行分表*/



#define CPE_DR_TAG_MACRO_VALUE            "value"           /**< macro元素的属性，表示宏定义的值 */
#define CPE_DR_TAG_MACRO_ID                "id"

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

#define CPE_DR_TAG_SIZETYPE            "sizetype"          /**< 指明了如何对该元数据结构的打包长度信息进行打包*/
#define CPE_DR_TAG_INCLUDE_FILE            "file"           /**< includ元素的属性，表示包含的文件 */

#endif

