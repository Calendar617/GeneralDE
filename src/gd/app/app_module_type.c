#include "app_internal_ops.h"

static
gd_app_module_type_list_t g_app_module_types = TAILQ_HEAD_INITIALIZER(g_app_module_types);

static
int gd_app_module_type_load_fun(
    struct gd_app_module_type * module_type,
    const char * type_name,
    error_monitor_t em)
{
    size_t nameLen = strlen(type_name);
    size_t nameBufCapacity = nameLen + 50;
    char * nameBuf;
    int rv = 0;

    nameBuf = (char *)mem_alloc(NULL, nameBufCapacity);
    if (nameBuf == NULL) {
        CPE_ERROR(em, "%s: module-type alloc name buf fail!", type_name);
        return -1;
    }

    memcpy(nameBuf, type_name, nameLen + 1);

    strcpy(nameBuf + nameLen, "_global_init");
    module_type->m_global_init = (gd_app_module_global_init)gd_app_lib_sym(module_type->m_lib, nameBuf, NULL);

    strcpy(nameBuf + nameLen, "_global_fini");
    module_type->m_global_fini = (gd_app_module_global_fini)gd_app_lib_sym(module_type->m_lib, nameBuf, NULL);

    strcpy(nameBuf + nameLen, "_app_init");
    module_type->m_app_init = (gd_app_module_app_init)gd_app_lib_sym(module_type->m_lib, nameBuf, NULL);

    strcpy(nameBuf + nameLen, "_app_fini");
    module_type->m_app_fini = (gd_app_module_app_fini)gd_app_lib_sym(module_type->m_lib, nameBuf, NULL);

    mem_free(NULL, nameBuf);

    if (module_type->m_global_init && module_type->m_global_fini == NULL) {
        CPE_ERROR(em, "%s: module-type global_init without global_fini!", type_name);
        rv = -1;
    }

    if (module_type->m_app_init && module_type->m_app_fini == NULL) {
        CPE_ERROR(em, "%s: module-type app_init without app_fini!", type_name);
        rv = -1;
    }

    if (module_type->m_global_init == NULL
        && module_type->m_global_fini == NULL
        && module_type->m_app_init == NULL
        && module_type->m_app_fini == NULL)
    {
        CPE_ERROR(em, "module_type-type %s: no any init function!", type_name);
        rv = -1;
    }

    return rv;
}

struct gd_app_module_type *
gd_app_module_type_create(
    const char * type_name,
    const char * libName,
    error_monitor_t em)
{
    size_t nameLen = cpe_hs_len_to_binary_len(strlen(type_name));
    struct gd_app_module_type * module_type = NULL;
    char * buf;

    buf = (char *)mem_alloc(NULL, nameLen + sizeof(struct gd_app_module_type));
    if (buf == NULL) {
        CPE_ERROR(em, "%s: module-type alloc buf fail!", type_name);
        return NULL;
    }
    
    cpe_hs_init((cpe_hash_string_t)buf, nameLen, type_name);
    module_type = (struct gd_app_module_type *)(buf + nameLen);

    module_type->m_name = (cpe_hash_string_t)buf;

    TAILQ_INIT(&module_type->m_runing_modules);

    if (libName == NULL) {
        module_type->m_lib = NULL;
    }
    else {
        module_type->m_lib = gd_app_lib_open_for_module(libName, module_type, em);
        if (module_type->m_lib == NULL) {
            mem_free(NULL, buf);
            return NULL;
        }
    }

    if (gd_app_module_type_load_fun(module_type, type_name, em) != 0) {
        if (module_type->m_lib) gd_app_lib_close_for_module(module_type->m_lib, module_type, em);
        mem_free(NULL, buf);
        return NULL;
    }

    if (module_type->m_global_init) {
        if (module_type->m_global_init() != 0) {
            CPE_ERROR(em, "%s: module-type global init fail!", type_name);
            if (module_type->m_lib) gd_app_lib_close_for_module(module_type->m_lib, module_type, em);
            mem_free(NULL, buf);
            return NULL;
        }
    }

    TAILQ_INSERT_TAIL(&g_app_module_types, module_type, m_next);

    return module_type;
}

void gd_app_module_type_free(struct gd_app_module_type * module_type, error_monitor_t em) {
    TAILQ_REMOVE(&g_app_module_types, module_type, m_next);
    if (module_type->m_lib) gd_app_lib_close_for_module(module_type->m_lib, module_type, em);
    mem_free(NULL, module_type->m_name);
}

struct gd_app_module_type *
gd_app_module_type_find(const char * type_name) {
    struct gd_app_module_type * module_type;

    TAILQ_FOREACH(module_type, &g_app_module_types, m_next) {
        if (strcmp(cpe_hs_data(module_type->m_name), type_name) == 0) {
            return module_type;
        }
    }

    return NULL;
}

cpe_hash_string_t gd_app_module_type_name_hs(gd_app_module_type_t module_type) {
    return module_type->m_name;
}

gd_app_lib_t gd_app_module_type_lib(gd_app_module_type_t module_type) {
    return module_type->m_lib;
}

