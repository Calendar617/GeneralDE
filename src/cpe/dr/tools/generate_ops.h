#ifndef CPE_DR_TOOLS_GENERATE_OPS_H
#define CPE_DR_TOOLS_GENERATE_OPS_H
#include "cpe/utils/stream.h"
#include "cpe/utils/error.h"
#include "cpe/dr/dr_types.h"

int cpe_dr_generate_h(write_stream_t stream, dr_metalib_source_t source, error_monitor_t em);

#endif
