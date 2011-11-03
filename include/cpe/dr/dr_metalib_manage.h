#ifndef CPE_DR_METALIB_MANAGE_H
#define CPE_DR_METALIB_MANAGE_H
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_external.h"

#ifdef __cplusplus
extern "C" {
#endif
	
CPE_DR_API LPDRMETA dr_get_meta_by_name(LPDRMETALIB metaLib, const char* name);
CPE_DR_API LPDRMETA dr_get_meta_by_id(LPDRMETALIB metaLib, int a_iID);
CPE_DR_API int dr_get_meta_based_version(LPDRMETA meta);
CPE_DR_API int dr_get_meta_current_version(LPDRMETA meta);
CPE_DR_API int dr_get_meta_type(LPDRMETA meta);
CPE_DR_API const char *dr_get_meta_name(LPDRMETA meta);
CPE_DR_API const char *dr_get_meta_desc(LPDRMETA meta);
CPE_DR_API int dr_get_meta_id(LPDRMETA meta);
CPE_DR_API size_t dr_get_meta_size(LPDRMETA meta);
CPE_DR_API int dr_get_meta_align(LPDRMETA meta);
CPE_DR_API int dr_get_entry_num(LPDRMETA meta);
CPE_DR_API int dr_entry_path_to_off(LPDRMETA meta, LPDRMETAENTRY *addrOfEntry, DROFF *a_piHOff, const char *a_pszPath);
CPE_DR_API char *dr_entry_off_to_path(LPDRMETA meta, int a_iOff, char * a_pBuf, size_t a_iBufSize);
CPE_DR_API int dr_do_have_autoincrement_entry(LPDRMETA meta);

CPE_DR_API int dr_sizeinfo_to_off(LPDRSIZEINFO a_pstRedirector, LPDRMETA meta, int a_iEntry, const char* name);

CPE_DR_API int dr_get_entry_version(LPDRMETAENTRY entry);
CPE_DR_API const char *dr_get_entry_name(LPDRMETAENTRY entry);
CPE_DR_API const char *dr_get_entry_cname(LPDRMETAENTRY entry);
CPE_DR_API const char *dr_get_entry_desc(LPDRMETAENTRY entry);
CPE_DR_API LPDRMACROSGROUP dr_get_entry_macrosgroup(LPDRMETAENTRY entry);
CPE_DR_API LPDRMETAENTRY dr_get_entryptr_by_name(LPDRMETA meta, const char* name);
CPE_DR_API int dr_get_entry_by_name(int* a_piIdx, LPDRMETA meta, const char* name);
CPE_DR_API int dr_get_entry_by_id(int* a_piIdx, LPDRMETA meta, int a_iId);
CPE_DR_API LPDRMETAENTRY dr_get_entry_by_index(LPDRMETA meta, int a_idxEntry);
CPE_DR_API int dr_get_entry_id(LPDRMETAENTRY entry);
CPE_DR_API const char *dr_get_entry_id_name(LPDRMETALIB metaLib, LPDRMETAENTRY entry);
CPE_DR_API size_t dr_get_entry_unitsize(LPDRMETAENTRY entry);
CPE_DR_API int dr_get_entry_type(LPDRMETAENTRY entry);
CPE_DR_API int dr_get_entry_count(LPDRMETAENTRY entry);
CPE_DR_API LPDRMETA dr_get_entry_type_meta(LPDRMETALIB metaLib, LPDRMETAENTRY entry);
CPE_DR_API LPDRMETAENTRY dr_get_entry_by_path(LPDRMETA meta, const char* a_pszEntryPath);
CPE_DR_API const char *dr_get_entry_customattr(LPDRMETALIB metaLib, LPDRMETAENTRY entry);

CPE_DR_API int dr_get_macro_value(int *a_piID, LPDRMETALIB metaLib, const  char *name);
CPE_DR_API int dr_get_metalib_macro_num(LPDRMETALIB metaLib);
CPE_DR_API LPDRMACRO dr_get_metalib_macro_by_index(LPDRMETALIB metaLib, int a_iIdx);
CPE_DR_API const char* dr_get_macro_name_by_ptr(LPDRMETALIB metaLib, LPDRMACRO macro);
CPE_DR_API int dr_get_macro_value_by_ptr(int *a_piID, LPDRMACRO macro);
CPE_DR_API const char* dr_get_macro_desc_by_ptr(LPDRMETALIB metaLib, LPDRMACRO macro);

CPE_DR_API LPDRMACROSGROUP dr_get_macro_macrosgroup(LPDRMETALIB metaLib, LPDRMACRO macro);

CPE_DR_API int dr_get_macrosgroup_num(LPDRMETALIB metaLib);
CPE_DR_API LPDRMACROSGROUP dr_get_macrosgroup_by_index(LPDRMETALIB metaLib, int a_iIdx);
CPE_DR_API const char* dr_get_macrosgroup_name(LPDRMACROSGROUP a_pstGroup);
CPE_DR_API int dr_get_macrosgroup_macro_num(LPDRMACROSGROUP a_pstGroup);
CPE_DR_API LPDRMACRO dr_get_macrosgroup_macro_by_index(LPDRMETALIB metaLib, LPDRMACROSGROUP a_pstGroup, int a_iIdx);

#ifdef __cplusplus
}
#endif


#endif
