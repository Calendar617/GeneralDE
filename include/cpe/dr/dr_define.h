#ifndef CPE_DR_DEFINE_H
#define CPE_DR_DEFINE_H


/** @name 函数参数输入/输出属性
 *  @{
 */
/*表示该参数只是输入参数*/
#ifndef IN
#define IN
#endif


/*表示该参数只是输出参数*/
#ifndef OUT
#define OUT
#endif


/*表示该参数既是输入参数，又是输出参数*/
#ifndef INOUT
#define INOUT
#endif

/** @}*/ // 函数参数输入/输出属性

/** @name 元数据描述常量
 *  @{
 */
#define CPE_DR_MAGIC		0x02D6      /**< 元数据二进制文件中的"魔数"*/

#define CPE_DR_MAX_LEVEL    32          /*组合类型最大嵌套深度*/

#define	CPE_DR_MAX_VERSION		0x7fffffff		/**< 元数据库最大版本号*/

#define CPE_DR_NAME_LEN		128		/**< 元数据名字串最大字节数*/
#define CPE_DR_DESC_LEN		1024		/**< 元数据描述串最大字节数*/
#define CPE_DR_MACRO_LEN	128		/**< 元数据宏定义名字串最大字节数*/
#define CPE_DR_CHNAME_LEN	512		/**< 元数据中文名串最大字节数*/
#define CPE_DR_DEFAULT_VALUE_LEN	1024	/**< 元数据缺省值最大字节数*/
/** @}*/ // 元数据描述常量

#define CPE_DR_MAX_FILES_IN_ONE_PARSE		128   /**< 一次解析的最大XML文件数*/





/** @name 支持的元数据XML标签集的版本信息
 *  @{
 */
#define CPE_DR_XML_TAGSET_VERSION_0             0      /**< 第0版本*/
#define CPE_DR_SUPPORTING_MIN_XMLTAGSET_VERSION	CPE_DR_XML_TAGSET_VERSION_0       /**< 支持的最少版本号*/
#define CPE_DR_XML_TAGSET_VERSION_1             1      /**< 第1版本*/
#define CPE_DR_SUPPORTING_MAX_XMLTAGSET_VERSION	CPE_DR_XML_TAGSET_VERSION_1      /**< 支持的最大版本号*/
/** @}*/ // 支持的元数据XML标签集的版本信息






/** @name 元数据生成c语言头文件规则(各规则可以通过‘&’的方式组合)
*  @{
*/
/**成员名命名规则缺省规则：添加类型前缀，如果没有类型前缀则成员名首字母变成小写。
*例如某成员在DR中的名字属性值为"name",其类型为"string",则在c语言头文件中名字为:
*	szName
*/
#define CPE_DR_HPPRULE_DEFAULT   0x0000

#define CPE_DR_HPPRULE_NO_TYPE_PREFIX	0x0001	/**<成员名命名规则：不添加类型前缀*/

/**成员名命名规则：添加自定义前缀规则：例如某成员在DR中的名字属性值为"name",其类型为"string",自定义前缀为"m_"
*则在c语言头文件中名字为:	char*	m_Name
*/
#define CPE_DR_HPPRULE_ADD_CUSTOM_PREFIX 0x0002

#define CPE_DR_HPPRULE_NO_LOWERCASE_PREFIX	0x0004	/**<成员名命名规则：不强制将成名首字母变成小写*/

#define CPE_DR_HPPRULE_NO_TYPE_DECLARE		0x0008 /**<头文件中不添加类型前缀*/

#define CPE_DR_HPPRULE_CUSTOMED_PREFIX     0x0010 /**<使用前缀配置文件*/
/** @ }*/

#define CPE_DR_MAX_CUSTOM_NAME_PREFIX_LEN	9		/**<用户自定义名字前缀的最大长度*/

#define CPE_DR_MAX_CUMTOMATTR_VALUE_LEN	2048	/**<数据成员用户自定义属性值最大长度*/

/**@name IO属性值
*@ {*/
#define CPE_DR_IO_NOLIMIT		0x00  /**<表示其IO输出/输出没有限制*/
#define CPE_DR_IO_NOINPUT		0x01    /**<表示此成员不能输入*/
#define CPE_DR_IO_NOOUTPUT		0x02    /**<表示此成员不能输出*/
#define CPE_DR_IO_NOIO		0x03    /**< 表示此成员不参与输入/输出*/
#define CPE_DR_IO_MIN_VALUE      CPE_DR_IO_NOLIMIT
#define CPE_DR_IO_MAX_VALUE CPE_DR_IO_NOIO
/*@ }*/

/** @name 输入输出XML格式
*  @{
*/

enum tagDRXmlDataFormat
{
	/**
	此数据文件的特征是：结构体成员数据都作为子节点，对于复合数据类型的成员，
	子节点以结构体名进行命名
	*@deprecated 目前已不推荐使用，推荐使用以下定义的xml数据格式
	*/
	CPE_DR_XML_DATA_FORMAT_LIST_MATA_NAME =1,

	/**
	此数据文件的特征是：结构体成员数据都作为子节点，对于复合数据类型的成员，
	子节点以成员名进行命名
	*/
	CPE_DR_XML_DATA_FORMAT_LIST_ENTRY_NAME = 2,

	/**
	此数据文件的特征是：对于符合数据类型的成员，子节点以成员名进行命名
	结构体成员数据简单非数据成员以属性的方式存储，其他成员数据以子节点的方式存储，
	*/
	CPE_DR_XML_DATA_FORMAT_ATTR_ENTRY_NAME = 3,
};


/**<CPE_DR_XML_DATA_FORMAT_LIST_MATA_NAME宏定义指定的xml数据格式*/
#define CPE_DR_IO_OLD_XML_VERSION CPE_DR_XML_DATA_FORMAT_LIST_MATA_NAME

/**<CPE_DR_XML_DATA_FORMAT_LIST_ENTRY_NAME指定的xml数据格式*/
#define CPE_DR_IO_NEW_XML_VERSION CPE_DR_XML_DATA_FORMAT_LIST_ENTRY_NAME


/** @} */

/**@name 排序方法
*@ {*/
enum tagDRSortMethod
{
	CPE_DR_SORTMETHOD_NONE_SORT =	0x00,  /**< 不排序*/
	CPE_DR_SORTMETHOD_ASC_SORT	=	0x01,    /**<升序排序*/
	CPE_DR_SORTMETHOD_DSC_SORT =0x02,    /**<降序排序*/
};
/*@ }*/

/*const of type */
#define  CPE_DR_INVALID_INDEX		-1
#define  CPE_DR_INVALID_PTR                -1
#define  CPE_DR_INVALID_OFFSET             -1

#define CPE_DR_INVALID_ID     -1           /**< 无效ID值*/

#define CPE_DR_INVALID_VERSION -1          /**< 无效版本号*/

#endif /* CPE_DR_DEFINE_H */
