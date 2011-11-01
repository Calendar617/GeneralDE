#ifndef CPE_DR_INTERNALS_TYPES_H
#define CPE_DR_INTERNALS_TYPES_H
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_ctypes_info.h"

#pragma pack(push,1)

struct tagDRMetaLib {
    /*000*/ int16_t m_magic;
    /*002*/ int16_t m_build_version;
    int8_t reserve1[4];
    /*008*/ int32_t m_size;
    int8_t reserve2[16];
    /*028*/ int32_t m_id;
    /*032*/ int32_t m_tag_set_version;
    int8_t reserve3[4];
    /*040*/ int32_t m_meta_max_count;
    /*044*/ int32_t m_meta_count;
    /*048*/ int32_t m_macro_max_count;
    /*052*/ int32_t m_macro_count;
    /*056*/ int32_t m_macrogroup_max_count;
    /*056*/ int32_t m_macrogroup_count;
    int8_t reserve4[8];
    /*072*/ int32_t m_version;
    /*076*/ int32_t m_startpos_macro; /*guess*/
    /*080*/ int32_t m_startpos_meta_by_id;
    /*084*/ int32_t m_startpos_meta_by_name;
    /*088*/ int32_t m_startpos_meta_by_unknown; /*what is this index?*/
    /*088*/ int32_t m_startpos_meta;
    /*096*/ int32_t m_startpos_str;
    /*100*/ int32_t m_buf_size_str;
    int8_t reserve7[8];
    /*112*/ int32_t m_buf_size_meta;
    int8_t reserve8[4];
    /*120*/ int32_t m_buf_size_macros;
    int8_t reserve9[24];
    /*148*/ char m_name[CPE_DR_NAME_LEN];
};

struct tagDRMeta {
    int8_t reserve_1[4];
    /*004*/ int32_t m_id;
    /*008*/ int32_t m_based_version;
    /*012*/ int32_t m_current_version;
    /*016*/ int32_t m_type;
    int8_t reserve_2[8];
    /*028*/ int32_t m_data_size;
    int8_t reserve_3[12];


    /*044*/ int32_t m_entry_count;
    int8_t reserve_4[12];
    /*060*/ int32_t m_self_pos;
    int8_t reserve_5[68];
    /*132*/ int32_t m_name_pos;
    /*136*/ int32_t m_desc_pos;
    int8_t reserve_6[44];
};

struct tagDRMetaEntry {
    /*000*/ int32_t m_id;
    int8_t reserve_1[4];
    /*008*/ int32_t m_type;
    /*012*/ int32_t m_name_pos;
    /*016*/ int32_t m_unitsize;
    int8_t reserve_2[16];
    /*036*/ int32_t m_count;
    int8_t reserve_3[80];
    /*120*/ int32_t m_ref_type_pos;
    int8_t reserve_4[20];
    /*144*/ int32_t m_desc_pos;
    /*148*/ int32_t m_cname_pos;
    int8_t reserve_5[12];
    /*164*/ int32_t m_self_to_meta_pos;
    int8_t reserve_6[12];
};/*180*/

struct tagDRMacro {
    int32_t m_name_pos;
    int32_t m_value;
    int32_t m_desc_pos;
    int8_t reserve[4];
};

struct tagDRMacrosGroup {
    int8_t a;
};

struct tagDRCTypeInfo {
    char m_name[32];
    size_t m_size;
};

struct tagDRMetaIdxById {
    int32_t m_id;
    int32_t m_diff_to_base;
};

struct tagDRMetaIdxByName {
    int32_t m_name_pos;
    int32_t m_diff_to_base;
};

#pragma pack(pop)

extern struct tagDRCTypeInfo g_dr_ctypeinfos[CPE_DR_TYPE_MAX + 1];

#endif
