#include "argtable2.h"
#include "cpe/pal/pal_dlfcn.h"
#include "cpe/pal/pal_stdio.h"
#include "gd/app/app_log.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"

struct arg_file * a_input;
struct arg_str * a_ip;
struct arg_int * a_port;
struct arg_lit * a_help;
struct arg_end * a_end;

int tools_main(gd_app_context_t app) {
    gd_set_default_library(dlopen(NULL, RTLD_NOW));
    gd_app_flag_enable(app, gd_app_flag_no_auto_load);

    if (gd_app_install_module(app, "app_net_runner", NULL, NULL, NULL) == NULL) {
        APP_CTX_ERROR(app, "load app_net_runner fail");
        return -1;
    }

    return gd_app_run(app);
}

int main(int argc, char * argv[]) {
    int rv = 0;
    gd_app_context_t app = NULL;
    void* argtable[] = {
                 a_ip = arg_str1(   NULL,      "ip",              "<string>",     "target ip")
         ,    a_port = arg_int1(   "p",   "port",              "<string>",     "input file")
         ,    a_input = arg_file1(   "i",   "input",              "<string>",     "input file")
         ,    a_help = arg_lit0(   NULL,  "help",                                   "print this help and exit")
         ,    a_end = arg_end(20)
    };
    int nerrors;

    if (arg_nullcheck(argtable) != 0) {
        printf("init arg table fail!");
        goto exit;
    }

    nerrors = arg_parse(argc, argv, argtable);

    if (a_help->count > 0) {
        printf("Usage: %s", argv[0]);
        arg_print_syntax(stdout,argtable,"\n");
        rv = 0;
        goto exit;
    }

    if (nerrors > 0) {
        arg_print_errors(stdout, a_end, argv[0]);
        printf("Try '%s --help' for more information.\n", argv[0]);
        goto exit;
    }

    app = gd_app_context_create(NULL, 0, argc, argv);
    if (app == NULL) {
        printf("create app context fail!");
        return -1;
    }

    rv = tools_main(app);

exit:
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    if (app) { gd_app_context_free(app); app = NULL; }

    return rv;
}
