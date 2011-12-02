#include <assert.h>
#include "yaml.h"
#include "cpe/utils/buffer.h"
#include "cpe/cfg/cfg_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "cfg_internal_types.h"

struct cfg_yaml_read_ctx {
    yaml_parser_t m_parser;
    yaml_event_t m_input_event;

    cfg_t m_root;
    cfg_t m_curent;

    const char * m_name;
    struct mem_buffer m_name_buffer;

    error_monitor_t m_em;
};

static int cfg_read_from_stream_read_handler(
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

static int cfg_yaml_read_ctx_init(
    struct cfg_yaml_read_ctx * ctx,
    cfg_t root,
    read_stream_t stream,
    error_monitor_t em)
{
    bzero(ctx, sizeof(struct cfg_yaml_read_ctx));

    if (cfg_is_value(root)) {
        CPE_ERROR(em, "can`t read data into a data node!");
        return -1;
    }
        
    if (!yaml_parser_initialize(&ctx->m_parser)) {
        CPE_ERROR(em, "yaml parsser initialize fail!");
        return -1;
    }

    yaml_parser_set_input(&ctx->m_parser, cfg_read_from_stream_read_handler, stream);

    ctx->m_name = NULL;
    mem_buffer_init(&ctx->m_name_buffer, NULL);

    ctx->m_root = ctx->m_curent = root;
    ctx->m_em = em;

    return 0;
}

static void cfg_yaml_read_ctx_fini(struct cfg_yaml_read_ctx * ctx) {
    mem_buffer_clear(&ctx->m_name_buffer);
    yaml_parser_delete(&ctx->m_parser);
    yaml_event_delete(&ctx->m_input_event);
}

static void cfg_yaml_on_stream_begin(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;
}

static void cfg_yaml_on_stream_end(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;
}

static void cfg_yaml_on_document_begin(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;
}

static void cfg_yaml_on_document_end(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;
}

static void cfg_yaml_on_alias(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;
}

static void cfg_yaml_on_scalar(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;

    if(ctx->m_curent->m_type == CPE_CFG_TYPE_STRUCT) {
        if (ctx->m_name == NULL) {
            mem_buffer_clear_data(&ctx->m_name_buffer);
            ctx->m_name = mem_buffer_strndup(
                &ctx->m_name_buffer,
                (const char *)ctx->m_input_event.data.scalar.value,
                ctx->m_input_event.data.scalar.length);
            if (ctx->m_name == NULL) {
                CPE_ERROR(ctx->m_em, "dump scalar as name, no memory!");
                ctx->m_name = "";
            }
        }
        else {
            if (ctx->m_input_event.data.scalar.length > 0) {
                const char * value = 
                    mem_buffer_strndup(
                        &ctx->m_name_buffer,
                        (const char *)ctx->m_input_event.data.scalar.value,
                        ctx->m_input_event.data.scalar.length);
                if (value == NULL) {
                    CPE_ERROR(ctx->m_em, "dump scalar as map value, no memory!");
                }
                else {
                    cfg_struct_add_string(ctx->m_curent, ctx->m_name, value);
                    ctx->m_name = NULL;
                }
            }
            else {
                cfg_struct_add_string(ctx->m_curent, ctx->m_name, "");
                ctx->m_name = NULL;
            }
        }
    }
    else {
        assert(ctx->m_curent->m_type == CPE_CFG_TYPE_SEQUENCE);

        if (ctx->m_input_event.data.scalar.length > 0) {
            const char * value = 
                mem_buffer_strndup(
                    &ctx->m_name_buffer,
                    (const char *)ctx->m_input_event.data.scalar.value,
                    ctx->m_input_event.data.scalar.length);
            if (value == NULL) {
                CPE_ERROR(ctx->m_em, "dump scalar as seq value, no memory!");
            }
            else {
                cfg_seq_add_string(ctx->m_curent, value);
            }
        }
        else {
            //ignore empty in sequence
        }
    }
}

static void cfg_yaml_on_sequence_begin(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;

    if (ctx->m_curent->m_type == CPE_CFG_TYPE_STRUCT) {
        if (ctx->m_name != NULL) {
            ctx->m_curent = cfg_struct_add_seq(ctx->m_curent, ctx->m_name);
            ctx->m_name = NULL;
        }
        else if (ctx->m_curent == ctx->m_root) {
            ctx->m_curent = cfg_struct_add_seq(ctx->m_curent, "");
        }
        else {
            CPE_ERROR(ctx->m_em, "no name for new seq!");
            ctx->m_curent = NULL;
        }
    }
    else {
        assert(ctx->m_curent->m_type == CPE_CFG_TYPE_SEQUENCE);
        ctx->m_curent = cfg_seq_add_seq(ctx->m_curent);
    }
}

static void cfg_yaml_on_sequence_end(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;

    if (ctx->m_curent != ctx->m_root) {
        ctx->m_curent = cfg_parent(ctx->m_curent);
    }
}

static void cfg_yaml_on_map_begin(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;

    if (ctx->m_curent->m_type == CPE_CFG_TYPE_STRUCT) {
        if (ctx->m_name != NULL) {
            ctx->m_curent = cfg_struct_add_struct(ctx->m_curent, ctx->m_name);
            ctx->m_name = NULL;
        }
        else if (ctx->m_curent == ctx->m_root) {
            //DO NOTHING
        }
        else {
            CPE_ERROR(ctx->m_em, "no name for new map!");
            ctx->m_curent = NULL;
        }
    }
    else {
        assert(ctx->m_curent->m_type == CPE_CFG_TYPE_SEQUENCE);
        ctx->m_curent = cfg_seq_add_struct(ctx->m_curent);
    }
}

static void cfg_yaml_on_map_end(struct cfg_yaml_read_ctx * ctx) {
    if (ctx->m_curent == NULL) return;

    if (ctx->m_curent != ctx->m_root) {
        ctx->m_curent = cfg_parent(ctx->m_curent);
    }
}

static void cfg_yaml_notify_parse_error(struct cfg_yaml_read_ctx * ctx) {
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
                    (int)ctx->m_parser.problem_offset);
            }
            else {
                CPE_ERROR(
                    ctx->m_em,  "Reader error: %s at %d\n",
                    ctx->m_parser.problem,
                    (int)ctx->m_parser.problem_offset);
            }
            break;
        case YAML_SCANNER_ERROR:
            if (ctx->m_parser.context) {
                CPE_ERROR(
                    ctx->m_em, "Scanner error: %s at line %d, column %d\n"
                    "%s at line %d, column %d\n",
                    ctx->m_parser.context,
                    (int)ctx->m_parser.context_mark.line+1,
                    (int)ctx->m_parser.context_mark.column+1,
                    ctx->m_parser.problem,
                    (int)ctx->m_parser.problem_mark.line+1,
                    (int)ctx->m_parser.problem_mark.column+1);
            }
            else {
                CPE_ERROR(
                    ctx->m_em, "Scanner error: %s at line %d, column %d\n",
                    ctx->m_parser.problem,
                    (int)ctx->m_parser.problem_mark.line+1,
                    (int)ctx->m_parser.problem_mark.column+1);
            }
            break;
        case YAML_PARSER_ERROR:
            if (ctx->m_parser.context) {
                CPE_ERROR(
                    ctx->m_em,
                    "Parser error: %s at line %d, column %d\n"
                    "%s at line %d, column %d\n",
                    ctx->m_parser.context,
                    (int)ctx->m_parser.context_mark.line+1,
                    (int)ctx->m_parser.context_mark.column+1,
                    ctx->m_parser.problem,
                    (int)ctx->m_parser.problem_mark.line+1,
                    (int)ctx->m_parser.problem_mark.column+1);
            }
            else {
                CPE_ERROR(
                    ctx->m_em, "Parser error: %s at line %d, column %d\n",
                    ctx->m_parser.problem,
                    (int)ctx->m_parser.problem_mark.line+1,
                    (int)ctx->m_parser.problem_mark.column+1);
            }
            break;
        default:
            /* Couldn't happen. */
            CPE_ERROR(ctx->m_em, "Yaml Internal error\n");
            break;
    }
}

