#ifndef USF_DM_ROLE_H
#define USF_DM_ROLE_H
#include "dm_role_types.h"

#ifdef __cplusplus
extern "C" {
#endif

dm_role_t
dm_role_create(
    dm_role_manage_t mgr,
    void * data, size_t data_size,
    const char ** duplicate_index);

void dm_role_free(dm_role_t dm_role);

dm_role_id_t dm_role_id(dm_role_t dm_role);
void * dm_role_data(dm_role_t dm_role);
size_t dm_role_data_capacity(dm_role_t dm_role);
LPDRMETA dm_role_meta(dm_role_t dm_role);

dm_role_t dm_role_find_by_id(dm_role_manage_t mgr, dm_role_id_t id);

dm_role_t dm_role_find_by_index_int8(dm_role_manage_t mgr, const char * idx_name, int8_t input);
dm_role_t dm_role_find_by_index_uint8(dm_role_manage_t mgr, const char * idx_name, uint8_t input);
dm_role_t dm_role_find_by_index_int16(dm_role_manage_t mgr, const char * idx_name, int16_t input);
dm_role_t dm_role_find_by_index_uint16(dm_role_manage_t mgr, const char * idx_name, uint16_t input);
dm_role_t dm_role_find_by_index_int32(dm_role_manage_t mgr, const char * idx_name, int32_t input);
dm_role_t dm_role_find_by_index_uint32(dm_role_manage_t mgr, const char * idx_name, uint32_t input);
dm_role_t dm_role_find_by_index_int64(dm_role_manage_t mgr, const char * idx_name, int64_t input);
dm_role_t dm_role_find_by_index_uint64(dm_role_manage_t mgr, const char * idx_name, uint64_t input);
dm_role_t dm_role_find_by_index_float(dm_role_manage_t mgr, const char * idx_name, float input);
dm_role_t dm_role_find_by_index_double(dm_role_manage_t mgr, const char * idx_name, double input);
dm_role_t dm_role_find_by_index_string(dm_role_manage_t mgr, const char * idx_name, const char * input);
dm_role_t dm_role_find_by_index_ctype(dm_role_manage_t mgr, const char * idx_name, const void * input, int input_type);


#ifdef __cplusplus
}
#endif

#endif
