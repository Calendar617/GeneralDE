#ifndef USF_LOGIC_DATA_H
#define USF_LOGIC_DATA_H
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_data_t logic_data_find(logic_context_t context, const char * name);
logic_data_t logic_data_get_or_create(logic_context_t context, LPDRMETA meta, size_t capacity);
void logic_data_free(logic_data_t data);

LPDRMETA logic_data_meta(logic_data_t data);
const char * logic_data_name(logic_data_t data);
void * logic_data_data(logic_data_t data);
size_t logic_data_capacity(logic_data_t data);

#ifdef __cplusplus
}
#endif

#endif

