#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "cpe/cfg/cfg_read.h"
#include "gd/app/app_log.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "app_internal_ops.h"

struct gd_app_module {
    mem_allocrator_t m_alloc;
    struct gd_app_module_type * m_type;

    TAILQ_ENTRY(gd_app_module) m_qh_for_app;
    TAILQ_ENTRY(gd_app_module) m_qh_for_runing;
}; 


static
struct gd_app_module * 
gd_app_module_create_i(
    gd_app_context_t context,
    const char * moduleName,
    const char * type_name,
    const char * libName,
    cfg_t moduleCfg)
{
    struct gd_app_module_type * module;
    struct gd_app_module * runing_module;
    gd_nm_node_t moduleDataGroup;
    size_t name_len;

    if (type_name == NULL) type_name = moduleName;

    module = gd_app_module_type_find(type_name);
    if (module == NULL) {
        module = gd_app_module_type_create(type_name, libName, context->m_em);
        if (module == NULL) return NULL;
    }

    name_len = strlen(moduleName) + 1;

    runing_module = (struct gd_app_module *)
        mem_alloc(context->m_alloc, sizeof(struct gd_app_module) + name_len);
    if (runing_module == NULL) {
        APP_CTX_ERROR(context, "load module: alloc runing module fail!");
        if (TAILQ_EMPTY(&module->m_runing_modules)) gd_app_module_type_free(module, context->m_em);
        return NULL;
    }

    memcpy(runing_module + 1, moduleName, name_len);

    runing_module->m_alloc = context->m_alloc;
    runing_module->m_type = module;

    moduleDataGroup = gd_app_module_data_load(context, moduleName);
    if (moduleDataGroup == NULL) {
        if (TAILQ_EMPTY(&module->m_runing_modules)) gd_app_module_type_free(module, context->m_em);
        mem_free(context->m_alloc, runing_module);
        return NULL;
    }

    if (module->m_app_init) {
        if (module->m_app_init(context, runing_module, moduleCfg) != 0) {
            APP_CTX_ERROR(context, "load module %s: app init fail!", moduleName);
            gd_app_module_data_free(context, moduleName);
            if (TAILQ_EMPTY(&module->m_runing_modules)) gd_app_module_type_free(module, context->m_em);
            mem_free(context->m_alloc, runing_module);
            return NULL;
        }
    }

    TAILQ_INSERT_TAIL(&context->m_runing_modules, runing_module, m_qh_for_app);
    TAILQ_INSERT_TAIL(&module->m_runing_modules, runing_module, m_qh_for_runing);
    
    return runing_module;
}

static int gd_app_module_create(gd_app_context_t context, cfg_t cfg) {
    const char * moduleName;
    cfg_t moduleInitCfg;
    assert(context);

    moduleName = cfg_get_string(cfg, "name", NULL);
    if (moduleName == NULL) {
        APP_CTX_ERROR(context, "load module: no name!");
        return -1;
    }

    moduleInitCfg = 
        cfg_find_cfg(
            cfg_find_cfg(gd_app_cfg(context), "modules"),
            moduleName);
    if (moduleInitCfg == NULL) moduleInitCfg = cfg;

    return gd_app_module_create_i(
        context,
        moduleName,
        cfg_get_string(cfg, "type", NULL),
        cfg_get_string(cfg, "library", NULL),
        moduleInitCfg) == NULL
        ? -1
        : 0;
}

static void gd_app_module_free(
    struct gd_app_module * module,
    gd_app_context_t context)
{
    struct gd_app_module_type * type;

    assert(module);

    type = module->m_type;

    if (type->m_app_fini) {
        type->m_app_fini(context, module);
    }

    TAILQ_REMOVE(&context->m_runing_modules, module, m_qh_for_app);
    TAILQ_REMOVE(&type->m_runing_modules, module, m_qh_for_runing);

    gd_app_module_data_free(context, gd_app_module_name(module));

    mem_free(module->m_alloc, module);

    if (TAILQ_EMPTY(&type->m_runing_modules)) {
        gd_app_module_type_free(type, context->m_em);
    }
}

int gd_app_modules_load(gd_app_context_t context) {
    int rv;
    cfg_t moduleListCfg;
    cfg_t moduleCfg;
    struct cfg_it cfgIt;

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
    cfg_it_init(&cfgIt, moduleListCfg);
    while(rv == 0 && (moduleCfg = cfg_it_next(&cfgIt))) {
        if (gd_app_module_create(context, moduleCfg) != 0) {
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
        gd_app_module_free(
            TAILQ_LAST(&context->m_runing_modules, gd_app_module_list),
            context);
    }
}

const char * gd_app_module_name(gd_app_module_t module) {
    return (const char *)(module + 1);
}

const char * gd_app_module_type_name(gd_app_module_t module) {
    return cpe_hs_data(module->m_type->m_name);
}

gd_app_lib_t gd_app_module_lib(gd_app_module_t module) {
    return module->m_type->m_lib;
}

gd_app_module_t
gd_app_install_module(
    gd_app_context_t context,
    const char * name,
    const char * type_name,
    const char * libName,
    cfg_t moduleCfg)
{
    gd_app_module_t module;

    module = 
        gd_app_module_create_i(context, name, type_name, libName, moduleCfg);

    return module;
}

gd_app_module_t
gd_app_find_module(
    gd_app_context_t context,
    const char * name)
{
    gd_app_module_t runint_module;

    TAILQ_FOREACH(runint_module, &context->m_runing_modules, m_qh_for_app) {
        if (strcmp(name, gd_app_module_name(runint_module)) == 0) {
            return runint_module;
        }
    }

    return NULL;
}

int gd_app_uninstall_module(
    gd_app_context_t context,
    const char * name)
{
    struct gd_app_module * runint_module;

    TAILQ_FOREACH(runint_module, &context->m_runing_modules, m_qh_for_app) {
        if (strcmp(name, gd_app_module_name(runint_module)) == 0) {
            gd_app_module_free(runint_module, context);
            return 0;
        }
    }

    return -1;
}
