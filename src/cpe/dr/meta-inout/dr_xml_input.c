#include <string.h>
#include "libxml/tree.h"
#include "libxml/xmlstring.h"
#include "cpe/dr/dr_metalib_build.h"
#include "cpe/dr/dr_metalib_xml.h"
#include "cpe/dr/dr_error.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"
#include "dr_inbuild.h"
#include "dr_inbuild_error.h"
#include "dr_XMLtags.h"

enum DRXmlParseState {
    PS_Init
    , PS_InMetaLib
    , PS_InMeta
    , PS_Error
};

#define DR_XML_PARSE_ERROR_MSG_LEN 256

#define DR_COPY_STR(buf, str, len) \
    memcpy(buf, str, len);         \
    buf[len] = 0;

#define DR_DO_READ_INT(__d, __e)                                        \
    if (len >= INTEGER_BUF_LEN) {                                       \
        DR_NOTIFY_ERROR(ctx->m_em, (__e));                              \
        return;                                                         \
    }                                                                   \
    DR_COPY_STR(buf, (char const *)valueBegin, len);                    \
    sscanf(buf, "%d", &(__d))

#define DR_DO_DUP_STR(buf)                                              \
    buf =  mem_buffer_strndup(                                          \
        &ctx->m_metaLib->m_tmp_buf, (char const *)valueBegin, len);

struct DRXmlParseCtx {
    struct DRInBuildMetaLib * m_metaLib;
    struct DRInBuildMeta * m_curentMeta;
    enum DRXmlParseState m_state;
    error_monitor_t m_em;
};

static void dr_build_xml_process_metalib(
    struct DRXmlParseCtx * ctx,
    int nb_attributes,
    const xmlChar** attributes)
{
    const int INTEGER_BUF_LEN = 10;
    char buf[INTEGER_BUF_LEN];
    int indexAttribute = 0;
    int index = 0;
    int haveVersion = 0;

    if (ctx->m_state != PS_Init) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_XML_PARSE, "error metalib tag!");
        ctx->m_state = PS_Error;
        return;
    }

    for(index = 0, indexAttribute = 0;
        indexAttribute < nb_attributes;
        ++indexAttribute, index += 5)
    {
        const xmlChar *localname = attributes[index];
        /*const xmlChar *prefix = attributes[index+1];*/
        /*const xmlChar *nsURI = attributes[index+2];*/
        const xmlChar *valueBegin = attributes[index+3];
        const xmlChar *valueEnd = attributes[index+4];

        int len = valueEnd - valueBegin;

        if (strcmp((char const *)localname, CPE_DR_TAG_NAME) == 0) {
            if (len >= CPE_DR_NAME_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT);
                return;
            }

            DR_COPY_STR(ctx->m_metaLib->m_data.szName, (char const *)valueBegin, len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_TAGSET_VERSION) == 0) {
            if (len >= INTEGER_BUF_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_INVALID_TAGSET_VERSION);
                ctx->m_state = PS_Error;
                return;
            }

            DR_COPY_STR(buf, (char const *)valueBegin, len);
            sscanf(buf, "%d", &ctx->m_metaLib->m_data.iTagSetVersion);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_VERSION) == 0) {
            if (len > INTEGER_BUF_LEN - 1) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_INVALID_VERSION);
                return;
            }

            haveVersion = 1;
            DR_COPY_STR(buf, (char const *)valueBegin, len);
            sscanf(buf, "%d", &ctx->m_metaLib->m_data.iVersion);
        }
        else {
        }
    }

    if (ctx->m_metaLib->m_data.iTagSetVersion != 1) {
        CPE_ERROR_EX(
            ctx->m_em,
            CPE_DR_ERROR_INVALID_TAGSET_VERSION,
            "unknown tagsetversion version, only support 1!");

        ctx->m_state = PS_Error;
    }

    if (!haveVersion || ctx->m_metaLib->m_data.iVersion < 0) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NO_VERSION);
    }

    if (strlen(ctx->m_metaLib->m_data.szName) == 0) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_METALIB_ROOT_NO_NAME);
    }

    ctx->m_state = PS_InMetaLib;
}

