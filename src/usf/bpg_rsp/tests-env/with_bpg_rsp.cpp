#include <stdexcept>
#include "cpe/utils/buffer.h"
#include "cpe/utils/stream_buffer.h"
#include "cpe/utils/tests-env/with_em.hpp"
#include "cpe/cfg/tests-env/with_cfg.hpp"
#include "gd/nm/nm_manage.h"
#include "gd/app/tests-env/with_app.hpp"
#include "usf/logic/tests-env/with_logic.hpp"
#include "usf/bpg_rsp/tests-env/with_bpg_rsp.hpp"

namespace usf { namespace bpg_rsp { namespace testenv {

with_bpg_rsp::with_bpg_rsp() {
}

void with_bpg_rsp::SetUp() {
}

void with_bpg_rsp::TearDown() {
    gd_nm_mgr_free_nodes_with_type_name(
        envOf<gd::app::testenv::with_app>().t_nm(),
        "usf_bpg_rsp_manage");
}

bpg_rsp_manage_t
with_bpg_rsp::t_bpg_rsp_manage(const char * name, const char * logic_name) {
    bpg_rsp_manage_t mgr = bpg_rsp_manage_find_nc(envOf<gd::app::testenv::with_app>().t_app(), name);
    if (mgr == NULL) {

        error_monitor_t em = 0;
        if (utils::testenv::with_em * with_em = tryEnvOf<utils::testenv::with_em>()) {
            em = with_em->t_em();
        }

        mgr = bpg_rsp_manage_create(
            envOf<gd::app::testenv::with_app>().t_app(),
            name,
            envOf<usf::logic::testenv::with_logic>().t_logic_manage(logic_name),
            em);
    }

    return mgr;
}

}}}
