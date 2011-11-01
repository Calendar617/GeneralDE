#ifndef  CPE_DR_EXTERNAL_H
#define CPE_DR_EXTERNAL_H

#include "cpe/pal/api_external.h"

#ifdef CPE_DR_BUILD_DLL
#    define CPE_DR_API CPE_API_EXPORT
#else
#    define CPE_DR_API CPE_API_IMPORT
#endif
  
#endif /*TDR_EXTERNAL_H*/


