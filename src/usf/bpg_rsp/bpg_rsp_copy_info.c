#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "bpg_rsp_internal_ops.h"

struct bpg_rsp_copy_info *
bpg_rsp_copy_info_create(bpg_rsp_manage_t mgr, const char * data_name) {
    size_t name_len;
    struct bpg_rsp_copy_info * copy_info;

    name_len = strlen(data_name) + 1;

    copy_info = (struct bpg_rsp_copy_info *)mem_alloc(mgr->m_alloc, sizeof(struct bpg_rsp_copy_info) + name_len);

    memcpy(copy_info + 1, data_name, name_len);

    return copy_info;
}

void bpg_rsp_copy_info_free(bpg_rsp_manage_t mgr, struct bpg_rsp_copy_info * copy_info) {
    mem_free(mgr->m_alloc, copy_info);
}

const char * bpg_rsp_copy_info_data(struct bpg_rsp_copy_info * copy_info) {
    return (const char *)(copy_info + 1);
}
