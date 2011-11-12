#ifndef CPE_DR_METALIB_INIT_H
#define CPE_DR_METALIB_INIT_H
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_define.h"
#include "cpe/dr/dr_external.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CPE_DR_API void dr_lib_free(INOUT LPDRMETALIB* ppstLib);
CPE_DR_API LPDRMETALIB dr_lib_clone(OUT void* a_pvDst, INOUT int* a_piBuff, IN LPDRMETALIB a_pstSrcLib);

CPE_DR_API size_t dr_lib_size(IN LPDRMETALIB pstLib);
CPE_DR_API const char *dr_lib_name(IN LPDRMETALIB a_pstLib);
CPE_DR_API int dr_lib_version(IN LPDRMETALIB a_pstLib);
CPE_DR_API int dr_lib_build_version(IN LPDRMETALIB a_pstLib);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

