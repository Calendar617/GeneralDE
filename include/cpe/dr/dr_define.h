#ifndef CPE_DR_DEFINE_H
#define CPE_DR_DEFINE_H

#define CPE_DR_MAGIC		0x02D6      /**< 元数据二进制文件中的"魔数"*/

#define CPE_DR_MAX_LEVEL    32          /*组合类型最大嵌套深度*/

#define	CPE_DR_MAX_VERSION		0x7fffffff		/**< 元数据库最大版本号*/

#define CPE_DR_NAME_LEN		128		/**< 元数据名字串最大字节数*/
#define CPE_DR_DESC_LEN		1024		/**< 元数据描述串最大字节数*/
#define CPE_DR_MACRO_LEN	128		/**< 元数据宏定义名字串最大字节数*/
#define CPE_DR_CHNAME_LEN	512		/**< 元数据中文名串最大字节数*/
#define CPE_DR_DEFAULT_VALUE_LEN	1024	/**< 元数据缺省值最大字节数*/

#define CPE_DR_XML_TAGSET_VERSION_0             0      /**< 第0版本*/
#define CPE_DR_SUPPORTING_MIN_XMLTAGSET_VERSION	CPE_DR_XML_TAGSET_VERSION_0       /**< 支持的最少版本号*/
#define CPE_DR_XML_TAGSET_VERSION_1             1      /**< 第1版本*/
#define CPE_DR_SUPPORTING_MAX_XMLTAGSET_VERSION	CPE_DR_XML_TAGSET_VERSION_1      /**< 支持的最大版本号*/

#endif
