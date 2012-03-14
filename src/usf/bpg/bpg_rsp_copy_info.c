#include <assert.h>
#include "bpg_internal_ops.h"

struct bpg_rsp_copy_info *
bpg_rsp_copy_info_create(bpg_manage_t mgr, const char * data_name) {
    size_t name_len;
    struct bpg_rsp_copy_info * copy_info;

    name_len = strlen(data_name) + 1;

    copy_info = (struct bpg_rsp_copy_info *)mem_alloc(mgr->m_alloc, sizeof(struct bpg_rsp_copy_info) + name_len);

    memcpy(copy_info + 1, data_name, name_len);

    return copy_info;
}

void bpg_rsp_copy_info_free(bpg_manage_t mgr, struct bpg_rsp_copy_info * copy_info) {
        mem_free(mgr->m_alloc, copy_info);
}

int bpg_rsp_copy_ctx_to_pdu(bpg_rsp_t rsp, logic_context_t op_context, void * pkg) {
    return 0;
}