static void dr_build_xml_process_macro(
    struct DRXmlParseCtx * ctx,
    int nb_attributes,
    const xmlChar** attributes)
{
    const int INTEGER_BUF_LEN = 10;
    char buf[INTEGER_BUF_LEN];
    int indexAttribute = 0;
    int index = 0;
    int haveValue = 0;

    if (ctx->m_state != PS_InMetaLib) {
        return;
    }

    struct DRInBuildMacro * newMacro = dr_inbuild_metalib_add_macro(ctx->m_metaLib);
    if (newMacro == NULL) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NO_MEMORY);
        return;
    }

    int haveError = 0;

    for(index = 0, indexAttribute = 0;
        indexAttribute < nb_attributes && !haveError;
        ++indexAttribute, index += 5)
    {
        const xmlChar *localname = attributes[index];
        /*const xmlChar *prefix = attributes[index+1];*/
        /*const xmlChar *nsURI = attributes[index+2]*/;
        const xmlChar *valueBegin = attributes[index+3];
        const xmlChar *valueEnd = attributes[index+4];

        int len = valueEnd - valueBegin;

        if (strcmp((char const *)localname, CPE_DR_TAG_NAME) == 0) {
            if (len > CPE_DR_MACRO_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT);
                return;
            }

            DR_COPY_STR(newMacro->m_name, (char const *)valueBegin, len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_DESCIPTION) == 0) {
            if (len > CPE_DR_DESC_LEN) {
                len = CPE_DR_DESC_LEN;
            }

            DR_COPY_STR(newMacro->m_desc, (char const *)valueBegin, len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_MACRO_VALUE) == 0) {
            if (len >= INTEGER_BUF_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT);
                return;
            }
            haveValue = 1;

            DR_COPY_STR(
                buf,
                (char const *)valueBegin,
                len >= INTEGER_BUF_LEN ? INTEGER_BUF_LEN - 1 : len);
            sscanf(buf, "%d", &newMacro->m_data.m_value);
        }
        else {
        }
    }

    if (strlen(newMacro->m_name) == 0) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_MACRO_NO_NAME_ATTR);
        haveError = 1;
    }

    if (!haveValue) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_MACRO_NO_VALUE);
        haveError = 1;
    }

    if (haveError) {
        dr_inbuild_metalib_remove_macro(ctx->m_metaLib, newMacro);
    }
}

static void dr_build_xml_process_meta(
    struct DRXmlParseCtx * ctx,
    int nb_attributes,
    const xmlChar** attributes,
    int32_t metaType)
{
    const int INTEGER_BUF_LEN = 10;
    char buf[INTEGER_BUF_LEN];
    int indexAttribute = 0;
    int index = 0;
    int version = -1;
    struct DRInBuildMeta * newMeta = NULL;
    int haveError = 0;

    if (ctx->m_state != PS_InMetaLib) {
        return;
    }

    newMeta = dr_inbuild_metalib_add_meta(ctx->m_metaLib);
    if (newMeta == NULL) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NO_MEMORY);
        return;
    }

    newMeta->m_data.m_type = metaType;
    newMeta->m_data.m_align = 1;

    for(index = 0, indexAttribute = 0;
        indexAttribute < nb_attributes && !haveError;
        ++indexAttribute, index += 5)
    {
        const xmlChar *localname = attributes[index];
        /*const xmlChar *prefix = attributes[index+1];*/
        /*const xmlChar *nsURI = attributes[index+2];*/
        const xmlChar *valueBegin = attributes[index+3];
        const xmlChar *valueEnd = attributes[index+4];

        int len = valueEnd - valueBegin;

        if (strcmp((char const *)localname, CPE_DR_TAG_NAME) == 0) {
            if (len > CPE_DR_MACRO_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT);
                return;
            }

            DR_COPY_STR(newMeta->m_name, (char const *)valueBegin, len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_DESCIPTION) == 0) {
            if (len > CPE_DR_DESC_LEN) {
                len = CPE_DR_DESC_LEN;
            }

            DR_COPY_STR(newMeta->m_desc, (char const *)valueBegin, len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_ID) == 0) {
            if (len >= INTEGER_BUF_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_ENTRY_INVALID_ID_VALUE);
                return;
            }

            DR_COPY_STR(buf, (char const *)valueBegin, len);
            sscanf(buf, "%d", &newMeta->m_data.m_id);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_VERSION) == 0) {
            if (len >= INTEGER_BUF_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_INVALID_TAGSET_VERSION);
                return;
            }

            DR_COPY_STR(buf, (char const *)valueBegin, len);
            sscanf(buf, "%d", &version);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_ALIGN) == 0) {
            if (len >= INTEGER_BUF_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_META_INVALID_ALIGN_VALUE);
                return;
            }

            DR_COPY_STR(buf, (char const *)valueBegin, len);
            sscanf(buf, "%d", &newMeta->m_data.m_align);
        }
        else {
        }
    }


    if (version < 0) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NO_VERSION);
        haveError = 1;
    }
    else if (version > ctx->m_metaLib->m_data.iVersion) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_INVALID_VERSION);
        haveError = 1;
    }
    else {
        newMeta->m_data.m_based_version = version;
        newMeta->m_data.m_current_version = version;
    }

    if (haveError) {
        dr_inbuild_metalib_remove_meta(ctx->m_metaLib, newMeta);
        return;
    }

    ctx->m_state = PS_InMeta;
    ctx->m_curentMeta = newMeta;
}

