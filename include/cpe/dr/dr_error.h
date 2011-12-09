#ifndef CPE_DR_ERROR_H
#define CPE_DR_ERROR_H
#include "cpe/utils/error_macros.h"
#include "cpe/dr/dr_define.h"
#include "cpe/dr/dr_external.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CPE_MODID_DR	0x01

typedef enum {
    CPE_DR_ERROR_NONE = 0,                    /**< 没有错误*/
	CPE_DR_ERROR_INVALID_CUTOFF_VERSION,	    /**<剪裁版本错误，其取值应大于元数据基准版本,并且小于元数据的当前版本
                                              如果元数据有版本指示器，则剪裁版本必须不小于版本指示器允许的最小版本*/
	CPE_DR_ERR_NET_NO_NETBUFF_SPACE,			/**<网络编码信息缓冲区剩余空间不够*/
	CPE_DR_ERROR_INVALID_REFER_VALUE,		/**<此元素的refer属性值不正确，其值不能为负数且必须比count属性值小*/
	CPE_DR_ERROR_TOO_COMPLIEX_META,		/**<元数据描述复合数据类型嵌套层次超过32层*/
	CPE_DR_ERROR_NET_UNSUPPORTED_TYPE,			/**<不支持的元数据类型*/
	CPE_DR_ERROR_NET_INVALID_STRING_LEN,		/**<string类型的元素其字符串长度超过了预定最大长度*/
	CPE_DR_ERROR_NO_HOSTBUFF_SPACE,			/**<本地存储缓冲区剩余空间不够*/
	CPE_DR_ERROR_NET_INVALID_VERSIONINDICATOR,	/**<元素数版本指示器的值不正确*/
    CPE_DR_ERROR_NO_MEMORY,                   /**< 分配存储空间失败*/
    CPE_DR_ERROR_XML_PARSE,                /**< XML文件解析错误 */
    CPE_DR_ERROR_NO_XML_ROOT,              /**< XML元素树中没有根元素*/
    CPE_DR_ERROR_INVAILD_XML_ROOT,         /**< 无效的元数据根元素*/
    CPE_DR_ERROR_NAMESPACE_CONFLICT,       /**< 元数据描述名字空间冲突，即不能将根元素name属性值不同的信息加到同一个库中*/
    CPE_DR_ERROR_MACRO_NO_NAME_ATTR,             /**< 宏定义元素必须指定name属性*/
    CPE_DR_ERROR_INCLUDE_NO_FILE,          /**< include未定义file属性*/
    CPE_DR_ERROR_NO_VERSION,       /**< 没有指定version属性*/
    CPE_DR_ERROR_ROOT_ID_CONFLICT,         /**< 根元素ID冲突，即不能将根元素ID属性值不同的信息加到同一个库中*/
    CPE_DR_ERROR_INVALID_TAGSET_VERSION,    /**< 不支持的元数据描述XML标签集版本*/
    CPE_DR_ERROR_INVALID_METALIB_PARAM,    /**< 元数据库参数不正确*/
    CPE_DR_ERROR_INVALID_METALIB_CHECKSUM, /**< 元数据库校验和不正确*/
    CPE_DR_ERROR_TOO_MUCH_MACROS,          /**< 需加到元数据描述库中的宏定义数比预定义的要多 */
    CPE_DR_ERROR_MACRO_NO_VALUE,           /**< 宏定义元素没有值属性*/
    CPE_DR_ERROR_UNSUPPORTED_TYPE,           /**< 不支持的数据类型 */
    CPE_DR_ERROR_METALIB_ROOT_NO_NAME,         /**< 元数据描述库根元素必须指定name属性*/
    CPE_DR_ERROR_NO_SPACE_FOR_MATA,            /**< 没有足够的空间存储自定义数据类型*/
    CPE_DR_ERROR_NO_SPACE_FOR_STRBUF,          /**< 字符串缓冲区空间不够*/
    CPE_DR_ERROR_META_NO_NAME,                 /**< union和stuct元素必须包含name属性*/
    CPE_DR_ERROR_META_NAME_CONFLICT,           /**< 同类型的union和stuct元素不容许同名*/
    CPE_DR_ERROR_UNDEFINED_MACRO_NAME,         /**< 该宏名没有定义*/
    CPE_DR_ERROR_META_ID_CONFLICT,          /**< 同一父元素下不能出现ID相同的子元素*/
    CPE_DR_ERROR_ENTRY_NO_TYPE,             /**< entry元素必须包含type属性且不能为空串*/
    CPE_DR_ERROR_ENTRY_INVALID_TYPE_VALUE,     /**< entry的type属性值无效*/
    CPE_DR_ERROR_ENTRY_INVALID_IO_VALUE,       /**< entry的io属性值无效*/
    CPE_DR_ERROR_ENTRY_INVALID_UNIQUE_VALUE,   /**< entry的unique属性不正确，正确取值为false,true*/
    CPE_DR_ERROR_ENTRY_INVALID_NOTNULL_VALUE,  /**< entry的notnull属性不正确，正确取值为false,true*/
    CPE_DR_ERROR_ENTRY_INVALID_SIZE_VALUE,  /**< entry的size属性值不正确*/
    CPE_DR_ERROR_ENTRY_IVALID_SORTKEY_VALUE,    /**<entry的sortkey属性值不正确*/
    CPE_DR_ERROR_ENTRY_INVALID_SELECT_VALUE,    /**< entry的select属性值不正确*/
    CPE_DR_ERROR_ENTRY_INVALID_MAXID_VALUE,     /**< entry的maxid属性不正确*/
    CPE_DR_ERROR_ENTRY_INVALID_MINID_VALUE,     /**< entry的minid属性不正确*/
    CPE_DR_ERROR_ENTRY_INVALID_MAXMINID_VALUE,     /**< entry的minid和maxid属性值不正确*/
    CPE_DR_ERROR_ENTRY_INVALID_COUNT_VALUE,     /**<entry的count属性值不正确*/
	CPE_DR_ERROR_ENTRY_INVALID_ID_VALUE,		/**<entry的id属性值不正确*/
	CPE_DR_ERROR_ENTRY_INVALID_DEFAULT_VALUE,  /**<entry的default属性值不正确*/
	CPE_DR_ERROR_ENTRY_INVALID_SORTMETHOD_VALUE, /**<entry的sortmethod属性值不正确*/
	CPE_DR_ERROR_ENTRY_INVALID_DATETIME_VALUE,		/**<entry的datetime属性值不正确*/
	CPE_DR_ERROR_ENTRY_INVAILD_DATE_VALUE,			/**<entry的date属性值不正确*/
	CPE_DR_ERROR_ENTRY_INVALID_TIME_VALUE,			/**<entry的time属性值不正确*/
	CPE_DR_ERROR_ENTRY_INVALID_IP_VALUE,			/**<entry的ip属性值不正确*/
	CPE_DR_ERROR_ENTRY_INVALID_EXTENDTOTABLE_VALUE,	/**<entry的extendtotable属性不正确*/
    CPE_DR_ERROR_META_INVALID_SIZE_VALUE,      /**<struct元素的size属性不正确*/
    CPE_DR_ERROR_META_INVALID_ALIGN_VALUE,     /**<struct元素的align属性值不正确*/
    CPE_DR_ERROR_META_INVALID_VERSIONINDICATOR_VALUE,      /**<struct元素的versionindicator属性不正确*/
    CPE_DR_ERROR_META_INVALID_SIZETYPE_VALUE,      /**< 元素的sizetype/sizeof属性值不正确*/
	CPE_DR_ERROR_META_INVALID_SPLITTABLEFACTOR,	/**< struct元素的splittablefactor属性值不正确*/
	CPE_DR_ERROR_META_INVALID_PRIMARYKEY,			/**< struct元素的primarykey属性值不正确*/
    CPE_DR_ERROR_META_INVALID_INDEXCOLUMN,         /**< index元素的column属性值不正确*/
	CPE_DR_ERROR_META_INVALID_SPLITTABLEKEY,	/**< struct元素的splittablekey属性值不正确*/
	CPE_DR_ERROR_META_INVALID_SPLITTABLERULE,	/**< struct元素的splittablerule属性值不正确*/
	CPE_DR_ERROR_META_INVALID_STRICTINPUT,		/**<struct元素的strictinput属性值不正确*/
	CPE_DR_ERROR_META_INVALID_DEPENDONSTRUCT,		/**<struct元素的dependonstruct属性值不正确*/
    CPE_DR_ERROR_INVALID_PATH_VALUE,               /**<元素的path不正确，不能正确匹配meta中的元素*/
    CPE_DR_ERROR_INVALID_OFFSET,                   /**<元素的偏移值不对*/
    CPE_DR_ERROR_NO_SPACE_TO_WRITE,                /**<将信息写到缓冲区时空间不够*/
    CPE_DR_ERROR_META_NO_ENTRY,                    /**< 自定义数据类型没有包含任何子成员*/
    CPE_DR_ERROR_ENTRY_INVALID_REFER_VALUE,        /**<entry元素的refer属性值不正确*/
    CPE_DR_ERROR_ENTRY_INVALID_SIZEINFO_VALUE,     /**<entry元素的sizeinfo属性值不正确*/
	CPE_DR_ERROR_UNSPORTED_IOSTREAM,				/**<不支持的IO流*/
	CPE_DR_ERROR_FAILED_TO_WRITE_FILE,				/**<写文件失败*/
	CPE_DR_ERROR_FAILED_OPEN_FILE_TO_WRITE,					/**<打开文件写失败*/
	CPE_DR_ERROR_INVALID_METALIB_FILE,				/**<保存元数据库的二进制文件无效*/
	CPE_DR_ERROR_FAILED_OPEN_FILE_TO_READ,			/**<打开文件读失败*/
    CPE_DR_ERROR_VARIABLE_ARRAY_NO_REFER,          /**<可变数组必须指定refer属性*/
    CPE_DR_ERROR_VARIABLE_BEFOR_SIZEINFO,          /**<元数据中sizeinfo成员前的成员的存储空间必须是固定的*/
	CPE_DR_ERROR_FAILED_CONVERT_CHINESE_TO_UNICODE,	/**<中文字符串转换成unicode字符串失败*/
	CPE_DR_ERROR_BREACH_KEY_RESTRICTION,			/**<entry元素的值不满足键约束*/
	CPE_DR_ERROR_DB_UNSUPPORTED_DBMS,				/**<不支持的数据库管理系统DBMS*/
	CPE_DR_ERROR_DB_UNSUPPORTED_COMPOSITE_ARRAY,	/**<不支持为复合数据类型数组成员生成建表语句*/
	CPE_DR_ERROR_DB_FAILD_TO_CONNECT_SERVER,       /**<连接数据库服务器失败*/
	CPE_DR_ERROR_DB_UNSUPPORTED_OPER,				/**<不支持的数据操作*/
	CPE_DR_ERROR_DB_NO_PRIMARYKEY,					/**<该剪裁版本无法生成有效的主键信息*/
	CPE_DR_ERROR_DB_FAILED_TO_QUERY,					/**<执行数据库SQL语句失败*/
	CPE_DR_ERROR_DB_FAILED_TO_CONNECT,					/**<数据库连接错误*/
	CPE_DR_ERROR_DB_FAILED_TO_GET_QUERY_RESULT,		/**<取SQL查询结果失败*/
	CPE_DR_ERROR_DB_NO_RESULT_SET,			/**<SQL查询结果集为空*/
	CPE_DR_ERROR_DB_NO_RECORD_IN_RESULTSET, /**<结果集中没有更多的数据记录或出现了错误*/
	CPE_DR_ERROR_DB_NO_EXPECTED_FIELD,			/**<当前数据行中不存在指定的数据域*/
	CPE_DR_ERROR_DB_UNSUPPORTED_VARIABLE_META,	/**<不支持为存储空间不固定的结构生成建表语句*/
	CPE_DR_ERROR_BUILD_VERSION_CONFLICT,		/**<生成元数据库文件的tdr工具的核心数据构建版本和tdr库的核心数据构构建版本不一致*/
	CPE_DR_ERROR_DIFF_METALIB_HASH,			/**<元数据库的散列值和期望的散列值不一致*/
	CPE_DR_ERROR_CONFICT_INDEX_NUM,			/**<结构体成员的实际索引数与预计的不一致*/
	CPE_DR_ERROR_INVALID_VERSION,					/**<成员的vesion属性值不正确*/
	CPE_DR_ERROR_DB_NOSPLITTABLE_KEY,				/**<数据所在的数据库表是分表存储的,但此数据的元数据描述没有指定分表关键字*/
	CPE_DR_ERROR_MACROGSROUP_ATTRIBUTE,			/**<处理macrosgroup属性失败*/
	CPE_DR_ERROR_ENTRY_INVALID_BINDMACROSGROUP,	/**<entry的bindmacrosgroup属性值无效*/
	CPE_DR_ERROR_VALUE_BEYOND_TYPE_FIELD,				/**<成员取值已经超出此类型的置于范围*/
	CPE_DR_ERROR_META_EXTENABLE_ARRAY,					/**<在可扩展的结构体数组成员中定义了不容许出现的属性*/
	CPE_DR_ERROR_FAILED_TO_CALL_ENTER_META,		/**<当发现复合数据类型数据成员时，调用回调失败*/
	CPE_DR_ERROR_FAILED_TO_CALL_VIST_ENTRY,		/**<当发现简单数据类型成员时,调用回调函数失败*/
	CPE_DR_ERROR_INVALID_AUTOINCREMENT,			/**<成员的autoincrement属性无效，目前只有非数组整数数据类型的成员才能定义此属性*/
	CPE_DR_ERROR_INVALID_CUSTOMATTR,				/**<成员的custom属性值无效,请确定属性值的长度不超过最大长度限制*/
	CPE_DR_ERROR_INVALID_UNIQUEENTRYNAMEVALUE,				/**<结构体的uniqueentryname属性值无效,此属性的属性值只能为true/false*/
	CPE_DR_ERROR_NAME_CONFLICT_WHEN_EXTEND,				/**<将结构体成员展开存储时会出现重名*/
	CPE_DR_ERROR_INVALID_PARAM,					/**<参数无效，请检查每个参数是否满足接口的前置条件约束*/
	CPE_DR_ERROR_NEED_ADD_PREFIX_META_UNSPPORT_ENTRY_INDEX,	/**<在展开存储时需要添加前缀的结构体不支持建立所有成员名字偏移索引表*/
	CPE_DR_ERROR_SET_DB_OPTIONS,     /**<设置db属性失败*/
	CPE_DR_ERROR_INVALID_VARIABLE_ARRAY_ENTRY,		/**<结构体成员中，只容许最后一个成员是可变数组类型的成员*/
	CPE_DR_ERROR_INVALID_VARIABLE_STRING_ENTRY,	/**<结构题成员中，没有定义size属性的string类型成员(其存储空间是可变的)只能作为最后一个成员，且不能为数组，*/
    CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT,          /**<名字串太长，超过了最大长度限制*/
    CPE_DR_ERROR_PLATFORM_CONFLICT,               /**<生成元数据库文件的系统平台与生成tdr库的系统平台不一致，请注意32位平台与64位平台的区别*/
    CPE_DR_ERROR_UNSOLVED_INCLUDE,                 /**<xml文件中存在无法解析的include关系*/
    CPE_DR_ERROR_INVALID_INDICATOR_VALUE,      /**<网络数据包中的版本指示器值不正确，其取值应大于元数据基准版本,并且小于元数据的当前版本
                                              如果元数据有版本指示器，则版本指示器的值必须不小于版本指示器允许的最小版本*/
    CPE_DR_ERROR_COUNT                     /**< 错误代码结束标志*/
} CPEDRERRORNO;


typedef enum
{
    CPE_DR_SUCWARN_NONE = 0,                    /**< 没有错误*/
    CPE_DR_SUCWARN_MACRO_NAME_CONFLICT,        /**<出现同名的宏定义*/
    CPE_DR_SUCWARN_CNAME_BE_TRANCATED,         /**<cname属性值超过预定最大长度，被截断*/
    CPE_DR_SUCWARN_DESC_VALUE_BE_TRANCATED,    /**<desc属性值超过预定最大长度，被截断*/
    CPE_DR_SUCWARN_NO_ID,                      /**<元素没有指定id属性*/
    CPE_DR_SUCWARN_NO_NAME,                    /**<元素没有指定name属性*/
    CPE_DR_SUCWARN_NO_VERSION,                 /**<元素没有指定version属性*/
	CPE_DR_SUCWARN_TRUNCATE_DATE,				/**<保存数据时进行了数据截断*/
} CPEDRSUCWARNINGNO;


CPE_DR_API  char const* dr_error_string(int iErrorCode);

#undef _CPE_CUR_MODID

#ifdef __cplusplus
}
#endif

#endif
