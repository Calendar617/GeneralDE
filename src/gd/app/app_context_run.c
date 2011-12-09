#include <strings.h>
#include <unistd.h>
#include "gd/app/app_context.h"
#include "app_internal_types.h"

int gd_app_run(gd_app_context_t context) {
    int rv;

    if (context == NULL || context->m_main == NULL) return -1;

    if (context->m_root) {
        if (chdir(context->m_root) != 0) {
            CPE_ERROR(
                gd_app_em(context),
                "change root to %s fail!",
                context->m_root);
        }
        return -1;
    }

    rv = context->m_main(context);

    return -1;
}

