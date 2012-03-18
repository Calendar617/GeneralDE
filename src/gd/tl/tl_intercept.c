#include "cpe/pal/pal_string.h"
#include "cpe/utils/memory.h"
#include "gd/tl/tl_intercept.h"
#include "tl_internal_ops.h"

gd_tl_intercept_t
gd_tl_intercept_create(
    gd_tl_t tl,
    const char * name,
    gd_tl_intercept_fun_t intercept_fun,
    void * intercept_ctx)
{
    gd_tl_intercept_t intercept;
    char * buf;
    size_t name_len;

    name_len = strlen(name);

    buf = mem_alloc(tl->m_manage->m_alloc, sizeof(struct gd_tl_intercept) + name_len);
    if (buf == NULL) return NULL;

    memcpy(buf, name, name_len);

    intercept = (gd_tl_intercept_t)(buf + name_len);

    intercept->m_tl = tl;
    intercept->m_name = buf;
    intercept->m_intercept_fun = intercept_fun;
    intercept->m_intercept_ctx = intercept_ctx;

    return intercept;
}

void gd_tl_intercept_free(gd_tl_intercept_t intercept) {
    TAILQ_REMOVE(&intercept->m_tl->m_intercepts, intercept, m_next);
    mem_free(intercept->m_tl->m_manage->m_alloc, (void*)intercept->m_name);
}

gd_tl_intercept_t
gd_tl_intercept_find(gd_tl_t tl, const char * name) {
    gd_tl_intercept_t intercept;

    TAILQ_FOREACH(intercept, &tl->m_intercepts, m_next) {
        if (strcmp(intercept->m_name, name) == 0) {
            return intercept;
        }
    }

    return NULL;
}
