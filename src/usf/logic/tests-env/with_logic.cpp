#include "cpe/utils/buffer.h"
#include "cpe/utils/stream_buffer.h"
#include "cpe/utils/tests-env/with_em.hpp"
#include "cpe/cfg/tests-env/with_cfg.hpp"
#include "gd/app/tests-env/with_app.hpp"
#include "usf/logic/logic_build.h"
#include "usf/logic/tests-env/with_logic.hpp"

namespace usf { namespace logic { namespace testenv {

with_logic::with_logic() : m_logic_manager(NULL) {
}

void with_logic::SetUp() {
    m_logic_manager =
        logic_manage_create(
            0,
            envOf<gd::app::testenv::with_app>().t_app(),
            t_allocrator());
    EXPECT_TRUE(m_logic_manager) << "logic_manager create fail!";
}

void with_logic::TearDown() {
    if (m_logic_manager) {
        logic_manage_free(m_logic_manager);
        m_logic_manager = 0;
    }
}

logic_manage_t
with_logic::t_logic_manager(void) {
    EXPECT_TRUE(m_logic_manager) << "logic_manager not create!";
    return m_logic_manager;
}

logic_executor_t
with_logic::t_logic_executor_build(cfg_t cfg, logic_executor_build_fun_t fun, void * ctx, error_monitor_t em) {
    if (em == 0) {
        if (utils::testenv::with_em * with_em = tryEnvOf<utils::testenv::with_em>()) {
            em = with_em->t_em();
        }
    }

    return logic_executor_build(t_logic_manager(), cfg, fun, ctx, em);
}

logic_executor_t
with_logic::t_logic_executor_build(const char * cfg, logic_executor_build_fun_t fun, void * ctx, error_monitor_t em) {
    return t_logic_executor_build(
        envOf<cpe::cfg::testenv::with_cfg>().t_cfg_parse(cfg), fun, ctx, em);
}

const char *
with_logic::t_logic_executor_dump(logic_executor_t executor) {
    struct mem_buffer buffer;
    mem_buffer_init(&buffer, 0);

    write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(&buffer);
    logic_executor_dump(executor, (write_stream_t)&stream, 0);
    stream_putc((write_stream_t)&stream, 0);

    const char * r =  t_tmp_strdup((const char *)mem_buffer_make_continuous(&buffer, 0));
    mem_buffer_clear(&buffer);

    return r;
}

}}}