static void dr_build_xml_process_entry(
    struct DRXmlParseCtx * ctx,
    int nb_attributes,
    const xmlChar** attributes)
{
    const int INTEGER_BUF_LEN = 10;
    char buf[INTEGER_BUF_LEN];
    int indexAttribute = 0;
    int index = 0;
    int version = -1;
    int haveError = 0;
    int haveMin = 0;
    int haveMax = 0;

    if (ctx->m_state != PS_InMeta || ctx->m_curentMeta == NULL) {
        return;
    }

    struct DRInBuildMetaEntry * newEntry = dr_inbuild_meta_add_entry(ctx->m_curentMeta);
    if (newEntry == NULL) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NO_MEMORY);
        return;
    }

    for(index = 0, indexAttribute = 0;
        indexAttribute < nb_attributes && !haveError;
        ++indexAttribute, index += 5)
    {
        const xmlChar *localname = attributes[index];
        /*const xmlChar *prefix = attributes[index+1];*/
        /*const xmlChar *nsURI = attributes[index+2];*/
        const xmlChar *valueBegin = attributes[index+3];
        const xmlChar *valueEnd = attributes[index+4];

        int len = valueEnd - valueBegin;

        if (strcmp((char const *)localname, CPE_DR_TAG_NAME) == 0) {
            if (len >= CPE_DR_NAME_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT);
                return;
            }

            DR_COPY_STR(newEntry->m_name, (char const *)valueBegin, len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_CNNAME) == 0) {
            if (len >= CPE_DR_CHNAME_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT);
                len = CPE_DR_CHNAME_LEN - 1;
            }

            DR_COPY_STR(newEntry->m_cname, (char const *)valueBegin, len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_DESCIPTION) == 0) {
            if (len >= CPE_DR_DESC_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT);
                len = CPE_DR_CHNAME_LEN - 1;
            }

            DR_COPY_STR(newEntry->m_desc, (char const *)valueBegin, len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_TYPE) == 0) {
            if (len >= CPE_DR_NAME_LEN) {
                DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_NAME_LEN_BEYOND_UPLIMIT);
                return;
            }

            DR_DO_DUP_STR(newEntry->m_ref_type_name);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_VERSION) == 0) {
            DR_DO_READ_INT(version, CPE_DR_ERROR_INVALID_TAGSET_VERSION);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_ID) == 0) {
            DR_DO_READ_INT(newEntry->m_data.m_select_range_min, CPE_DR_ERROR_ENTRY_INVALID_ID_VALUE);
            newEntry->m_data.m_select_range_max = newEntry->m_data.m_select_range_min;
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_MAXID) == 0) {
            haveMax = 1;
            DR_DO_READ_INT(newEntry->m_data.m_select_range_max, CPE_DR_ERROR_ENTRY_INVALID_ID_VALUE);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_MINID) == 0) {
            haveMin = 1;
            DR_DO_READ_INT(newEntry->m_data.m_select_range_min, CPE_DR_ERROR_ENTRY_INVALID_ID_VALUE);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_SELECT) == 0) {
            DR_DO_DUP_STR(newEntry->m_selector_path);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_SIZE) == 0) {
            DR_DO_READ_INT(newEntry->m_data.m_unitsize, CPE_DR_ERROR_ENTRY_INVALID_SIZE_VALUE);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_DEFAULT_VALUE) == 0) {
            DR_DO_DUP_STR(newEntry->m_dft_value);
        }
        else {
        }
    }

    if (haveMin && haveMax && newEntry->m_data.m_select_range_min > newEntry->m_data.m_select_range_max) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_INVALID_ID_VALUE, "id min(%d) bigger than max(%d)!",
                     newEntry->m_data.m_select_range_min, newEntry->m_data.m_select_range_max);
    }

    if (haveMin && !haveMax) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_INVALID_ID_VALUE, "id maxid not setted!");
    }

    if (haveMax && !haveMin) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_INVALID_ID_VALUE, "id minid not setted!");
    }

    if (version < 0) {
        version = ctx->m_curentMeta->m_data.m_based_version;
    }
    else if (version > ctx->m_metaLib->m_data.iVersion) {
        DR_NOTIFY_ERROR(ctx->m_em, CPE_DR_ERROR_INVALID_VERSION);
        haveError = 1;
    }

    newEntry->m_data.m_version = version;

    if (strlen(newEntry->m_name) == 0) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_META_NO_NAME, "entry no name");
        haveError = 1;
    }

    if (newEntry->m_ref_type_name == NULL) {
        CPE_ERROR_EX(ctx->m_em, CPE_DR_ERROR_ENTRY_NO_TYPE, "%s's type is null!", newEntry->m_name);
        haveError = 1;
    }

    if (haveError) {
        dr_inbuild_meta_remove_entry(ctx->m_curentMeta, newEntry);
        return;
    }
}

