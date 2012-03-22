#include <cassert>
#include "gdpp/app/Log.hpp"
#include "usf/bpg_rsp/bpg_rsp_addition.h"
#include "usfpp/bpg_rsp/RspOpContext.hpp"

namespace Usf { namespace Bpg {

void RspOpContext::addAdditionData(uint32_t meta_id) {
    if (bpg_rsp_addition_data_add(*this, meta_id) != 0) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "add addition data %d fail!", meta_id);
    }
}

}}
