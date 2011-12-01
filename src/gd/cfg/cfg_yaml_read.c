#include "yaml.h"
#include "gd/cfg/cfg_manage.h"
#include "cfg_internal_types.h"

struct gd_cfg_yaml_read_ctx {
    yaml_parser_t m_parser;
    yaml_event_t m_input_event;

    error_monitor_t m_em;
};

static int gd_cfg_read_from_stream_read_handler(
    void *data,
    unsigned char *buffer,
    size_t size,
    size_t *size_read)
{
    int readSize;

    readSize = stream_read((read_stream_t)data, (char*)buffer, size);

    if (readSize >= 0) {
        *size_read = (size_t)readSize;
        return 1;
    }
    else {
        return 0;
    }
}

static int gd_cfg_yaml_read_ctx_init(
    struct gd_cfg_yaml_read_ctx * ctx,
    read_stream_t stream,
    error_monitor_t em)
{
    bzero(ctx, sizeof(struct gd_cfg_yaml_read_ctx));

    if (!yaml_parser_initialize(&ctx->m_parser)) {
        CPE_ERROR(em, "yaml parsser initialize fail!");
        return -1;
    }

    yaml_parser_set_input(&ctx->m_parser, gd_cfg_read_from_stream_read_handler, stream);

    ctx->m_em = em;

    return 0;
}

static void gd_cfg_yaml_read_ctx_fini(struct gd_cfg_yaml_read_ctx * ctx) {
    yaml_parser_delete(&ctx->m_parser);
    yaml_event_delete(&ctx->m_input_event);
}

static void gd_cfg_yaml_on_stream_begin(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_on_stream_end(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_on_document_begin(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_on_document_end(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_on_alias(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_on_scalar(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_on_sequence_begin(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_on_sequence_end(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_on_map_begin(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_on_map_end(struct gd_cfg_yaml_read_ctx * ctx) {
}

static void gd_cfg_yaml_notify_parse_error(struct gd_cfg_yaml_read_ctx * ctx) {
    switch (ctx->m_parser.error) {
        case YAML_MEMORY_ERROR:
            CPE_ERROR(ctx->m_em, "Memory error: Not enough memory for parsing\n");
            break;
        case YAML_READER_ERROR:
            if (ctx->m_parser.problem_value != -1) {
                CPE_ERROR(
                    ctx->m_em, "Reader error: %s: #%X at %d\n",
                    ctx->m_parser.problem,
                    ctx->m_parser.problem_value,
                    ctx->m_parser.problem_offset);
            }
            else {
                CPE_ERROR(
                    ctx->m_em,  "Reader error: %s at %d\n",
                    ctx->m_parser.problem,
                    ctx->m_parser.problem_offset);
            }
            break;
        case YAML_SCANNER_ERROR:
            if (ctx->m_parser.context) {
                CPE_ERROR(
                    ctx->m_em, "Scanner error: %s at line %d, column %d\n"
                    "%s at line %d, column %d\n", ctx->m_parser.context,
                    ctx->m_parser.context_mark.line+1, ctx->m_parser.context_mark.column+1,
                    ctx->m_parser.problem, ctx->m_parser.problem_mark.line+1,
                    ctx->m_parser.problem_mark.column+1);
            }
            else {
                CPE_ERROR(
                    ctx->m_em, "Scanner error: %s at line %d, column %d\n",
                    ctx->m_parser.problem, ctx->m_parser.problem_mark.line+1,
                    ctx->m_parser.problem_mark.column+1);
            }
            break;
        case YAML_PARSER_ERROR:
            if (ctx->m_parser.context) {
                CPE_ERROR(
                    ctx->m_em, "Parser error: %s at line %d, column %d\n"
                    "%s at line %d, column %d\n", ctx->m_parser.context,
                    ctx->m_parser.context_mark.line+1, ctx->m_parser.context_mark.column+1,
                    ctx->m_parser.problem, ctx->m_parser.problem_mark.line+1,
                    ctx->m_parser.problem_mark.column+1);
            }
            else {
                CPE_ERROR(
                    ctx->m_em, "Parser error: %s at line %d, column %d\n",
                    ctx->m_parser.problem, ctx->m_parser.problem_mark.line+1,
                    ctx->m_parser.problem_mark.column+1);
            }
            break;
        default:
            /* Couldn't happen. */
            CPE_ERROR(ctx->m_em, "Yaml Internal error\n");
            break;
    }
}

typedef void (*gd_cfg_yaml_read_event_process_fun_t)(struct gd_cfg_yaml_read_ctx * ctx);

static
gd_cfg_yaml_read_event_process_fun_t
g_yaml_read_event_processors[YAML_MAPPING_END_EVENT + 1] = {
    /*YAML_NO_EVENT*/ 0,
    /*YAML_STREAM_START_EVENT*/ gd_cfg_yaml_on_stream_begin,
    /*YAML_STREAM_END_EVENT*/ gd_cfg_yaml_on_stream_end,
    /*YAML_DOCUMENT_START_EVENT*/ gd_cfg_yaml_on_document_begin,
    /*YAML_DOCUMENT_END_EVENT*/ gd_cfg_yaml_on_document_end,
    /*YAML_ALIAS_EVENT*/ gd_cfg_yaml_on_alias,
    /*YAML_SCALAR_EVENT*/ gd_cfg_yaml_on_scalar,
    /*YAML_SEQUENCE_START_EVENT*/ gd_cfg_yaml_on_sequence_begin,
    /*YAML_SEQUENCE_END_EVENT*/ gd_cfg_yaml_on_sequence_end,
    /*YAML_MAPPING_START_EVENT*/ gd_cfg_yaml_on_map_begin,
    /*YAML_MAPPING_END_EVENT*/ gd_cfg_yaml_on_map_end
};

static void gd_cfg_read_i(gd_cfg_t cfg, read_stream_t stream, error_monitor_t em) {
    struct gd_cfg_yaml_read_ctx ctx;
    int done = 0;

    if (gd_cfg_yaml_read_ctx_init(&ctx, stream, em) != 0) return;

    while (!done) {
        if (!yaml_parser_parse(&ctx.m_parser, &ctx.m_input_event)) {
            gd_cfg_yaml_notify_parse_error(&ctx);
            break;
        }

        if (ctx.m_input_event.type == YAML_STREAM_END_EVENT) done = 1;

        if (ctx.m_input_event.type < 0
            || ctx.m_input_event.type >= sizeof(g_yaml_read_event_processors)/sizeof(gd_cfg_yaml_read_event_process_fun_t))
        {
            CPE_ERROR(em, "unknown yaml event %d!", ctx.m_input_event.type);
            done = 1;
        }
        else {
            gd_cfg_yaml_read_event_process_fun_t processor = g_yaml_read_event_processors[ctx.m_input_event.type];
            if (processor == NULL) {
                CPE_ERROR(em, "no processor for yaml event %d!", ctx.m_input_event.type);
                done = 1;
            }
            else {
                processor(&ctx);
            }
        }
        
        yaml_event_delete(&ctx.m_input_event);
    }

    gd_cfg_yaml_read_ctx_fini(&ctx);
}

int gd_cfg_read_stream(gd_cfg_t cfg, read_stream_t stream, error_monitor_t em) {
    int ret = 0;
    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        gd_cfg_read_i(cfg, stream, em);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        gd_cfg_read_i(cfg, stream, &logError);
    }

    return ret;
}
