#ifndef CPE_DR_CTYPE_OPS_H
#define CPE_DR_CTYPE_OPS_H
#include "dr_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*n: >= 0    name-length
     < 0     use strlen(name)
*/
const struct tagDRCTypeInfo *
dr_find_ctype_info_by_name(const char * name, size_t n);

const struct tagDRCTypeInfo *
dr_find_ctype_info_by_type(int typeId);

#ifdef __cplusplus
}
#endif


#endif