typedef void (*cfg_yaml_read_event_process_fun_t)(struct cfg_yaml_read_ctx * ctx);

static
cfg_yaml_read_event_process_fun_t
g_yaml_read_event_processors[YAML_MAPPING_END_EVENT + 1] = {
    /*YAML_NO_EVENT*/ 0,
    /*YAML_STREAM_START_EVENT*/ cfg_yaml_on_stream_begin,
    /*YAML_STREAM_END_EVENT*/ cfg_yaml_on_stream_end,
    /*YAML_DOCUMENT_START_EVENT*/ cfg_yaml_on_document_begin,
    /*YAML_DOCUMENT_END_EVENT*/ cfg_yaml_on_document_end,
    /*YAML_ALIAS_EVENT*/ cfg_yaml_on_alias,
    /*YAML_SCALAR_EVENT*/ cfg_yaml_on_scalar,
    /*YAML_SEQUENCE_START_EVENT*/ cfg_yaml_on_sequence_begin,
    /*YAML_SEQUENCE_END_EVENT*/ cfg_yaml_on_sequence_end,
    /*YAML_MAPPING_START_EVENT*/ cfg_yaml_on_map_begin,
    /*YAML_MAPPING_END_EVENT*/ cfg_yaml_on_map_end
};

static void cfg_read_i(cfg_t cfg, read_stream_t stream, error_monitor_t em) {
    struct cfg_yaml_read_ctx ctx;
    int done = 0;

    if (cfg_yaml_read_ctx_init(&ctx, cfg, stream, em) != 0) return;

    while (!done) {
        if (!yaml_parser_parse(&ctx.m_parser, &ctx.m_input_event)) {
            cfg_yaml_notify_parse_error(&ctx);
            break;
        }

        if (ctx.m_input_event.type == YAML_STREAM_END_EVENT) done = 1;

        if (ctx.m_input_event.type < 0
            || ctx.m_input_event.type >= sizeof(g_yaml_read_event_processors)/sizeof(cfg_yaml_read_event_process_fun_t))
        {
            CPE_ERROR(em, "unknown yaml event %d!", ctx.m_input_event.type);
            done = 1;
        }
        else {
            cfg_yaml_read_event_process_fun_t processor = g_yaml_read_event_processors[ctx.m_input_event.type];
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

    cfg_yaml_read_ctx_fini(&ctx);
}

int cfg_read(cfg_t cfg, read_stream_t stream, error_monitor_t em) {
    int ret = 0;
    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        cfg_read_i(cfg, stream, em);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        cfg_read_i(cfg, stream, &logError);
    }

    return ret;
}
