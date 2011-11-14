#ifndef CPE_DR_CTYPE_OPS_H
#define CPE_DR_CTYPE_OPS_H
#include "cpe/utils/stream.h"
#include "dr_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tagDRCTypeInfo {
    int m_id;
    char m_name[32];
    size_t m_size;

    /*type operations*/
    int (* read_from_string)(void * output, const char * input);
    int (* printf_to_stream)(write_stream_t output, const void * input);
};

/*n: >= 0    name-length
     < 0     use strlen(name)
*/
const struct tagDRCTypeInfo *
dr_find_ctype_info_by_name(const char * name);

const struct tagDRCTypeInfo *
dr_find_ctype_info_by_type(int typeId);

#ifdef __cplusplus
}
#endif


#endif
