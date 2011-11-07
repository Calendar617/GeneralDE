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
    CPE_DR_ERROR_NONE = 0,                    /**< û�д���*/
	CPE_DR_ERROR_INVALID_CUTOFF_VERSION,	    /**<���ð汾������ȡֵӦ����Ԫ���ݻ�׼�汾,����С��Ԫ���ݵĵ�ǰ�汾
                                              ���Ԫ�����а汾ָʾ��������ð汾���벻С�ڰ汾ָʾ���������С�汾*/
	CPE_DR_ERR_NET_NO_NETBUFF_SPACE,			/**<���������Ϣ������ʣ��ռ䲻��*/
	CPE_DR_ERROR_INVALID_REFER_VALUE,		/**<��Ԫ�ص�refer����ֵ����ȷ����ֵ����Ϊ�����ұ����count����ֵС*/
	CPE_DR_ERROR_TOO_COMPLIEX_META,		/**<Ԫ��������������������Ƕ�ײ�γ���32��*/
	CPE_DR_ERROR_NET_UNSUPPORTED_TYPE,			/**<��֧�ֵ�Ԫ��������*/
	CPE_DR_ERROR_NET_INVALID_STRING_LEN,		/**<string���͵�Ԫ�����ַ������ȳ�����Ԥ����󳤶�*/
	CPE_DR_ERROR_NO_HOSTBUFF_SPACE,			/**<���ش洢������ʣ��ռ䲻��*/
	CPE_DR_ERROR_NET_INVALID_VERSIONINDICATOR,	/**<Ԫ�����汾ָʾ����ֵ����ȷ*/
    CPE_DR_ERROR_NO_MEMORY,                   /**< ����洢�ռ�ʧ��*/
    CPE_DR_ERROR_XML_PARSE,                /**< XML�ļ��������� */
    CPE_DR_ERROR_NO_XML_ROOT,              /**< XMLԪ������û�и�Ԫ��*/
    CPE_DR_ERROR_INVAILD_XML_ROOT,         /**< ��Ч��Ԫ���ݸ�Ԫ��*/
    CPE_DR_ERROR_NAMESPACE_CONFLICT,       /**< Ԫ�����������ֿռ��ͻ�������ܽ���Ԫ��name����ֵ��ͬ����Ϣ�ӵ�ͬһ������*/
    CPE_DR_ERROR_MACRO_NO_NAME_ATTR,             /**< �궨��Ԫ�ر���ָ��name����*/
    CPE_DR_ERROR_INCLUDE_NO_FILE,          /**< includeδ����file����*/
    CPE_DR_ERROR_NO_VERSION,       /**< û��ָ��version����*/
    CPE_DR_ERROR_ROOT_ID_CONFLICT,         /**< ��Ԫ��ID��ͻ�������ܽ���Ԫ��ID����ֵ��ͬ����Ϣ�ӵ�ͬһ������*/
    CPE_DR_ERROR_INVALID_TAGSET_VERSION,    /**< ��֧�ֵ�Ԫ��������XML��ǩ���汾*/
    CPE_DR_ERROR_INVALID_METALIB_PARAM,    /**< Ԫ���ݿ��������ȷ*/
    CPE_DR_ERROR_INVALID_METALIB_CHECKSUM, /**< Ԫ���ݿ�У��Ͳ���ȷ*/
    CPE_DR_ERROR_TOO_MUCH_MACROS,          /**< ��ӵ�Ԫ�����������еĺ궨������Ԥ�����Ҫ�� */
    CPE_DR_ERROR_MACRO_NO_VALUE,           /**< �궨��Ԫ��û��ֵ����*/
    CPE_DR_ERROR_UNSUPPORTED_TYPE,           /**< ��֧�ֵ��������� */
    CPE_DR_ERROR_METALIB_ROOT_NO_NAME,         /**< Ԫ�����������Ԫ�ر���ָ��name����*/
    CPE_DR_ERROR_NO_SPACE_FOR_MATA,            /**< û���㹻�Ŀռ�洢�Զ�����������*/
    CPE_DR_ERROR_NO_SPACE_FOR_STRBUF,          /**< �ַ����������ռ䲻��*/
    CPE_DR_ERROR_META_NO_NAME,                 /**< union��stuctԪ�ر������name����*/
    CPE_DR_ERROR_META_NAME_CONFLICT,           /**< ͬ���͵�union��stuctԪ�ز�����ͬ��*/
    CPE_DR_ERROR_UNDEFINED_MACRO_NAME,         /**< �ú���û�ж���*/
    CPE_DR_ERROR_META_ID_CONFLICT,          /**< ͬһ��Ԫ���²��ܳ���ID��ͬ����Ԫ��*/
    CPE_DR_ERROR_ENTRY_NO_TYPE,             /**< entryԪ�ر������type�����Ҳ���Ϊ�մ�*/
    CPE_DR_ERROR_ENTRY_INVALID_TYPE_VALUE,     /**< entry��type����ֵ��Ч*/
    CPE_DR_ERROR_ENTRY_INVALID_IO_VALUE,       /**< entry��io����ֵ��Ч*/
    CPE_DR_ERROR_ENTRY_INVALID_UNIQUE_VALUE,   /**< entry��unique���Բ���ȷ����ȷȡֵΪfalse,true*/
    CPE_DR_ERROR_ENTRY_INVALID_NOTNULL_VALUE,  /**< entry��notnull���Բ���ȷ����ȷȡֵΪfalse,true*/
    CPE_DR_ERROR_ENTRY_INVALID_SIZE_VALUE,  /**< entry��size����ֵ����ȷ*/
    CPE_DR_ERROR_ENTRY_IVALID_SORTKEY_VALUE,    /**<entry��sortkey����ֵ����ȷ*/
    CPE_DR_ERROR_ENTRY_INVALID_SELECT_VALUE,    /**< entry��select����ֵ����ȷ*/
    CPE_DR_ERROR_ENTRY_INVALID_MAXID_VALUE,     /**< entry��maxid���Բ���ȷ*/
    CPE_DR_ERROR_ENTRY_INVALID_MINID_VALUE,     /**< entry��minid���Բ���ȷ*/
    CPE_DR_ERROR_ENTRY_INVALID_MAXMINID_VALUE,     /**< entry��minid��maxid����ֵ����ȷ*/
    CPE_DR_ERROR_ENTRY_INVALID_COUNT_VALUE,     /**<entry��count����ֵ����ȷ*/
	CPE_DR_ERROR_ENTRY_INVALID_ID_VALUE,		/**<entry��id����ֵ����ȷ*/
	CPE_DR_ERROR_ENTRY_INVALID_DEFAULT_VALUE,  /**<entry��default����ֵ����ȷ*/
	CPE_DR_ERROR_ENTRY_INVALID_SORTMETHOD_VALUE, /**<entry��sortmethod����ֵ����ȷ*/
	CPE_DR_ERROR_ENTRY_INVALID_DATETIME_VALUE,		/**<entry��datetime����ֵ����ȷ*/
	CPE_DR_ERROR_ENTRY_INVAILD_DATE_VALUE,			/**<entry��date����ֵ����ȷ*/
	CPE_DR_ERROR_ENTRY_INVALID_TIME_VALUE,			/**<entry��time����ֵ����ȷ*/
	CPE_DR_ERROR_ENTRY_INVALID_IP_VALUE,			/**<entry��ip����ֵ����ȷ*/
	CPE_DR_ERROR_ENTRY_INVALID_EXTENDTOTABLE_VALUE,	/**<entry��extendtotable���Բ���ȷ*/
    CPE_DR_ERROR_META_INVALID_SIZE_VALUE,      /**<structԪ�ص�size���Բ���ȷ*/
    CPE_DR_ERROR_META_INVALID_ALIGN_VALUE,     /**<structԪ�ص�align����ֵ����ȷ*/
    CPE_DR_ERROR_META_INVALID_VERSIONINDICATOR_VALUE,      /**<structԪ�ص�versionindicator���Բ���ȷ*/
    CPE_DR_ERROR_META_INVALID_SIZETYPE_VALUE,      /**< Ԫ�ص�sizetype/sizeof����ֵ����ȷ*/
	CPE_DR_ERROR_META_INVALID_SPLITTABLEFACTOR,	/**< structԪ�ص�splittablefactor����ֵ����ȷ*/
	CPE_DR_ERROR_META_INVALID_PRIMARYKEY,			/**< structԪ�ص�primarykey����ֵ����ȷ*/
    CPE_DR_ERROR_META_INVALID_INDEXCOLUMN,         /**< indexԪ�ص�column����ֵ����ȷ*/
	CPE_DR_ERROR_META_INVALID_SPLITTABLEKEY,	/**< structԪ�ص�splittablekey����ֵ����ȷ*/
	CPE_DR_ERROR_META_INVALID_SPLITTABLERULE,	/**< structԪ�ص�splittablerule����ֵ����ȷ*/
	CPE_DR_ERROR_META_INVALID_STRICTINPUT,		/**<structԪ�ص�strictinput����ֵ����ȷ*/
	CPE_DR_ERROR_META_INVALID_DEPENDONSTRUCT,		/**<structԪ�ص�dependonstruct����ֵ����ȷ*/
    CPE_DR_ERROR_INVALID_PATH_VALUE,               /**<Ԫ�ص�path����ȷ��������ȷƥ��meta�е�Ԫ��*/
    CPE_DR_ERROR_INVALID_OFFSET,                   /**<Ԫ�ص�ƫ��ֵ����*/
    CPE_DR_ERROR_NO_SPACE_TO_WRITE,                /**<����Ϣд��������ʱ�ռ䲻��*/
    CPE_DR_ERROR_META_NO_ENTRY,                    /**< �Զ�����������û�а����κ��ӳ�Ա*/
    CPE_DR_ERROR_ENTRY_INVALID_REFER_VALUE,        /**<entryԪ�ص�refer����ֵ����ȷ*/
    CPE_DR_ERROR_ENTRY_INVALID_SIZEINFO_VALUE,     /**<entryԪ�ص�sizeinfo����ֵ����ȷ*/
	CPE_DR_ERROR_UNSPORTED_IOSTREAM,				/**<��֧�ֵ�IO��*/
	CPE_DR_ERROR_FAILED_TO_WRITE_FILE,				/**<д�ļ�ʧ��*/
	CPE_DR_ERROR_FAILED_OPEN_FILE_TO_WRITE,					/**<���ļ�дʧ��*/
	CPE_DR_ERROR_INVALID_METALIB_FILE,				/**<����Ԫ���ݿ�Ķ������ļ���Ч*/
	CPE_DR_ERROR_FAILED_OPEN_FILE_TO_READ,			/**<���ļ���ʧ��*/
    CPE_DR_ERROR_VARIABLE_ARRAY_NO_REFER,          /**<�ɱ��������ָ��refer����*/
    CPE_DR_ERROR_VARIABLE_BEFOR_SIZEINFO,          /**<Ԫ������sizeinfo��Աǰ�ĳ�Ա�Ĵ洢�ռ�����ǹ̶���*/
	CPE_DR_ERROR_FAILED_CONVERT_CHINESE_TO_UNICODE,	/**<�����ַ���ת����unicode�ַ���ʧ��*/
	CPE_DR_ERROR_BREACH_KEY_RESTRICTION,			/**<entryԪ�ص�ֵ�������Լ��*/
	CPE_DR_ERROR_DB_UNSUPPORTED_DBMS,				/**<��֧�ֵ����ݿ����ϵͳDBMS*/
	CPE_DR_ERROR_DB_UNSUPPORTED_COMPOSITE_ARRAY,	/**<��֧��Ϊ�����������������Ա���ɽ������*/
	CPE_DR_ERROR_DB_FAILD_TO_CONNECT_SERVER,       /**<�������ݿ������ʧ��*/
	CPE_DR_ERROR_DB_UNSUPPORTED_OPER,				/**<��֧�ֵ����ݲ���*/
	CPE_DR_ERROR_DB_NO_PRIMARYKEY,					/**<�ü��ð汾�޷�������Ч��������Ϣ*/
	CPE_DR_ERROR_DB_FAILED_TO_QUERY,					/**<ִ�����ݿ�SQL���ʧ��*/
	CPE_DR_ERROR_DB_FAILED_TO_CONNECT,					/**<���ݿ����Ӵ���*/
	CPE_DR_ERROR_DB_FAILED_TO_GET_QUERY_RESULT,		/**<ȡSQL��ѯ���ʧ��*/
	CPE_DR_ERROR_DB_NO_RESULT_SET,			/**<SQL��ѯ�����Ϊ��*/
	CPE_DR_ERROR_DB_NO_RECORD_IN_RESULTSET, /**<�������û�и�������ݼ�¼������˴���*/
	CPE_DR_ERROR_DB_NO_EXPECTED_FIELD,			/**<��ǰ�������в�����ָ����������*/
	CPE_DR_ERROR_DB_UNSUPPORTED_VARIABLE_META,	/**<��֧��Ϊ�洢�ռ䲻�̶��Ľṹ���ɽ������*/
	CPE_DR_ERROR_BUILD_VERSION_CONFLICT,		/**<����Ԫ���ݿ��ļ���tdr���ߵĺ������ݹ����汾��tdr��ĺ������ݹ������汾��һ��*/
	CPE_DR_ERROR_DIFF_METALIB_HASH,			/**<Ԫ���ݿ��ɢ��ֵ��������ɢ��ֵ��һ��*/
	CPE_DR_ERROR_CONFICT_INDEX_NUM,			/**<�ṹ���Ա��ʵ����������Ԥ�ƵĲ�һ��*/
	CPE_DR_ERROR_INVALID_VERSION,					/**<��Ա��vesion����ֵ����ȷ*/
	CPE_DR_ERROR_DB_NOSPLITTABLE_KEY,				/**<�������ڵ����ݿ���Ƿֱ�洢��,�������ݵ�Ԫ��������û��ָ���ֱ�ؼ���*/
	CPE_DR_ERROR_MACROGSROUP_ATTRIBUTE,			/**<����macrosgroup����ʧ��*/
	CPE_DR_ERROR_ENTRY_INVALID_BINDMACROSGROUP,	/**<entry��bindmacrosgroup����ֵ��Ч*/
	CPE_DR_ERROR_VALUE_BEYOND_TYPE_FIELD,				/**<��Աȡֵ�Ѿ����������͵����ڷ�Χ*/
	CPE_DR_ERROR_META_EXTENABLE_ARRAY,					/**<�ڿ���չ�Ľṹ�������Ա�ж����˲�������ֵ�����*/
	CPE_DR_ERROR_FAILED_TO_CALL_ENTER_META,		/**<�����ָ��������������ݳ�Աʱ�����ûص�ʧ��*/
	CPE_DR_ERROR_FAILED_TO_CALL_VIST_ENTRY,		/**<�����ּ��������ͳ�Աʱ,���ûص�����ʧ��*/
	CPE_DR_ERROR_INVALID_AUTOINCREMENT,			/**<��Ա��autoincrement������Ч��Ŀǰֻ�з����������������͵ĳ�Ա���ܶ��������*/
	CPE_DR_ERROR_INVALID_CUSTOMATTR,				/**<��Ա��custom����ֵ��Ч,��ȷ������ֵ�ĳ��Ȳ�������󳤶�����*/
	CPE_DR_ERROR_INVALID_UNIQUEENTRYNAMEVALUE,				/**<�ṹ���uniqueentryname����ֵ��Ч,�����Ե�����ֵֻ��Ϊtrue/false*/
	CPE_DR_ERROR_NAME_CONFLICT_WHEN_EXTEND,				/**<���ṹ���Աչ���洢ʱ���������*/
	CPE_DR_ERROR_INVALID_PARAM,					/**<������Ч������ÿ�������Ƿ�����ӿڵ�ǰ������Լ��*/
	CPE_DR_ERROR_NEED_ADD_PREFIX_META_UNSPPORT_ENTRY_INDEX,	/**<��չ���洢ʱ��Ҫ���ǰ׺�Ľṹ�岻֧�ֽ������г�Ա����ƫ��������*/
	CPE_DR_ERROR_SET_DB_OPTIONS,     /**<����db����ʧ��*/
	CPE_DR_ERROR_INVALID_VARIABLE_ARRAY_ENTRY,		/**<�ṹ���Ա�У�ֻ�������һ����Ա�ǿɱ��������͵ĳ�Ա*/
	CPE_DR_ERROR_INVALID_VARIABLE_STRING_ENTRY,	/**<�ṹ���Ա�У�û�ж���size���Ե�string���ͳ�Ա(��洢�ռ��ǿɱ��)ֻ����Ϊ���һ����Ա���Ҳ���Ϊ���飬*/
    CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT,          /**<���ִ�̫������������󳤶�����*/
    CPE_DR_ERROR_PLATFORM_CONFLICT,               /**<����Ԫ���ݿ��ļ���ϵͳƽ̨������tdr���ϵͳƽ̨��һ�£���ע��32λƽ̨��64λƽ̨������*/
    CPE_DR_ERROR_UNSOLVED_INCLUDE,                 /**<xml�ļ��д����޷�������include��ϵ*/
    CPE_DR_ERROR_INVALID_INDICATOR_VALUE,      /**<�������ݰ��еİ汾ָʾ��ֵ����ȷ����ȡֵӦ����Ԫ���ݻ�׼�汾,����С��Ԫ���ݵĵ�ǰ�汾
                                              ���Ԫ�����а汾ָʾ������汾ָʾ����ֵ���벻С�ڰ汾ָʾ���������С�汾*/
    CPE_DR_ERROR_COUNT                     /**< ������������־*/
} CPEDRERRORNO;


typedef enum
{
    CPE_DR_SUCWARN_NONE = 0,                    /**< û�д���*/
    CPE_DR_SUCWARN_MACRO_NAME_CONFLICT,        /**<����ͬ���ĺ궨��*/
    CPE_DR_SUCWARN_CNAME_BE_TRANCATED,         /**<cname����ֵ����Ԥ����󳤶ȣ����ض�*/
    CPE_DR_SUCWARN_DESC_VALUE_BE_TRANCATED,    /**<desc����ֵ����Ԥ����󳤶ȣ����ض�*/
    CPE_DR_SUCWARN_NO_ID,                      /**<Ԫ��û��ָ��id����*/
    CPE_DR_SUCWARN_NO_NAME,                    /**<Ԫ��û��ָ��name����*/
    CPE_DR_SUCWARN_NO_VERSION,                 /**<Ԫ��û��ָ��version����*/
	CPE_DR_SUCWARN_TRUNCATE_DATE,				/**<��������ʱ���������ݽض�*/
} CPEDRSUCWARNINGNO;


CPE_DR_API  char const* dr_error_string(IN int iErrorCode);

#undef _CPE_CUR_MODID

#ifdef __cplusplus
}
#endif

#endif
