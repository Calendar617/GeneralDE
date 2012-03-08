#include "argtable2.h"
#include "cpe/dr/dr_metalib_builder.h"
#include "generate_ops.h"

struct arg_lit * i;
struct arg_lit * help;
struct arg_end *end;

int tools_main(error_monitor_t em) {
    dr_metalib_builder_t builder = NULL;

    builder = dr_metalib_builder_create(NULL, em);
    if (builder == NULL) {
        CPE_ERROR(em, "create metalib builder fail!");
        return -1;
    }


    dr_metalib_builder_analize(builder);
    return 0;
}

int main(int argc, char * argv[]) {
    void* argtable[] = {
                i = arg_lit0(   "i",   NULL,       "input file")
        ,    help = arg_lit0(   NULL,  "help",     "print this help and exit")
        ,     end = arg_end(20)
    };

    struct error_monitor em_buf;
    error_monitor_t em;
    int rv;

    cpe_error_monitor_init(&em_buf, cpe_error_log_to_consol, 0);
    em = &em_buf;

    rv = -1;

    if (arg_nullcheck(argtable) != 0) {
        CPE_ERROR(em, "init arg table fail!");
        goto exit;
    }

    if (help->count > 0) {
        printf("Usage: %s", "cpe_dr_tool");
        arg_print_syntax(stdout,argtable,"\n");
        rv = 0;
        goto exit;
    }

    rv = tools_main(em);

exit:
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

    return rv;
}
