#ifndef CPE_DR_METALIB_MANAGE_H
#define CPE_DR_METALIB_MANAGE_H
#include "cpe/utils/stream.h"
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_external.h"

#ifdef __cplusplus
extern "C" {
#endif
	
LPDRMETA dr_lib_find_meta_by_name(LPDRMETALIB metaLib, const char* name);
LPDRMETA dr_lib_find_meta_by_id(LPDRMETALIB metaLib, int id);
int dr_lib_meta_num(LPDRMETALIB metaLib);
LPDRMETA dr_lib_meta_at(LPDRMETALIB metaLib, int idx);

int dr_meta_based_version(LPDRMETA meta);
int dr_meta_current_version(LPDRMETA meta);
int dr_meta_type(LPDRMETA meta);
const char *dr_meta_name(LPDRMETA meta);
const char *dr_meta_desc(LPDRMETA meta);
int dr_meta_id(LPDRMETA meta);
size_t dr_meta_size(LPDRMETA meta);
int dr_meta_align(LPDRMETA meta);
int dr_meta_entry_num(LPDRMETA meta);
LPDRMETAENTRY dr_meta_entry_at(LPDRMETA meta, int idx);
int dr_meta_find_entry_idx_by_name(LPDRMETA meta, const char* name);
int dr_meta_find_entry_idx_by_id(LPDRMETA meta, int id);
char *dr_meta_off_to_path(LPDRMETA meta, int off, char * buf, size_t bufSize);
int dr_meta_path_to_off(LPDRMETA meta, const char * path, LPDRMETAENTRY * entry);
LPDRMETAENTRY dr_meta_find_entry_by_name(LPDRMETA meta, const char* name);
LPDRMETAENTRY dr_meta_find_entry_by_path(LPDRMETA meta, const char* entryPath);
LPDRMETALIB dr_meta_owner_lib(LPDRMETA meta);

int dr_entry_version(LPDRMETAENTRY entry);
const char * dr_entry_name(LPDRMETAENTRY entry);
const char * dr_entry_cname(LPDRMETAENTRY entry);
const char * dr_entry_desc(LPDRMETAENTRY entry);
const void * dr_entry_dft_value(LPDRMETAENTRY entry);
LPDRMETA dr_entry_ref_meta(LPDRMETAENTRY entry);
LPDRMETA dr_entry_self_meta(LPDRMETAENTRY entry);
LPDRMACROSGROUP dr_entry_macrosgroup(LPDRMETAENTRY entry);
int dr_entry_id(LPDRMETAENTRY entry);
size_t dr_entry_size(LPDRMETAENTRY entry);
size_t dr_entry_element_size(LPDRMETAENTRY entry);
int dr_entry_type(LPDRMETAENTRY entry);
int dr_entry_align(LPDRMETAENTRY entry);
int dr_entry_array_count(LPDRMETAENTRY entry);
LPDRMETAENTRY dr_entry_array_refer_entry(LPDRMETAENTRY entry);
LPDRMETAENTRY dr_entry_select_entry(LPDRMETAENTRY entry);
const char *dr_entry_customattr(LPDRMETALIB metaLib, LPDRMETAENTRY entry);
size_t dr_entry_data_start_pos(LPDRMETAENTRY entry);

int dr_lib_find_macro_value(int *a_piID, LPDRMETALIB metaLib, const  char *name);
LPDRMACRO dr_lib_macro_at(LPDRMETALIB metaLib, int a_iIdx);

int dr_lib_macro_num(LPDRMETALIB metaLib);
const char* dr_macro_name(LPDRMETALIB metaLib, LPDRMACRO macro);
int dr_macro_value(LPDRMACRO macro);
const char* dr_macro_desc(LPDRMETALIB metaLib, LPDRMACRO macro);

LPDRMACROSGROUP dr_macro_macrosgroup(LPDRMETALIB metaLib, LPDRMACRO macro);

int dr_lib_macrosgroup_num(LPDRMETALIB metaLib);
LPDRMACROSGROUP dr_lib_macrosgroup_at(LPDRMETALIB metaLib, int a_iIdx);
const char* dr_macrosgroup_name(LPDRMACROSGROUP macroGroup);
int dr_macrosgroup_macro_num(LPDRMACROSGROUP macroGroup);
LPDRMACRO dr_macrosgroup_macro_at(LPDRMETALIB metaLib, LPDRMACROSGROUP macroGroup, int idx);

void dr_lib_dump(write_stream_t stream, LPDRMETALIB metaLib, int ident);

#ifdef __cplusplus
}
#endif


#endif
