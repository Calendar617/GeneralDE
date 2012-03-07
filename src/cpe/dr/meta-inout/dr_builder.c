#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "cpe/dr/dr_metalib_builder.h"
#include "cpe/dr/dr_metalib_build.h"
#include "dr_builder_types.h"

uint32_t dr_metalib_source_hash(dr_metalib_source_t source) {
    return cpe_hash_str(source->m_name, strlen(source->m_name));
}

int dr_metalib_source_cmp(dr_metalib_source_t l, dr_metalib_source_t r) {
    return strcmp(l->m_name, r->m_name) == 0;
}

dr_metalib_builder_t
dr_metalib_builder_create(mem_allocrator_t alloc) {
    dr_metalib_builder_t builder;
    builder = mem_alloc(alloc, sizeof(struct dr_metalib_builder));
    if (builder == NULL) return NULL;

    builder->m_inbuild_lib = dr_inbuild_create_lib();
    if (builder->m_inbuild_lib == NULL) {
        mem_free(alloc, builder);
        return NULL;
    }

    builder->m_alloc = alloc;

    if (cpe_hash_table_init(
            &builder->m_sources,
            alloc,
            (cpe_hash_fun_t)dr_metalib_source_hash,
            (cpe_hash_cmp_t)dr_metalib_source_cmp,
            CPE_HASH_OBJ2ENTRY(dr_metalib_source, m_hh),
            256) != 0)
    {
        dr_inbuild_free_lib(builder->m_inbuild_lib);
        mem_free(alloc, builder);
        return NULL;
    }

    return builder;
}

void dr_metalib_builder_free(dr_metalib_builder_t builder) {
    struct cpe_hash_it sourceIt;
    dr_metalib_source_t source;

    cpe_hash_it_init(&sourceIt, &builder->m_sources);
    source = cpe_hash_it_next(&sourceIt);
    while(source) {
        dr_metalib_source_t next = cpe_hash_it_next(&sourceIt);
        dr_metalib_source_free(source);
        source = next;
    }

    dr_inbuild_free_lib(builder->m_inbuild_lib);
    mem_free(builder->m_alloc, builder);
}

dr_metalib_source_t
dr_metalib_source_create(dr_metalib_builder_t builder, const char * name, size_t capacity) {
    char * buf;
    dr_metalib_source_t source;
    size_t name_len;

    assert(builder);

    name_len = strlen(name) + 1;

    buf = mem_alloc(builder->m_alloc, sizeof(struct dr_metalib_source) + name_len);
    if (buf == NULL) return NULL;

    memcpy(buf, name, name_len);

    source = (dr_metalib_source_t)(buf + name_len);
    source->m_builder = builder;
    source->m_name = buf;

    TAILQ_INIT(&source->m_includes);
    TAILQ_INIT(&source->m_include_by);

    cpe_hash_entry_init(&source->m_hh);
    if (cpe_hash_table_insert_unique(&builder->m_sources, builder) != 0) {
        mem_free(builder->m_alloc, buf);
        return NULL;
    }

    return source;
}

dr_metalib_source_t
dr_metalib_builder_add_file(dr_metalib_builder_t builder, const char * file) {
    dr_metalib_source_t source;

    assert(builder);
    assert(file);

    source = dr_metalib_source_create(builder, file, strlen(file) + 1);

    return source;
}

void dr_metalib_source_free(dr_metalib_source_t source) {
    assert(source);

    cpe_hash_table_remove_by_ins(&source->m_builder->m_sources, source);

    mem_free(source->m_builder->m_alloc, source);
}

dr_metalib_source_t
dr_metalib_source_find(dr_metalib_builder_t builder, const char * name) {
    struct dr_metalib_source key;
    key.m_name = name;

    return (dr_metalib_source_t)cpe_hash_table_find(&builder->m_sources, &key);
}

int dr_metalib_source_add_include(dr_metalib_source_t user, dr_metalib_source_t using) {
    assert(user);
    assert(using);
    return -1;
}

