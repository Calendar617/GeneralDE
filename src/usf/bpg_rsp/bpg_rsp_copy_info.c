#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "bpg_rsp_internal_ops.h"

struct bpg_rsp_copy_info *
bpg_rsp_copy_info_create(
    bpg_rsp_manage_t mgr, 
    bpg_rsp_copy_info_list_t copy_info_list,
    const char * data_name)
{
    size_t name_len;
    struct bpg_rsp_copy_info * copy_info;

    name_len = strlen(data_name) + 1;

    copy_info = (struct bpg_rsp_copy_info *)mem_alloc(mgr->m_alloc, sizeof(struct bpg_rsp_copy_info) + name_len);

    memcpy(copy_info + 1, data_name, name_len);

    TAILQ_INSERT_TAIL(copy_info_list, copy_info, m_next);

    return copy_info;
}

void bpg_rsp_copy_info_free(
    bpg_rsp_manage_t mgr, 
    bpg_rsp_copy_info_list_t copy_info_list,
    struct bpg_rsp_copy_info * copy_info)
{
    TAILQ_REMOVE(copy_info_list, copy_info, m_next);

    mem_free(mgr->m_alloc, copy_info);
}

const char * bpg_rsp_copy_info_data(struct bpg_rsp_copy_info * copy_info) {
    return (const char *)(copy_info + 1);
}

void bpg_rsp_copy_info_clear(
    bpg_rsp_manage_t mgr, 
    bpg_rsp_copy_info_list_t copy_info_list)
{
    while(!TAILQ_EMPTY(copy_info_list)) {
        bpg_rsp_copy_info_free(mgr, copy_info_list, TAILQ_FIRST(copy_info_list));
    }
}

