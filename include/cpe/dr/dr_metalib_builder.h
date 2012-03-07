#ifndef CPE_DR_METALIB_BUILDER_H
#define CPE_DR_METALIB_BUILDER_H
#include "cpe/utils/memory.h"
#include "dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

dr_metalib_builder_t dr_metalib_builder_create(mem_allocrator_t alloc);
void dr_metalib_builder_free(dr_metalib_builder_t builder);

dr_metalib_source_t dr_metalib_builder_add_file(dr_metalib_builder_t builder, const char * file);
dr_metalib_source_t dr_metalib_builder_add_buf(dr_metalib_builder_t builder, const char * name, dr_metalib_source_format_t, const void * buf);
void dr_metalib_source_free(dr_metalib_source_t source);

dr_metalib_source_type_t dr_metalib_source_type(dr_metalib_source_t source);
dr_metalib_source_format_t dr_metalib_source_format(dr_metalib_source_t source);

dr_metalib_source_t dr_metalib_source_find(dr_metalib_builder_t builder, const char * name);
int dr_metalib_source_add_include(dr_metalib_source_t user, dr_metalib_source_t using);

#ifdef __cplusplus
}
#endif

#endif
