#include <assert.h>
#include "cpe/pal/pal_stdlib.h"
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/utils/error.h"
#include "cpe/utils/range.h"
#include "cpe/utils/file.h"
#include "cpe/nm/nm_manage.h"
#include "cpe/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/utils/id_generator.h"

struct usf_id_generator {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;
    uint16_t m_global_id;
    const char * m_save_file;
    uint32_t m_one_alloc_size;
    struct cpe_range m_range;
    int m_debug;
};

static void usf_id_generator_clear(nm_node_t node);
static int usf_id_generator_write_back(usf_id_generator_t id_generator, ptr_int_t new_begin);

struct nm_node_type s_nm_node_type_usf_id_generator = {
    "usf_id_generator",
    usf_id_generator_clear
};

usf_id_generator_t
usf_id_generator_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc,
    error_monitor_t em)
{
    usf_id_generator_t generator;
    nm_node_t generator_node;

    assert(app);

    generator_node = nm_instance_create(gd_app_nm_mgr(app), name, sizeof(struct usf_id_generator));
    if (generator_node == NULL) return NULL;

    generator = (usf_id_generator_t)nm_node_data(generator_node);

    generator->m_app = app;
    generator->m_alloc = alloc ? alloc : gd_app_alloc(app);
    generator->m_em = em ? em : gd_app_em(app);
    generator->m_save_file = NULL;
    generator->m_one_alloc_size = 2046;
    generator->m_debug = 0;
    generator->m_global_id = 0;
    generator->m_range.m_start = 1;
    generator->m_range.m_end = 1;

    nm_node_set_type(generator_node, &s_nm_node_type_usf_id_generator);

    return generator;
}

static void usf_id_generator_clear(nm_node_t node) {
    usf_id_generator_t generator;
    generator = (usf_id_generator_t)nm_node_data(node);

    if (generator->m_save_file) {
        mem_free(generator->m_alloc, (void*)generator->m_save_file);
        generator->m_save_file = NULL;
    }
}

void usf_id_generator_free(usf_id_generator_t generator) {
    nm_node_t generator_node;
    assert(generator);

    generator_node = nm_node_from_data(generator);
    if (nm_node_type(generator_node) != &s_nm_node_type_usf_id_generator) return;
    nm_node_free(generator_node);
}

usf_id_generator_t
usf_id_generator_find(gd_app_context_t app, cpe_hash_string_t name) {
    nm_node_t node;

    if (name == NULL) return NULL;

    node = nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_usf_id_generator) return NULL;
    return (usf_id_generator_t)nm_node_data(node);
}

usf_id_generator_t
usf_id_generator_find_nc(gd_app_context_t app, const char * name) {
    nm_node_t node;

    if (name == NULL) return NULL;

    node = nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_usf_id_generator) return NULL;
    return (usf_id_generator_t)nm_node_data(node);
}

gd_app_context_t usf_id_generator_app(usf_id_generator_t generator) {
    return generator->m_app;
}

const char * usf_id_generator_name(usf_id_generator_t generator) {
    return nm_node_name(nm_node_from_data(generator));
}

cpe_hash_string_t
usf_id_generator_name_hs(usf_id_generator_t generator) {
    return nm_node_name_hs(nm_node_from_data(generator));
}

const char * usf_id_generator_save_file(usf_id_generator_t generator) {
    return generator->m_save_file;
}

int usf_id_generator_set_save_file(usf_id_generator_t generator, const char * file) {
    char * new_save_file;

    new_save_file = NULL;
    if (file) {
        size_t size;
        size = strlen(file) + 1;

        new_save_file = mem_alloc(generator->m_alloc, size);
        if (new_save_file == NULL) return -1;
        memcpy(new_save_file, file, size);
    }

    if (generator->m_save_file) {
        mem_free(generator->m_alloc, (void*)generator->m_save_file);
    }

    generator->m_range.m_start = 1;
    generator->m_range.m_end = 1;
    generator->m_save_file = new_save_file;
    return 0;
}

uint32_t usf_id_generator_once_alloc_size(usf_id_generator_t generator) {
    return generator->m_one_alloc_size;
}

void usf_id_generator_set_once_alloc_size(usf_id_generator_t generator, uint32_t once_alloc_size) {
    generator->m_one_alloc_size = once_alloc_size;
}

int usf_id_generator_set_next_id(usf_id_generator_t generator, usf_id_t id) {
    ptr_int_t new_begin = id & 0xFFFFFF;

    if (generator->m_save_file) {
        if (usf_id_generator_write_back(generator, new_begin + generator->m_one_alloc_size) != 0) {
            return -1;
        }
    }

    generator->m_range.m_start = new_begin;
    generator->m_range.m_end = new_begin + generator->m_one_alloc_size;
    return 0;
}

