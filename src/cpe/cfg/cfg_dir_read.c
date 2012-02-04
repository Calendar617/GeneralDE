#include <string.h>
#include <assert.h>
#include "cpe/utils/file.h"
#include "cpe/utils/stream_file.h"
#include "cpe/dr/dr_ctypes_op.h"
#include "cpe/cfg/cfg_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "cfg_internal_types.h"
#include "cfg_internal_ops.h"

struct cfg_dir_read_ctx {
    cfg_policy_t m_policy;
    cfg_t m_curent;
    error_monitor_t m_em;
    struct mem_buffer m_tbuffer;

    struct {
        cfg_t m_node;
    } m_node_stack[CPE_CFG_MAX_LEVEL];
    int m_node_stack_pos;
};

dir_visit_next_op_t cfg_read_dir_on_enter(const char * full, const char * base, void * ctx) {
    struct cfg_dir_read_ctx * readCtx = (struct cfg_dir_read_ctx *)ctx;

    assert(readCtx);
    if (readCtx->m_curent == NULL || cfg_is_value(readCtx->m_curent)) return dir_visit_next_exit;

    ++readCtx->m_node_stack_pos;
    if (readCtx->m_node_stack_pos >= CPE_CFG_MAX_LEVEL) {
        readCtx->m_curent = NULL;
        return dir_visit_next_ignore;
    }

    readCtx->m_node_stack[readCtx->m_node_stack_pos].m_node = readCtx->m_curent;

    if (readCtx->m_curent->m_type == CPE_CFG_TYPE_STRUCT) {
        readCtx->m_curent = cfg_struct_add_struct(readCtx->m_curent, base, readCtx->m_policy);
    }
    else {
        readCtx->m_curent = cfg_seq_add_struct(readCtx->m_curent);
    }

    return dir_visit_next_go;
}

dir_visit_next_op_t cfg_read_dir_on_leave(const char * full, const char * base, void * ctx) {
    struct cfg_dir_read_ctx * readCtx = (struct cfg_dir_read_ctx *)ctx;

    assert(readCtx);
    if (readCtx->m_curent == NULL) return dir_visit_next_exit;

    if (readCtx->m_node_stack_pos >= 0 && readCtx->m_node_stack_pos < CPE_CFG_MAX_LEVEL) {
        readCtx->m_curent = readCtx->m_node_stack[readCtx->m_node_stack_pos].m_node;
    }
    else {
        readCtx->m_curent = NULL;
    }
    --readCtx->m_node_stack_pos;

    return dir_visit_next_go;
}

dir_visit_next_op_t cfg_read_dir_on_file(const char * full, const char * base, void * ctx) {
    struct cfg_dir_read_ctx * readCtx = (struct cfg_dir_read_ctx *)ctx;
    struct read_stream_file fstream;
    FILE * fp;
    const char * fileSuffix;

    if (readCtx->m_curent == NULL || cfg_is_value(readCtx->m_curent)) return dir_visit_next_ignore;

    fileSuffix = file_name_suffix(base);
    if (strcmp(fileSuffix, "yml") != 0
        && strcmp(fileSuffix, "yaml") != 0
        )
    {
        return dir_visit_next_go;
    }

    CPE_ERROR_SET_FILE(readCtx->m_em, full);
    fp = file_stream_open(full, "r", readCtx->m_em);
    CPE_ERROR_SET_FILE(readCtx->m_em, NULL);

    if (fp ==NULL) return dir_visit_next_go;

    read_stream_file_init(&fstream, fp, readCtx->m_em);

    
    cfg_read_with_name(
        readCtx->m_curent,
        file_name_base(base, &readCtx->m_tbuffer),
        (read_stream_t)&fstream,
        readCtx->m_policy,
        readCtx->m_em);
    
    file_stream_close(fp, readCtx->m_em);

    return dir_visit_next_go;
}

static void cfg_read_dir_i(cfg_t cfg, const char * path, cfg_policy_t policy, error_monitor_t em, mem_allocrator_t talloc) {
    struct cfg_dir_read_ctx ctx;
    struct dir_visitor dirVisitor;

    ctx.m_policy = policy;
    ctx.m_curent = cfg;
    ctx.m_em = em;
    ctx.m_node_stack_pos = -1;

    dirVisitor.on_dir_enter = cfg_read_dir_on_enter;
    dirVisitor.on_dir_leave = cfg_read_dir_on_leave;
    dirVisitor.on_file = cfg_read_dir_on_file;

    mem_buffer_init(&ctx.m_tbuffer, talloc);
    dir_search(&dirVisitor, &ctx, path, -1, em, NULL);
    mem_buffer_clear(&ctx.m_tbuffer);
}

int cfg_read_dir(cfg_t cfg, const char * path, cfg_policy_t policy, error_monitor_t em, mem_allocrator_t talloc) {
    int ret = 0;
    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        cfg_read_dir_i(cfg, path, policy, em, talloc);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        cfg_read_dir_i(cfg, path, policy, &logError, talloc);
    }

    return ret;
}
