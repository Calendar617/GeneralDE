#ifndef CPE_DR_BUILD_INBUILD_TYPES_H
#define CPE_DR_BUILD_INBUILD_TYPES_H
#include "cpe/utils/buffer.h"
#include "../dr_internal_types.h"
#include "cpe/pal/queue.h"

struct DRInBuildMacro;
struct DRInBuildMeta;

struct DRInBuildMetaLib {
    struct tagDRLibParam m_data;
    struct mem_buffer m_tmp_buf;

    TAILQ_HEAD(DRInBuildMacroList, DRInBuildMacro) m_macros;
    TAILQ_HEAD(DRInBuildMetaList, DRInBuildMeta) m_metas;
};

struct DRInBuildMeta {
    TAILQ_ENTRY(DRInBuildMeta) m_next;
    TAILQ_HEAD(DRInBuildMetaEntryList, DRInBuildMetaEntry) m_entries;
    int m_entries_count;

    struct tagDRMeta m_data;
    char m_desc[CPE_DR_DESC_LEN];
    char m_name[CPE_DR_NAME_LEN];
};

struct DRInBuildMetaEntry {
    TAILQ_ENTRY(DRInBuildMetaEntry) m_next;

    struct tagDRMetaEntry m_data;
    char m_name[CPE_DR_NAME_LEN];
    char m_desc[CPE_DR_DESC_LEN];
    char m_cname[CPE_DR_CHNAME_LEN];
    char * m_ref_type_name;
    char * m_selector_path;
};

struct DRInBuildMacro {
    TAILQ_ENTRY(DRInBuildMacro) m_next;

    struct tagDRMacro m_data;
    char m_name[CPE_DR_MACRO_LEN];
    char m_desc[CPE_DR_DESC_LEN];
};

#endif