static int usf_id_generator_read_range_start(ptr_int_t * result, usf_id_generator_t generator) {
    struct mem_buffer buffer;
    ssize_t size;
    char * read_end_pos;

    if (generator->m_save_file == NULL) return -1;

    if (!file_exist(generator->m_save_file, generator->m_em)) {
        *result = 1;
        return 0;
    }

    mem_buffer_init(&buffer, NULL);

    size = file_load_to_buffer(&buffer, generator->m_save_file, generator->m_em);
    if (size < 0) {
        CPE_ERROR(
            generator->m_em,
            "usf_id_generator %s: open %s to read id range fail!", 
            usf_id_generator_name(generator), generator->m_save_file);
        mem_buffer_clear(&buffer);
        return -1;
    } 

    mem_buffer_append_char(&buffer, 0);

    read_end_pos = 0;
    *result = strtol((const char *)mem_buffer_make_continuous(&buffer, 0), &read_end_pos, 10);
    if (read_end_pos == 0 || *read_end_pos != 0) {
        CPE_ERROR(
            generator->m_em,
            "usf_id_generator %s: read next id from %s fail, input is '%s'!", 
            usf_id_generator_name(generator), generator->m_save_file,
            (const char *)mem_buffer_make_continuous(&buffer, 0));
        mem_buffer_clear(&buffer);
        return -1;
    }

    mem_buffer_clear(&buffer);

    if (generator->m_debug) {
        CPE_INFO(
            generator->m_em, "usf_id_generator %s: load new range success: start at %d",
            usf_id_generator_name(generator), (int)*result);
    }

    return 0;
}

static int usf_id_generator_write_back(usf_id_generator_t id_generator, ptr_int_t new_begin) {
    char buf[20];

    snprintf(buf, sizeof(buf), "%d", (int)new_begin);

    if (file_write_from_str(id_generator->m_save_file, buf, id_generator->m_em) <= 0) {
        CPE_ERROR(
            id_generator->m_em, "usf_id_generator %s: write back to %s fail!", 
            usf_id_generator_name(id_generator), id_generator->m_save_file);
        return -1;
    }

    if (id_generator->m_debug) {
        CPE_INFO(
            id_generator->m_em, "usf_id_generator %s: save new range success: value is %d",
            usf_id_generator_name(id_generator), (int)new_begin);
    }

    return 0;
}

static int usf_id_generator_load_next_range(usf_id_generator_t generator) {
    ptr_int_t new_begin;

    if (generator->m_save_file == 0) {
        generator->m_range.m_start = generator->m_range.m_end;
        generator->m_range.m_end += generator->m_one_alloc_size;
        return 0;
    }

    if (usf_id_generator_read_range_start(&new_begin, generator) != 0) return -1;

    if(new_begin < 0) {
        CPE_ERROR(
            generator->m_em, "usf_id_generator %s: id overflow!", 
            usf_id_generator_name(generator));
        return -1;
    }

    if (usf_id_generator_write_back(generator, new_begin + generator->m_one_alloc_size) != 0) return -1;

    generator->m_range.m_start = new_begin;
    generator->m_range.m_end = new_begin + generator->m_one_alloc_size;

    return 0;
}

int usf_id_generator_generate(usf_id_t * r, usf_id_generator_t generator) {
    assert(r);
    assert(generator);

    if (cpe_range_size(generator->m_range) <= 0) {
        if (usf_id_generator_load_next_range(generator) != 0) {
            return -1;
        }
    }

    *r =
        ((usf_id_t)generator->m_global_id << 48)
        | (((usf_id_t)generator->m_range.m_start) & 0xFFFFFFFFFFFFLL);

    ++generator->m_range.m_start;

    return 0;
}

EXPORT_DIRECTIVE
int usf_id_generator_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    usf_id_generator_t usf_id_generator;

    usf_id_generator = usf_id_generator_create(app, gd_app_module_name(module), NULL, NULL);
    if (usf_id_generator == NULL) return -1;

    if (usf_id_generator_set_save_file(usf_id_generator, cfg_get_string(cfg, "save-file", NULL)) != 0) {
        CPE_ERROR(gd_app_em(app), "%s: set save-file fail!", gd_app_module_name(module));
        usf_id_generator_free(usf_id_generator);
        return -1;
    }

    usf_id_generator_set_once_alloc_size(
        usf_id_generator,
        cfg_get_uint32(cfg, "once-alloc-size", usf_id_generator->m_one_alloc_size));

    usf_id_generator->m_debug = cfg_get_uint32(cfg, "debug", 0);

    if (usf_id_generator->m_debug) {
        CPE_INFO(
            gd_app_em(app), "%s: save-file=%s, once-alloc-size=%d!",
            gd_app_module_name(module), usf_id_generator->m_save_file, usf_id_generator->m_one_alloc_size);
    }

    return 0;
}

EXPORT_DIRECTIVE
void usf_id_generator_app_fini(gd_app_context_t app, gd_app_module_t module) {
    usf_id_generator_t usf_id_generator;

    usf_id_generator = usf_id_generator_find_nc(app, gd_app_module_name(module));
    if (usf_id_generator) {
        usf_id_generator_free(usf_id_generator);
    }
}

