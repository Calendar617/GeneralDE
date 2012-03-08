#include "builder_create.h"

int main(int argc, char * argv[]) {
    struct error_monitor em;
    dr_metalib_builder_t builder;

    cpe_error_monitor_init(&em, cpe_error_log_to_consol, 0);

    builder = builder_create(argc, argv, &em);
    dr_metalib_builder_analize(builder);
    
    return 0;
}
