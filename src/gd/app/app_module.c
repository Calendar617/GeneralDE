#include <assert.h>
#include <string.h>
#include "cpe/cfg/cfg_read.h"
#include "gd/app/app_log.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "app_internal_ops.h"

struct gd_app_runing_module {
    mem_allocrator_t m_alloc;
    struct gd_app_module * m_module;

    TAILQ_ENTRY(gd_app_runing_module) m_qh_for_app;
    TAILQ_ENTRY(gd_app_runing_module) m_qh_for_runing;
}; 

static
gd_app_module_list_t g_app_modules = TAILQ_HEAD_INITIALIZER(g_app_modules);

static
int gd_app_module_load_fun(
    struct gd_app_module * module,
    const char * moduleName,
    error_monitor_t em)
{
    size_t nameLen = strlen(moduleName);
    size_t nameBufCapacity = nameLen + 50;
    char * nameBuf;
    int rv = 0;

    nameBuf = mem_alloc(NULL, nameBufCapacity);
    if (nameBuf == NULL) {
        CPE_ERROR(em, "create module %s: alloc name buf fail!", moduleName);
        return -1;
    }

    memcpy(nameBuf, moduleName, nameLen + 1);

    strcpy(nameBuf + nameLen, "_global_init");
    module->m_global_init = gd_app_lib_sym(module->m_lib, nameBuf);

    strcpy(nameBuf + nameLen, "_global_fini");
    module->m_global_fini = gd_app_lib_sym(module->m_lib, nameBuf);

    strcpy(nameBuf + nameLen, "_app_init");
    module->m_app_init = gd_app_lib_sym(module->m_lib, nameBuf);

    strcpy(nameBuf + nameLen, "_app_fini");
    module->m_app_fini = gd_app_lib_sym(module->m_lib, nameBuf);

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

static
struct gd_app_module *
gd_app_module_create(
    const char * moduleName,
    const char * libName,
    error_monitor_t em)
{
    size_t nameLen = cpe_hs_len_to_binary_len(strlen(moduleName));
    struct gd_app_module * module = NULL;
    char * buf;

    buf = mem_alloc(NULL, nameLen + sizeof(struct gd_app_module));
    if (buf == NULL) {
        CPE_ERROR(em, "create module %s: alloc buf fail!", moduleName);
        return NULL;
    }
    
    cpe_hs_init((cpe_hash_string_t)buf, nameLen, moduleName);
    module = (struct gd_app_module *)(buf + nameLen);

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

    if (gd_app_module_load_fun(module, moduleName, em) != 0) {
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

    TAILQ_INSERT_TAIL(&g_app_modules, module, m_next);

    CPE_INFO(em, "create module %s: success!", moduleName);

    return module;
}

static
void gd_app_module_free(struct gd_app_module * module, error_monitor_t em) {
    TAILQ_REMOVE(&g_app_modules, module, m_next);
    if (module->m_lib) gd_app_lib_close_for_module(module->m_lib, module, em);
    CPE_INFO(em, "destory module %s: success!", cpe_hs_data(module->m_name));
    mem_free(NULL, module->m_name);
}

struct gd_app_module *
gd_app_module_find(const char * moduleName) {
    struct gd_app_module * module;

    TAILQ_FOREACH(module, &g_app_modules, m_next) {
        if (strcmp(cpe_hs_data(module->m_name), moduleName) == 0) {
            return module;
        }
    }

    return NULL;
}

static int gd_app_runing_module_create(gd_app_context_t context, cfg_t cfg) {
    struct gd_app_module * module;
    const char * moduleName;
    struct gd_app_runing_module * runing_module;
    gd_nm_node_t moduleDataGroup;

    assert(context);

    moduleName = cfg_get_string(cfg, "name", NULL);
    if (moduleName == NULL) {
        APP_CTX_ERROR(context, "load module: no name!");
        return -1;
    }

    module = gd_app_module_find(moduleName);
    if (module == NULL) {
        module = gd_app_module_create(moduleName, cfg_get_string(cfg, "library", NULL), context->m_em);
        if (module == NULL) return -1;
    }

    runing_module = (struct gd_app_runing_module *)
        mem_alloc(context->m_alloc, sizeof(struct gd_app_runing_module));
    if (runing_module == NULL) {
        APP_CTX_ERROR(context, "load module: alloc runing module fail!");
        if (TAILQ_EMPTY(&module->m_runing_modules)) gd_app_module_free(module, context->m_em);
        return -1;
    }

    moduleDataGroup = gd_app_runing_module_data_load(
        context,
        runing_module->m_module->m_name,
        cfg);
    if (moduleDataGroup == NULL) {
        if (TAILQ_EMPTY(&module->m_runing_modules)) gd_app_module_free(module, context->m_em);
        mem_free(context->m_alloc, runing_module);
        return -1;
    }

    if (module->m_app_init) {
        if (module->m_app_init(context, module, cfg) != 0) {
            APP_CTX_ERROR(context, "load module %s: app init fail!", moduleName);
            gd_app_runing_module_data_free(context, module->m_name);
            if (TAILQ_EMPTY(&module->m_runing_modules)) gd_app_module_free(module, context->m_em);
            mem_free(context->m_alloc, runing_module);
            return -1;
        }
    }

    runing_module->m_alloc = context->m_alloc;
    runing_module->m_module = module;

    TAILQ_INSERT_TAIL(&context->m_runing_modules, runing_module, m_qh_for_app);
    TAILQ_INSERT_TAIL(&module->m_runing_modules, runing_module, m_qh_for_runing);
    
    return 0;
}

static void gd_app_runing_module_free(
    struct gd_app_runing_module * runing_module,
    gd_app_context_t context)
{
    struct gd_app_module * module;

    assert(runing_module);

    module = runing_module->m_module;

    if (module->m_app_fini) {
        module->m_app_fini(context, module);
    }

    TAILQ_REMOVE(&context->m_runing_modules, runing_module, m_qh_for_app);
    TAILQ_REMOVE(&module->m_runing_modules, runing_module, m_qh_for_runing);

    gd_app_runing_module_data_free(context, module->m_name);

    mem_free(runing_module->m_alloc, runing_module);

    if (TAILQ_EMPTY(&module->m_runing_modules)) {
        gd_app_module_free(module, context->m_em);
    }
}

int gd_app_modules_load(gd_app_context_t context) {
    int rv;
    cfg_t moduleListCfg;
    cfg_t moduleCfg;
    struct cfg_seq_it cfgIt;

    moduleListCfg = cfg_find_cfg(context->m_cfg, "modules.load");

    if (moduleListCfg == NULL) {
        APP_CTX_INFO(context, "no modules need to load!");
        return -1;
    }

    if (cfg_type(moduleListCfg) != CPE_CFG_TYPE_SEQUENCE) {
        APP_CTX_ERROR(context, "dispatcher config type error!");
        return -1;
    }

    rv = 0;
    cfg_seq_it_init(&cfgIt, moduleListCfg);
    while(rv == 0
          && (moduleCfg = cfg_find_cfg(cfg_seq_it_next(&cfgIt), "module")))
    {
        if (gd_app_runing_module_create(context, moduleCfg) != 0) {
            rv = -1;
        }
    }

    if (rv != 0) {
        gd_app_modules_unload(context);
    }

    return rv;
}

void gd_app_modules_unload(gd_app_context_t context) {
    while (!TAILQ_EMPTY(&context->m_runing_modules)) {
        gd_app_runing_module_free(
            TAILQ_LAST(&context->m_runing_modules, gd_app_runing_module_list),
            context);
    }
}

const char * gd_app_module_name(gd_app_module_t module) {
    return cpe_hs_data(module->m_name);
}

cpe_hash_string_t gd_app_module_name_hs(gd_app_module_t module) {
    return module->m_name;
}
