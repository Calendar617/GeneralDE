#ifndef CPE_DR_BUILD_INBUILD_TYPES_H
#define CPE_DR_BUILD_INBUILD_TYPES_H
#include "cpe/utils/buffer.h"
#include "cpe/utils/hash.h"
#include "../dr_internal_types.h"
#include "cpe/pal/pal_queue.h"

struct DRInBuildMacro;
struct DRInBuildMeta;

struct DRInBuildMetaLib {
    struct tagDRLibParam m_data;
    struct mem_buffer m_tmp_buf;

    TAILQ_HEAD(DRInBuildMacroList, DRInBuildMacro) m_macros;
    struct cpe_hash_table m_index_macros;

    TAILQ_HEAD(DRInBuildMetaList, DRInBuildMeta) m_metas;
    struct cpe_hash_table m_index_metas;
};

struct DRInBuildMeta {
    TAILQ_ENTRY(DRInBuildMeta) m_next;
    TAILQ_HEAD(DRInBuildMetaEntryList, DRInBuildMetaEntry) m_entries;
    struct cpe_hash_entry m_hh;

    int m_entries_count;

    struct tagDRMeta m_data;
    char * m_desc;
    char * m_name;
};

struct DRInBuildMetaEntry {
    TAILQ_ENTRY(DRInBuildMetaEntry) m_next;

    struct tagDRMetaEntry m_data;
    int m_ignore;
    char * m_name;
    char * m_desc;
    char * m_cname;
    char * m_ref_type_name;
    char * m_selector_path;
    char * m_refer_path;
    char * m_dft_value;
};

struct DRInBuildMacro {
    TAILQ_ENTRY(DRInBuildMacro) m_next;
    struct cpe_hash_entry m_hh;

    struct tagDRMacro m_data;
    char * m_name;
    char * m_desc;
};

#endif


