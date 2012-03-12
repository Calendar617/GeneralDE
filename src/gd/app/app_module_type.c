#include "app_internal_ops.h"

static
gd_app_module_type_list_t g_app_module_types = TAILQ_HEAD_INITIALIZER(g_app_module_types);

static
int gd_app_module_type_load_fun(
    struct gd_app_module_type * module,
    const char * moduleName,
    error_monitor_t em)
{
    size_t nameLen = strlen(moduleName);
    size_t nameBufCapacity = nameLen + 50;
    char * nameBuf;
    int rv = 0;

    nameBuf = (char *)mem_alloc(NULL, nameBufCapacity);
    if (nameBuf == NULL) {
        CPE_ERROR(em, "create module %s: alloc name buf fail!", moduleName);
        return -1;
    }

    memcpy(nameBuf, moduleName, nameLen + 1);

    strcpy(nameBuf + nameLen, "_global_init");
    module->m_global_init = (gd_app_module_global_init)gd_app_lib_sym(module->m_lib, nameBuf, NULL);

    strcpy(nameBuf + nameLen, "_global_fini");
    module->m_global_fini = (gd_app_module_global_fini)gd_app_lib_sym(module->m_lib, nameBuf, NULL);

    strcpy(nameBuf + nameLen, "_app_init");
    module->m_app_init = (gd_app_module_app_init)gd_app_lib_sym(module->m_lib, nameBuf, NULL);

    strcpy(nameBuf + nameLen, "_app_fini");
    module->m_app_fini = (gd_app_module_app_fini)gd_app_lib_sym(module->m_lib, nameBuf, NULL);

    mem_free(NULL, nameBuf);

    if (module->m_global_init && module->m_global_fini == NULL) {
        CPE_ERROR(em, "create module %s: global_init without global_fini!", moduleName);
        rv = -1;
    }

    if (module->m_app_init && module->m_app_fini == NULL) {
        CPE_ERROR(em, "create module %s: app_init without app_fini!", moduleName);
        rv = -1;
    }

    if (module->m_global_init == NULL
        && module->m_global_fini == NULL
        && module->m_app_init == NULL
        && module->m_app_fini == NULL)
    {
        CPE_ERROR(em, "create module %s: no any init function!", moduleName);
        rv = -1;
    }

    return rv;
}

struct gd_app_module_type *
gd_app_module_type_create(
    const char * moduleName,
    const char * libName,
    error_monitor_t em)
{
    size_t nameLen = cpe_hs_len_to_binary_len(strlen(moduleName));
    struct gd_app_module_type * module = NULL;
    char * buf;

    buf = (char *)mem_alloc(NULL, nameLen + sizeof(struct gd_app_module_type));
    if (buf == NULL) {
        CPE_ERROR(em, "create module %s: alloc buf fail!", moduleName);
        return NULL;
    }
    
    cpe_hs_init((cpe_hash_string_t)buf, nameLen, moduleName);
    module = (struct gd_app_module_type *)(buf + nameLen);

    module->m_name = (cpe_hash_string_t)buf;

    TAILQ_INIT(&module->m_runing_modules);

    if (libName == NULL) {
        module->m_lib = NULL;
    }
    else {
        module->m_lib = gd_app_lib_open_for_module(libName, module, em);
        if (module->m_lib == NULL) {
            mem_free(NULL, buf);
            return NULL;
        }
    }

    if (gd_app_module_type_load_fun(module, moduleName, em) != 0) {
        if (module->m_lib) gd_app_lib_close_for_module(module->m_lib, module, em);
        mem_free(NULL, buf);
        return NULL;
    }

    if (module->m_global_init) {
        if (module->m_global_init() != 0) {
            CPE_ERROR(em, "create module %s: global init fail!", moduleName);
            if (module->m_lib) gd_app_lib_close_for_module(module->m_lib, module, em);
            mem_free(NULL, buf);
            return NULL;
        }
    }

    TAILQ_INSERT_TAIL(&g_app_module_types, module, m_next);

    CPE_INFO(em, "create module %s: success!", moduleName);

    return module;
}

void gd_app_module_type_free(struct gd_app_module_type * module, error_monitor_t em) {
    TAILQ_REMOVE(&g_app_module_types, module, m_next);
    if (module->m_lib) gd_app_lib_close_for_module(module->m_lib, module, em);
    CPE_INFO(em, "destory module %s: success!", cpe_hs_data(module->m_name));
    mem_free(NULL, module->m_name);
}

struct gd_app_module_type *
gd_app_module_type_find(const char * moduleName) {
    struct gd_app_module_type * module;

    TAILQ_FOREACH(module, &g_app_module_types, m_next) {
        if (strcmp(cpe_hs_data(module->m_name), moduleName) == 0) {
            return module;
        }
    }

    return NULL;
}

cpe_hash_string_t gd_app_module_type_name_hs(gd_app_module_type_t module) {
    return module->m_name;
}

gd_app_lib_t gd_app_module_type_lib(gd_app_module_type_t module) {
    return module->m_lib;
}

