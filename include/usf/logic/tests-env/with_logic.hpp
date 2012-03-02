#ifndef USF_LOGIC_TESTENV_WITH_LOGIC_H
#define USF_LOGIC_TESTENV_WITH_LOGIC_H
#include "../logic_manage.h"
#include "../logic_executor.h"
#include "../logic_context.h"
#include "cpe/utils/tests-env/test-env.hpp"

namespace usf { namespace logic { namespace testenv {

class with_logic : public ::testenv::env<> {
public:
    with_logic();

    void SetUp();
    void TearDown();

    logic_manage_t t_logic_manager(void);


    logic_context_t t_logic_context_create(size_t capacity = 0, logic_require_id_t id = INVALID_LOGIC_CONTEXT_ID);
    logic_context_t t_logic_context_create(const char * cfg, size_t capacity = 0, logic_require_id_t id = INVALID_LOGIC_CONTEXT_ID);
    logic_context_t t_logic_context_create(cfg_t cfg, size_t capacity = 0, logic_require_id_t id = INVALID_LOGIC_CONTEXT_ID);

    logic_context_t t_logic_context_find(logic_context_id_t id);
    logic_context_t t_logic_context(logic_context_id_t id);

    void t_logic_context_install_data(logic_context_t context, const char * cfg);
    void t_logic_context_install_data(logic_context_t context, cfg_t cfg);

    void t_logic_execute(logic_context_t context, logic_executor_t executor);

    logic_executor_t t_logic_executor_build(cfg_t cfg, logic_executor_build_fun_t fun, void * ctx, error_monitor_t em = 0);
    logic_executor_t t_logic_executor_build(const char * cfg, logic_executor_build_fun_t fun, void * ctx, error_monitor_t em = 0);

    const char * t_logic_executor_dump(logic_executor_t executor);

private:
    logic_manage_t m_logic_manager;
};

}}}

#endif
