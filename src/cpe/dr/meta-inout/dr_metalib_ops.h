#ifndef CPE_DR_METAINOUT_INTERNALS_INIT_H
#define CPE_DR_METAINOUT_INTERNALS_INIT_H
#include "cpe/utils/error.h"
#include "cpe/dr/dr_metalib_build.h"
#include "../dr_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int dr_init_lib(IN LPDRMETALIB pstLib, IN const LPDRLIBPARAM pstParam);
int dr_add_metalib_string(LPDRMETALIB metaLib, const char * data, int * usedCount, error_monitor_t em);

LPDRMACRO dr_add_metalib_macro(LPDRMETALIB metaLib, LPDRMACRO macro, error_monitor_t em);

LPDRMETA dr_add_metalib_meta(LPDRMETALIB metaLib, LPDRMETA meta, error_monitor_t em);

LPDRMETAENTRY dr_add_meta_entry( LPDRMETA meta, LPDRMETAENTRY entry, error_monitor_t em);
void dr_meta_complete(LPDRMETA meta, error_monitor_t em);

int dr_calc_meta_use_size(int entryCount);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
