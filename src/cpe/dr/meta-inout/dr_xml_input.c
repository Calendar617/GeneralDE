#include "libxml/tree.h"
#include "libxml/xmlstring.h"
#include "cpe/dr/dr_metalib_build.h"
#include "cpe/dr/dr_metalib_xml.h"
#include "cpe/dr/dr_error.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"
#include "dr_inbuild.h"
#include "dr_XMLtags.h"

enum DRXmlParseState {
    PS_Init
    , PS_InMetaLib
    , PS_InMeta
};

#define DR_XML_PARSE_ERROR_MSG_LEN 256

struct DRXmlParseCtx {
    struct DRInBuildMetaLib * m_metaLib;
    struct DRInBuildMeta * m_curentMeta;
    enum DRXmlParseState m_state;
    void * m_userData;
    dr_inbuild_log_fun_t m_errorProcessor;
    char m_errorBuf[DR_XML_PARSE_ERROR_MSG_LEN];
};

static void dr_build_xml_notify_error(struct DRXmlParseCtx * ctx, int e, char const * msg) {
    if (ctx->m_errorProcessor) {
        ctx->m_errorProcessor(
            ctx->m_userData,
            NULL,
            -1,
            CPE_ERR_MAKE(CPE_MODID_DR, e),
            msg == NULL ? dr_error_string(e) : msg);
    }
}

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
        return;
    }

    for(index = 0, indexAttribute = 0;
        indexAttribute < nb_attributes;
        ++indexAttribute, index += 5)
    {
        const xmlChar *localname = attributes[index];
        const xmlChar *prefix = attributes[index+1];
        const xmlChar *nsURI = attributes[index+2];
        const xmlChar *valueBegin = attributes[index+3];
        const xmlChar *valueEnd = attributes[index+4];

        int len = valueEnd - valueBegin;

        if (strcmp((char const *)localname, CPE_DR_TAG_NAME) == 0) {
            memcpy(
                ctx->m_metaLib->m_data.szName,
                (char const *)valueBegin,
                len > CPE_DR_NAME_LEN ? CPE_DR_NAME_LEN : len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_TAGSET_VERSION) == 0) {
            strncpy(
                buf,
                (char const *)valueBegin, 
                len >= INTEGER_BUF_LEN ? INTEGER_BUF_LEN - 1 : len);
            sscanf(buf, "%d", &ctx->m_metaLib->m_data.iTagSetVersion);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_VERSION) == 0) {
            haveVersion = 1;
            strncpy(
                buf,
                (char const *)valueBegin, 
                len >= INTEGER_BUF_LEN ? INTEGER_BUF_LEN - 1 : len);
            sscanf(buf, "%d", &ctx->m_metaLib->m_data.iVersion);
        }
        else {
        }
    }

    if (!haveVersion || ctx->m_metaLib->m_data.iVersion < 0) {
        dr_build_xml_notify_error(ctx, CPE_DR_ERROR_NO_VERSION, NULL);
    }

    if (strlen(ctx->m_metaLib->m_data.szName) == 0) {
        dr_build_xml_notify_error(ctx, CPE_DR_ERROR_METALIB_ROOT_NO_NAME, NULL);
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
        dr_build_xml_notify_error(ctx, CPE_DR_ERROR_NO_MEMORY, NULL);
        return;
    }

    int haveError = 0;

    for(index = 0, indexAttribute = 0;
        indexAttribute < nb_attributes && !haveError;
        ++indexAttribute, index += 5)
    {
        const xmlChar *localname = attributes[index];
        const xmlChar *prefix = attributes[index+1];
        const xmlChar *nsURI = attributes[index+2];
        const xmlChar *valueBegin = attributes[index+3];
        const xmlChar *valueEnd = attributes[index+4];

        int len = valueEnd - valueBegin;

        if (strcmp((char const *)localname, CPE_DR_TAG_NAME) == 0) {
            memcpy(
                newMacro->m_name,
                (char const *)valueBegin,
                len > CPE_DR_MACRO_LEN ? CPE_DR_MACRO_LEN : len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_DESCIPTION) == 0) {
            memcpy(
                newMacro->m_desc,
                (char const *)valueBegin,
                len > CPE_DR_DESC_LEN ? CPE_DR_DESC_LEN : len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_MACRO_VALUE) == 0) {
            haveValue = 1;
            strncpy(
                buf,
                (char const *)valueBegin,
                len >= INTEGER_BUF_LEN ? INTEGER_BUF_LEN - 1 : len);
            sscanf(buf, "%d", &newMacro->m_data.m_value);
        }
        else {
        }
    }

    if (strlen(newMacro->m_name) == 0) {
        dr_build_xml_notify_error(ctx, CPE_DR_ERROR_MACRO_NO_NAME_ATTR, NULL);
        haveError = 1;
    }

    if (!haveValue) {
        dr_build_xml_notify_error(ctx, CPE_DR_ERROR_MACRO_NO_VALUE, NULL);
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
        dr_build_xml_notify_error(ctx, CPE_DR_ERROR_NO_MEMORY, NULL);
        return;
    }

    newMeta->m_data.m_type = metaType;

    for(index = 0, indexAttribute = 0;
        indexAttribute < nb_attributes && !haveError;
        ++indexAttribute, index += 5)
    {
        const xmlChar *localname = attributes[index];
        const xmlChar *prefix = attributes[index+1];
        const xmlChar *nsURI = attributes[index+2];
        const xmlChar *valueBegin = attributes[index+3];
        const xmlChar *valueEnd = attributes[index+4];

        int len = valueEnd - valueBegin;

        if (strcmp((char const *)localname, CPE_DR_TAG_NAME) == 0) {
            memcpy(
                newMeta->m_name,
                (char const *)valueBegin,
                len > CPE_DR_NAME_LEN ? CPE_DR_NAME_LEN : len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_DESCIPTION) == 0) {
            int copyCount = len > CPE_DR_DESC_LEN ? CPE_DR_DESC_LEN : len;

            memcpy(
                newMeta->m_desc,
                (char const *)valueBegin,
                len > CPE_DR_DESC_LEN ? CPE_DR_DESC_LEN : len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_ID) == 0) {
            strncpy(
                buf,
                (char const *)valueBegin,
                len >= INTEGER_BUF_LEN ? INTEGER_BUF_LEN - 1 : len);
            sscanf(buf, "%d", &newMeta->m_data.m_id);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_VERSION) == 0) {
            strncpy(
                buf,
                (char const *)valueBegin,
                len >= INTEGER_BUF_LEN ? INTEGER_BUF_LEN - 1 : len);
            sscanf(buf, "%d", &version);
        }
        else {
        }
    }

    if (haveError) {
        dr_inbuild_metalib_remove_meta(ctx->m_metaLib, newMeta);
        return;
    }

    newMeta->m_data.m_current_version =
        version == -1 ? ctx->m_metaLib->m_data.iVersion : version;

    newMeta->m_data.m_based_version = newMeta->m_data.m_current_version;

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
    const struct tagDRCTypeInfo * ctypeInfo = NULL;
    int indexAttribute = 0;
    int index = 0;
    int version = -1;
    int haveError = 0;

    if (ctx->m_state != PS_InMeta || ctx->m_curentMeta == NULL) {
        return;
    }

    struct DRInBuildMetaEntry * newEntry = dr_inbuild_meta_add_entry(ctx->m_curentMeta);
    if (newEntry == NULL) {
        dr_build_xml_notify_error(ctx, CPE_DR_ERROR_NO_MEMORY, NULL);
        return;
    }

    for(index = 0, indexAttribute = 0;
        indexAttribute < nb_attributes && !haveError;
        ++indexAttribute, index += 5)
    {
        const xmlChar *localname = attributes[index];
        const xmlChar *prefix = attributes[index+1];
        const xmlChar *nsURI = attributes[index+2];
        const xmlChar *valueBegin = attributes[index+3];
        const xmlChar *valueEnd = attributes[index+4];

        int len = valueEnd - valueBegin;

        if (strcmp((char const *)localname, CPE_DR_TAG_NAME) == 0) {
            memcpy(
                newEntry->m_name,
                (char const *)valueBegin,
                len > CPE_DR_NAME_LEN ? CPE_DR_NAME_LEN : len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_CNNAME) == 0) {
            memcpy(
                newEntry->m_cname,
                (char const *)valueBegin,
                len > CPE_DR_CHNAME_LEN ? CPE_DR_CHNAME_LEN : len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_DESCIPTION) == 0) {
            memcpy(
                newEntry->m_desc,
                (char const *)valueBegin,
                len > CPE_DR_DESC_LEN ? CPE_DR_DESC_LEN : len);
        }
        else if (strcmp((char const *)localname, CPE_DR_TAG_TYPE) == 0) {
            ctypeInfo = dr_find_ctype_info_by_name(valueBegin, len);
        }
        else {
        }
    }

    if (ctypeInfo == NULL) {
        dr_build_xml_notify_error(ctx, CPE_DR_ERROR_ENTRY_NO_TYPE, NULL);
        haveError = 1;
    }
    else {
        newEntry->m_data.m_type = ctypeInfo->m_id;
        newEntry->m_data.m_unitsize = ctypeInfo->m_size;
    }

    if (strlen(newEntry->m_name) == 0) {
        dr_build_xml_notify_error(ctx, CPE_DR_ERROR_META_NO_NAME, "entry no name");
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
}

