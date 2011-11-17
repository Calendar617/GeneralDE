#ifndef CPE_DR_METALIB_MANAGE_H
#define CPE_DR_METALIB_MANAGE_H
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_external.h"

#ifdef __cplusplus
extern "C" {
#endif
	
CPE_DR_API LPDRMETA dr_lib_find_meta_by_name(LPDRMETALIB metaLib, const char* name);
CPE_DR_API LPDRMETA dr_lib_find_meta_by_id(LPDRMETALIB metaLib, int id);
CPE_DR_API int dr_meta_based_version(LPDRMETA meta);
CPE_DR_API int dr_meta_current_version(LPDRMETA meta);
CPE_DR_API int dr_meta_type(LPDRMETA meta);
CPE_DR_API const char *dr_meta_name(LPDRMETA meta);
CPE_DR_API const char *dr_meta_desc(LPDRMETA meta);
CPE_DR_API int dr_meta_id(LPDRMETA meta);
CPE_DR_API size_t dr_meta_size(LPDRMETA meta);
CPE_DR_API int dr_meta_align(LPDRMETA meta);
CPE_DR_API int dr_meta_entry_num(LPDRMETA meta);
CPE_DR_API LPDRMETAENTRY dr_meta_entry_at(LPDRMETA meta, int idx);
CPE_DR_API int dr_meta_find_entry_idx_by_name(LPDRMETA meta, const char* name);
CPE_DR_API int dr_meta_find_entry_idx_by_id(LPDRMETA meta, int id);
CPE_DR_API char *dr_meta_off_to_path(LPDRMETA meta, int off, char * buf, size_t bufSize);
CPE_DR_API int dr_meta_path_to_off(LPDRMETA meta, const char * path);
CPE_DR_API LPDRMETAENTRY dr_meta_find_entry_by_name(LPDRMETA meta, const char* name);
CPE_DR_API LPDRMETAENTRY dr_meta_find_entry_by_path(LPDRMETA meta, const char* entryPath);

CPE_DR_API int dr_entry_version(LPDRMETAENTRY entry);
CPE_DR_API const char *dr_entry_name(LPDRMETAENTRY entry);
CPE_DR_API const char *dr_entry_cname(LPDRMETAENTRY entry);
CPE_DR_API const char *dr_entry_desc(LPDRMETAENTRY entry);
CPE_DR_API LPDRMETA dr_entry_ref_meta(LPDRMETAENTRY entry);
CPE_DR_API LPDRMETA dr_entry_self_meta(LPDRMETAENTRY entry);
CPE_DR_API LPDRMACROSGROUP dr_entry_macrosgroup(LPDRMETAENTRY entry);
CPE_DR_API int dr_entry_id(LPDRMETAENTRY entry);
CPE_DR_API size_t dr_entry_size(LPDRMETAENTRY entry);
CPE_DR_API int dr_entry_type(LPDRMETAENTRY entry);
CPE_DR_API int dr_entry_array_count(LPDRMETAENTRY entry);
CPE_DR_API const char *dr_entry_customattr(LPDRMETALIB metaLib, LPDRMETAENTRY entry);

CPE_DR_API int dr_lib_find_macro_value(int *a_piID, LPDRMETALIB metaLib, const  char *name);
CPE_DR_API LPDRMACRO dr_lib_macro_at(LPDRMETALIB metaLib, int a_iIdx);

CPE_DR_API int dr_lib_macro_num(LPDRMETALIB metaLib);
CPE_DR_API const char* dr_macro_name(LPDRMETALIB metaLib, LPDRMACRO macro);
CPE_DR_API int dr_macro_value(LPDRMACRO macro);
CPE_DR_API const char* dr_macro_desc(LPDRMETALIB metaLib, LPDRMACRO macro);

CPE_DR_API LPDRMACROSGROUP dr_macro_macrosgroup(LPDRMETALIB metaLib, LPDRMACRO macro);

CPE_DR_API int dr_lib_macrosgroup_num(LPDRMETALIB metaLib);
CPE_DR_API LPDRMACROSGROUP dr_lib_macrosgroup_at(LPDRMETALIB metaLib, int a_iIdx);
CPE_DR_API const char* dr_macrosgroup_name(LPDRMACROSGROUP macroGroup);
CPE_DR_API int dr_macrosgroup_macro_num(LPDRMACROSGROUP macroGroup);
CPE_DR_API LPDRMACRO dr_macrosgroup_macro_at(LPDRMETALIB metaLib, LPDRMACROSGROUP macroGroup, int idx);

#ifdef __cplusplus
}
#endif


#endif