static void dr_build_xml_startElement(
        void* inputCtx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI,
        int nb_namespaces,
        const xmlChar** namespaces,
        int nb_attributes,
        int nb_defaulted,
        const xmlChar** attributes)
{
    struct DRXmlParseCtx * ctx = (struct DRXmlParseCtx *)(inputCtx);

    if (strcmp((const char *)localname, CPE_DR_TAG_ENTRY) == 0) {
        dr_build_xml_process_entry(ctx, nb_attributes, attributes);
    }
    else if (strcmp((const char *)localname, CPE_DR_TAG_STRUCT) == 0) {
        dr_build_xml_process_meta(ctx, nb_attributes, attributes, CPE_DR_TYPE_STRUCT);
    }
    else if (strcmp((const char *)localname, CPE_DR_TAG_UNION) == 0) {
        dr_build_xml_process_meta(ctx, nb_attributes, attributes, CPE_DR_TYPE_UNION);
    }
    else if (strcmp((const char *)localname, CPE_DR_TAG_MACRO) == 0) {
        dr_build_xml_process_macro(ctx, nb_attributes, attributes);
    }
    else if (strcmp((const char *)localname, CPE_DR_TAG_METALIB) == 0) {
        dr_build_xml_process_metalib(ctx, nb_attributes, attributes);
    }
    else {
        
    }
}

static void dr_build_xml_endElement(
        void* inputCtx,
        const xmlChar* localname,
        const xmlChar* prefix,
        const xmlChar* URI)
{
    struct DRXmlParseCtx * ctx = (struct DRXmlParseCtx *)(inputCtx);

    if (strcmp((const char *)localname, CPE_DR_TAG_STRUCT) == 0) {
        ctx->m_curentMeta = NULL;
        if (ctx->m_state == PS_InMeta) {
            ctx->m_state = PS_InMetaLib;
        }
    }
    else if (strcmp((const char *)localname, CPE_DR_TAG_UNION) == 0) {
        ctx->m_curentMeta = NULL;
        if (ctx->m_state == PS_InMeta) {
            ctx->m_state = PS_InMetaLib;
        }
    }
    else {
    }
}

static void dr_build_xml_warningSAX(void * inputCtx, const char *msg, ...) {
    struct DRXmlParseCtx * ctx = (struct DRXmlParseCtx *)(inputCtx);
    va_list ap;

    va_start(ap, msg);

    CPE_ERROR_SET_LEVEL(ctx->m_em, CPE_EL_WARNING);
    CPE_ERROR_SET_ERRNO(ctx->m_em, CPE_DR_ERROR_XML_PARSE);
    cpe_error_do_notify_var(ctx->m_em, msg, ap);

    va_end(ap);
}

static void dr_build_xml_errorSAX(void * inputCtx, const char *msg, ...) {
    struct DRXmlParseCtx * ctx = (struct DRXmlParseCtx *)(inputCtx);
    va_list ap;

    va_start(ap, msg);

    CPE_ERROR_SET_LEVEL(ctx->m_em, CPE_EL_ERROR);
    CPE_ERROR_SET_ERRNO(ctx->m_em, CPE_DR_ERROR_XML_PARSE);
    cpe_error_do_notify_var(ctx->m_em, msg, ap);

    va_end(ap);
}

