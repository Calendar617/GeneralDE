#include <strings.h>
#include "gd/app/app_context.h"
#include "app_internal_types.h"

gd_app_context_t gd_app_context_create(
    mem_allocrator_t alloc, size_t capacity,
    int argc, char * argv[])
{
    gd_app_context_t context;
    size_t allocSize;

    allocSize = sizeof(struct gd_app_context) + capacity;
    context = (gd_app_context_t)mem_alloc(alloc, allocSize);
    if (context == NULL) return NULL;

    bzero(context, allocSize);

	context->m_argc = argc;
	context->m_argv = argv;

    context->m_alloc = alloc;
    context->m_capacity = capacity;

    return context;
}

void gd_app_context_free(gd_app_context_t context) {
    if (context == NULL) return;

    mem_free(context->m_alloc, context);
}

int gd_app_context_argc(gd_app_context_t context) {
    return context->m_argc;
}

char ** gd_app_context_argv(gd_app_context_t context) {
    return context->m_argv;
}

void * gd_app_context_user_data(gd_app_context_t context) {
    return (void*)(context + 1);
}

int gd_app_run(gd_app_context_t context) {
    return -1;
}