static void dr_build_xml_errorSAX(void * inputCtx, const char *msg, ...) {
    struct DRXmlParseCtx * ctx = (struct DRXmlParseCtx *)(inputCtx);
    va_list ap;

    va_start(ap, msg);
    vsnprintf(ctx->m_errorBuf, DR_XML_PARSE_ERROR_MSG_LEN, msg, ap);
    va_end(ap);

    dr_build_xml_notify_error(ctx, CPE_DR_ERROR_XML_PARSE, ctx->m_errorBuf);
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
    , NULL /* fatalErrorSAXFunc fatalError; /* unused error() get all the errors * */
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

static void dr_create_lib_from_xml_error_processor(
    void * p, const char * file, int line, int errno, const char * msg)
{
    FILE * errorFp;

    if (p == NULL) {
        return;
    }

    errorFp = (FILE *)p;
}

int  dr_create_lib_from_xml(
    LPDRMETALIB * metaLib,
    const char* buf,
    int bufSize,
    FILE * errorFp)
{
    return dr_create_lib_from_xml_ex(
        metaLib,
        buf, bufSize,
        errorFp, dr_create_lib_from_xml_error_processor);
}

int dr_create_lib_from_xml_ex(
    LPDRMETALIB * metaLib,
    const char* buf,
    int bufSize,
    void * userData, 
    dr_inbuild_log_fun_t errorProcessor)
{
    xmlParserCtxtPtr parseCtx = NULL;
    struct DRXmlParseCtx ctx;
    int ret = 0;

    dr_build_xml_parse_ctx_init(&ctx);
    ctx.m_metaLib = dr_inbuild_create_lib();
    if (ctx.m_metaLib == NULL) {
        return -1;
    }

    ctx.m_userData = userData;
    ctx.m_errorProcessor = errorProcessor;

    parseCtx = xmlCreatePushParserCtxt(&g_dr_xml_handler, &ctx, buf, bufSize, NULL);
    xmlParseChunk(parseCtx, NULL, 0, 1);
    xmlFreeParserCtxt(parseCtx);

    ret = dr_inbuild_build_lib(metaLib, ctx.m_metaLib, userData, errorProcessor);

    dr_build_xml_parse_ctx_clear(&ctx);
    
    return ret;
}