static xmlSAXHandler g_dr_xml_handler = {
    NULL /* internalSubsetSAXFunc internalSubset */
    , NULL /* isStandaloneSAXFunc isStandalone */
    , NULL /* hasInternalSubsetSAXFunc hasInternalSubset */
    , NULL /* hasExternalSubsetSAXFunc hasExternalSubset */
    , NULL /* resolveEntitySAXFunc resolveEntity */
    , NULL /* getEntitySAXFunc getEntity */
    , NULL /* entityDeclSAXFunc entityDecl */
    , NULL /* notationDeclSAXFunc notationDecl */
    , NULL /* attributeDeclSAXFunc attributeDecl */
    , NULL /* elementDeclSAXFunc elementDecl */
    , NULL /* unparsedEntityDeclSAXFunc unparsedEntityDecl */
    , NULL /* setDocumentLocatorSAXFunc setDocumentLocator */
    , NULL /* startDocumentSAXFunc startDocument */
    , NULL /* endDocumentSAXFunc endDocument */
    , NULL /* startElementSAXFunc startElement */
    , NULL /* endElementSAXFunc endElement */
    , NULL /* referenceSAXFunc reference */
    , NULL /* charactersSAXFunc characters */
    , NULL /* ignorableWhitespaceSAXFunc ignorableWhitespace */
    , NULL /* processingInstructionSAXFunc processingInstruction */
    , NULL /* commentSAXFunc comment */
    , dr_build_xml_warningSAX /* warningSAXFunc warning */
    , dr_build_xml_errorSAX /* errorSAXFunc error */
    , NULL /* fatalErrorSAXFunc fatalError; unused error() get all the errors * */
    , NULL /* getParameterEntitySAXFunc getParameterEntity */
    , NULL /* cdataBlockSAXFunc cdataBlock */
    , NULL /* externalSubsetSAXFunc externalSubset */
    , XML_SAX2_MAGIC /* unsigned int initialized */
    , NULL /* void *_private */
    , dr_build_xml_startElement /* startElementNsSAX2Func startElementNs */
    , dr_build_xml_endElement /* endElementNsSAX2Func endElementNs */
    , NULL /* xmlStructuredErrorFunc serror */
};

void dr_build_xml_parse_ctx_init(struct DRXmlParseCtx * ctx) {
    bzero(ctx, sizeof(struct DRXmlParseCtx));

    ctx->m_state = PS_Init;
}

void dr_build_xml_parse_ctx_clear(struct DRXmlParseCtx * ctx) {
    if (ctx->m_metaLib) {
        dr_inbuild_free_lib(ctx->m_metaLib);
        ctx->m_metaLib = NULL;
    }
}

int  dr_create_lib_from_xml(
    mem_buffer_t buffer,
    const char* buf,
    int bufSize,
    FILE * errorFp)
{
    CPE_DEF_ERROR_MONITOR(em, cpe_error_log_to_file, errorFp);

    return dr_create_lib_from_xml_ex(buffer, buf, bufSize, &em);
}

void dr_create_lib_from_xml_ex_i(
    mem_buffer_t buffer,
    const char* buf,
    int bufSize,
    int * ret,
    error_monitor_t em)
{
    xmlParserCtxtPtr parseCtx = NULL;
    struct DRXmlParseCtx ctx;

    dr_build_xml_parse_ctx_init(&ctx);
    ctx.m_metaLib = dr_inbuild_create_lib();
    if (ctx.m_metaLib == NULL) {
        *ret = -1;
        return;
    }

    ctx.m_em = em;

    parseCtx = xmlCreatePushParserCtxt(&g_dr_xml_handler, &ctx, buf, bufSize, NULL);

    xmlParseChunk(parseCtx, NULL, 0, 1);
    xmlFreeParserCtxt(parseCtx);

    dr_inbuild_build_lib(buffer, ctx.m_metaLib, em);

    dr_build_xml_parse_ctx_clear(&ctx);
}

int dr_create_lib_from_xml_ex(
    mem_buffer_t buffer,
    const char* buf,
    int bufSize,
    error_monitor_t em)
{
    int ret = 0;

    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        dr_create_lib_from_xml_ex_i(buffer, buf, bufSize, &ret, em);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        dr_create_lib_from_xml_ex_i(buffer, buf, bufSize, &ret, &logError);
    }

    return ret;
}
