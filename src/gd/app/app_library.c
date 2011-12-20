#include <dlfcn.h>
#include <assert.h>
#include <string.h>
#include "app_internal_ops.h"

struct gd_app_lib {
    char * m_name;
    void * m_handler;
    gd_app_module_list_t m_modules;
    TAILQ_ENTRY(gd_app_lib) m_next;
};

static
gd_app_lib_list_t g_app_libs = TAILQ_HEAD_INITIALIZER(g_app_libs);

static
struct gd_app_lib *
gd_app_lib_create(const char * libName, error_monitor_t em) {
    size_t nameLen = strlen(libName);
    struct gd_app_lib * lib = NULL;
    char * buf;

    buf = mem_alloc(NULL, nameLen + 1 + sizeof(struct gd_app_lib));
    if (buf == NULL) {
        CPE_ERROR(em, "create lib %s: alloc buf fail!", libName);
        return NULL;
    }
    
    memcpy(buf, libName, nameLen + 1);
    lib = (struct gd_app_lib *)(buf + nameLen + 1);

    lib->m_name = buf;
    lib->m_handler = NULL;
    TAILQ_INIT(&lib->m_modules);

    lib->m_handler = dlopen(libName, RTLD_NOW | RTLD_LOCAL);
    if (lib->m_handler == NULL) {
        CPE_ERROR(em, "create lib %s: %s!", libName, dlerror());
        mem_free(NULL, buf);
        return NULL;
    }

    TAILQ_INSERT_TAIL(&g_app_libs, lib, m_next);

    return lib;
}

static
void gd_app_lib_free(struct gd_app_lib * lib) {
    if (lib->m_handler) {
        dlclose(lib->m_handler);
        lib->m_handler = NULL;
    }

    TAILQ_REMOVE(&g_app_libs, lib, m_next);

    mem_free(NULL, lib->m_name);
}

static
struct gd_app_lib *
gd_app_lib_find(const char * libName) {
    struct gd_app_lib * lib;

    TAILQ_FOREACH(lib, &g_app_libs, m_next) {
        if (strcmp(lib->m_name, libName) == 0) {
            return lib;
        }
    }

    return NULL;
}

struct gd_app_lib *
gd_app_lib_open_for_module(
    const char * libName,
    struct gd_app_module * module,
    error_monitor_t em)
{
    struct gd_app_lib * lib;

    assert(module);
    assert(libName);

    lib = gd_app_lib_find(libName);

    if (lib == NULL) {
        lib = gd_app_lib_create(libName, em);
        if (lib == NULL) return NULL;
    }

    TAILQ_INSERT_TAIL(&lib->m_modules, module, m_qh_for_lib);

    return lib;
}

void gd_app_lib_close_for_module(
    struct gd_app_lib * lib,
    struct gd_app_module * module,
    error_monitor_t em)
{
    assert(lib);
    assert(module);

    if (lib == NULL) return;

    TAILQ_INSERT_TAIL(&lib->m_modules, module, m_qh_for_lib);

    if (TAILQ_EMPTY(&lib->m_modules)) {
        gd_app_lib_free(lib);
    }
}

void * gd_app_lib_sym(struct gd_app_lib * lib, const char * symName) {
    return lib ? dlsym(lib->m_handler, symName) : dlsym(NULL, symName);
}
