#include "argtable2.h"
#include "cpe/pal/pal_dlfcn.h"
#include "cpe/pal/pal_stdio.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/cfg/cfg_manage.h"
#include "gd/app/app_log.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"

struct arg_file * a_input;
struct arg_str * a_ip;
struct arg_int * a_port;
struct arg_lit * a_help;
struct arg_int * a_app_debug;
struct arg_end * a_end;

int prepaire_dr_load_cfg(gd_app_context_t app, cfg_t cfg, struct gd_app_module_def * module_def, void * ctx) {
    if (a_input->count == 1 && strcmp(a_input->extension[0], ".bin") == 0) {
        cfg_struct_add_string(cfg, "load-from-bin", a_input->filename[0], cfg_replace);
    }
    else {
        cfg_t seq = cfg_struct_add_seq(cfg, "load-from-file", cfg_replace);
        int i = 0;
        for(; i < a_input->count; ++i) {
            cfg_seq_add_string(seq, a_input->filename[i]);
        }
    }

    cfg_struct_add_int32(cfg, "debug", 1, cfg_replace);

    return 0;
}

int prepaire_bpg_net_client_cfg(gd_app_context_t app, cfg_t cfg, struct gd_app_module_def * module_def, void * ctx) {
    return 0;
}

static struct gd_app_module_def g_init_modules[] = {
    { "app_net_runner", NULL, NULL, NULL, NULL}
    , { "dr_store_manage", NULL, NULL, NULL, NULL }
    , { "dr_loader", "dr_store_loader", NULL, NULL, prepaire_dr_load_cfg }
    , { "logic_manage", NULL, NULL, NULL, NULL }
    , { "bpg_manage", NULL, NULL, NULL, NULL }
    , { "bpg_net_client", NULL, NULL, NULL, prepaire_bpg_net_client_cfg }
};

int tools_main(gd_app_context_t app) {
    gd_set_default_library(dlopen(NULL, RTLD_NOW));
    gd_app_flag_enable(app, gd_app_flag_no_auto_load);

    if (gd_app_bulk_install_module(app, g_init_modules, sizeof(g_init_modules) / sizeof(g_init_modules[0]), NULL) != 0) {
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
         ,    a_input = arg_filen(   "i",   "input",              "<string>",   1, 100,  "input file")
         ,    a_help = arg_lit0(   NULL,  "help",                                   "print this help and exit")
         , a_app_debug = arg_int0(   NULL,  "app-debug",        "<string>", "debug app framework")
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

    if (a_app_debug->count > 0) {
        gd_app_set_debug(app, a_app_debug->ival[0]);
    }

    rv = tools_main(app);

exit:
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    if (app) { gd_app_context_free(app); app = NULL; }

    return rv;
}
