#include "cpe/dr/dr_metalib_builder.h"
#include "builder_create.h"

dr_metalib_builder_t
builder_create(int argc, char * argv[], error_monitor_t em) {
    dr_metalib_builder_t builder;

    builder = dr_metalib_builder_create(NULL);
    if (builder == NULL) {
        CPE_ERROR(em, "create metalib builder fail!");
        return NULL;
    }

    return builder;
}
