#ifndef CPE_DR_XML_METALIB_H
#define CPE_DR_XML_METALIB_H
#include <stdio.h>
#include "cpe/dr/dr_types.h"
#include "cpe/dr/dr_define.h"
#include "cpe/dr/dr_external.h"
#include "cpe/dr/dr_metalib_build.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CPE_DR_API int dr_create_lib_from_xml(
    LPDRMETALIB * metaLib,
    const char* buf, int bufSize,
    FILE* errorFp);

CPE_DR_API int dr_create_lib_from_xml_ex(
    LPDRMETALIB * metaLib,
    const char* buf, int bufSize,
    error_monitor_t er);

CPE_DR_API int dr_save_xml(
    LPDRMETALIB metaLib, const char * fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
