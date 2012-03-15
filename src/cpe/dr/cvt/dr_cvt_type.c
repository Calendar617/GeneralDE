#include "cpe/pal/pal_string.h"
#include "cpe/utils/memory.h"
#include "dr_cvt_internal_ops.h"

dr_cvt_type_list_t s_cvt_types = TAILQ_HEAD_INITIALIZER(s_cvt_types);

static void dr_cvt_types_init(void);

struct dr_cvt_type *
dr_cvt_type_find(const char * name) {
    struct dr_cvt_type * type;

    if (TAILQ_EMPTY(&s_cvt_types)) dr_cvt_types_init();

    TAILQ_FOREACH(type, &s_cvt_types, m_next) {
        if (strcmp(type->m_name, name) == 0) return type;
    }

    return NULL;
}

int dr_cvt_type_create(
    const char * name,
    dr_cvt_fun_t encode,
    dr_cvt_fun_t decode,
    void * ctx)
{
    struct dr_cvt_type * new_type;

    if (dr_cvt_type_find(name)) return -1;

    new_type = mem_alloc(NULL, sizeof(struct dr_cvt_type));
    if (new_type == NULL) return -1;

    new_type->m_name = name;
    new_type->m_encode = encode;
    new_type->m_decode = decode;
    new_type->m_ctx = ctx;
    new_type->m_ref_count = 1;

    TAILQ_INSERT_TAIL(&s_cvt_types, new_type, m_next);

    return 0;
}

void dr_cvt_type_add_ref(struct dr_cvt_type * type) {
    if (type->m_ref_count >= 0) {
        ++type->m_ref_count;
    }
}

void dr_cvt_type_remove_ref(struct dr_cvt_type * type) {
    if (type->m_ref_count < 0) return;

    --type->m_ref_count;
    if (type->m_ref_count > 0) return;

    TAILQ_REMOVE(&s_cvt_types, type, m_next);
    mem_free(NULL, type);
}

void dr_cvt_type_free(const char * name) {
    struct dr_cvt_type * type;

    type = dr_cvt_type_find(name);
    if (type == NULL) return;

    type->m_encode = NULL;
    type->m_decode = NULL;
    type->m_ctx = NULL;

    dr_cvt_type_remove_ref(type);
}

ssize_t dr_cvt_fun_cpoy_encode(
    LPDRMETA meta,
    void * output, size_t output_capacity,
    const void * input, size_t input_capacity,
    void * ctx,
    error_monitor_t em)
{
    if (output_capacity < input_capacity) {
        CPE_ERROR(
            em, "encode %s: not enought output buf, require %d, but only %d!",
            dr_meta_name(meta), (int)input_capacity, (int)output_capacity);
        return -1;
    }

    memcpy(output, input, input_capacity);
    return input_capacity;
}

extern struct dr_cvt_type s_cvt_type_copy;

void dr_cvt_types_init(void) {
    TAILQ_INSERT_TAIL(&s_cvt_types, &s_cvt_type_copy, m_next);
}

