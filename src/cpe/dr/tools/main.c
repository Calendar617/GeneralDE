#include "argtable2.h"
#include "cpe/utils/buffer.h"
#include "cpe/utils/file.h"
#include "cpe/utils/stream_file.h"
#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_builder.h"
#include "cpe/dr/dr_metalib_build.h"
#include "generate_ops.h"

struct arg_file * input;
struct arg_file * o_h;
struct arg_file * o_lib_c;
struct arg_str * o_lib_c_arg;
struct arg_file * o_lib_bin;
struct arg_lit * help;
struct arg_end *end;

void prepare_input(dr_metalib_builder_t builder, error_monitor_t em) {
    int i;
    for(i = 0; i < input->count; ++i) {
        dr_metalib_builder_add_file(builder, NULL, input->filename[i]);
    }
}

static int do_generate_lib_bin(cpe_dr_generate_ctx_t ctx) {
    int i;
    int rv;

    rv = 0;

    for(i = 0; i < o_lib_bin->count; ++i) {
        FILE * fp;

        fp = file_stream_open(o_lib_bin->filename[i], "w", ctx->m_em);
        if (fp == NULL) {
            rv = -1;
            continue;
        }

        if (file_stream_write_from_buf(fp, ctx->m_metalib, dr_lib_size(ctx->m_metalib), ctx->m_em) <= 0) {
            rv = -1;
        }

        file_stream_close(fp, ctx->m_em);
    }

    return rv;
}

static int do_generate_lib_c(cpe_dr_generate_ctx_t ctx) {
    struct write_stream_file stream;
    int i;
    int rv;

    rv = 0;

    for(i = 0; i < o_lib_c->count; ++i) {
        FILE * fp;

        if (o_lib_c_arg->count == 0) {
            CPE_ERROR(ctx->m_em, "not arg name setted!");
            return -1;
        }

        fp = file_stream_open(o_lib_c->filename[i], "w", ctx->m_em);
        if (fp == NULL) {
            rv = -1;
            continue;
        }

        write_stream_file_init(&stream, fp, ctx->m_em);

        cpe_dr_generate_lib_c((write_stream_t)&stream, o_lib_c_arg->sval[0], ctx);

        file_stream_close(fp, ctx->m_em);
    }

    return rv;
}

static int do_generate_h(cpe_dr_generate_ctx_t ctx) {
    struct dr_metalib_source_it source_it;
    dr_metalib_source_t source;
    struct mem_buffer buffer;
    struct write_stream_file stream;
    int i;
    FILE * fp;
    int rv;

    mem_buffer_init(&buffer, 0);

    rv = 0;
    for(i = 0; i < o_h->count; ++i) {
        dr_metalib_builder_sources(&source_it, ctx->m_builder);
        while((source = dr_metalib_source_next(&source_it))) {
            const char * file_name;

            if (dr_metalib_source_from(source) != dr_metalib_source_from_user) continue;

            file_name = dr_metalib_source_file(source);
            if (file_name == NULL) continue;

            mem_buffer_clear_data(&buffer);
            mem_buffer_strcat(&buffer, o_h->filename[i]);
            mem_buffer_strcat(&buffer, "/");
            file_name_append_base(&buffer, file_name);
            mem_buffer_strcat(&buffer, ".h");

            fp = file_stream_open((char *)mem_buffer_make_continuous(&buffer, 0), "w", ctx->m_em);
            if (fp == NULL) {
                rv = -1;
                continue;
            }

            write_stream_file_init(&stream, fp, ctx->m_em);
            cpe_dr_generate_h((write_stream_t)&stream, source, ctx);
            file_stream_close(fp, ctx->m_em);
        }
    }

    mem_buffer_clear(&buffer);

    return rv;
}

int tools_main(error_monitor_t em) {
    struct cpe_dr_generate_ctx ctx;
    struct mem_buffer buffer;
    int rv;

    ctx.m_builder = NULL;
    ctx.m_metalib = NULL;
    ctx.m_em = em;

    rv = 0;

    ctx.m_builder = dr_metalib_builder_create(NULL, em);
    if (ctx.m_builder == NULL) {
        CPE_ERROR(em, "create metalib builder fail!");
        return -1;
    }

    prepare_input(ctx.m_builder, em);

    dr_metalib_builder_analize(ctx.m_builder);

    mem_buffer_init(&buffer, 0);
    if (dr_inbuild_build_lib(
            &buffer,
            dr_metalib_bilder_lib(ctx.m_builder),
            em) == 0)
    {
        ctx.m_metalib = (LPDRMETALIB)mem_buffer_make_continuous(&buffer, 0), mem_buffer_size(&buffer);
        if (ctx.m_metalib) {
            if (do_generate_h(&ctx) != 0) rv = -1;
            if (do_generate_lib_bin(&ctx) != 0) rv = -1;
            if (do_generate_lib_c(&ctx) != 0) rv = -1;
        }
    }
    else {
        rv = -1;
    }

    mem_buffer_clear(&buffer);
    dr_metalib_builder_free(ctx.m_builder);

    return rv;
}

int main(int argc, char * argv[]) {
    void* argtable[] = {
                input = arg_filen(   "i",   "input",              "<string>", 0, 100,    "input file")
        ,         o_h = arg_file0(   NULL,  "output-h",           "<string>",            "output h file dir")
        ,     o_lib_c = arg_file0(   NULL,  "output-lib-c",       "<string>",            "output c lib file")
        , o_lib_c_arg = arg_str0(   NULL,  "output-lib-c-arg",    "<string>",            "output c lib file")
        ,   o_lib_bin = arg_file0(   NULL,  "output-lib-bin",     "<string>",            "output c lib file")
        ,        help = arg_lit0(   NULL,  "help",                                   "print this help and exit")
        ,         end = arg_end(20)
    };

    struct error_monitor em_buf;
    error_monitor_t em;
    int rv;
    int nerrors;

    cpe_error_monitor_init(&em_buf, cpe_error_log_to_consol, 0);
    em = &em_buf;

    rv = -1;

    if (arg_nullcheck(argtable) != 0) {
        CPE_ERROR(em, "init arg table fail!");
        goto exit;
    }

    nerrors = arg_parse(argc,argv,argtable);

    if (help->count > 0) {
        printf("Usage: %s", argv[0]);
        arg_print_syntax(stdout,argtable,"\n");
        rv = 0;
        goto exit;
    }

    if (nerrors > 0) {
        arg_print_errors(stdout, end, argv[0]);
        printf("Try '%s --help' for more information.\n", argv[0]);
        goto exit;
    }

    rv = tools_main(em);

exit:
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

    return rv;
}
