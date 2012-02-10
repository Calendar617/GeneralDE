#include "cpe/utils/stream.h"
#include "cpe/dr/dr_ctypes_op.h"
#include "cpe/cfg/cfg_read.h"
#include "cfg_internal_types.h"

void cfg_dump(cfg_t cfg, write_stream_t stream, int ident, int level_ident) {
    struct cfg_it it;
    cfg_t child;
    int c;

    if(cfg == 0) return;

    switch(cfg->m_type) {
    case CPE_CFG_TYPE_SEQUENCE: {
        stream_putc(stream, '[');

        c = 0;
        cfg_it_init(&it, cfg);

        if ((child = cfg_it_next(&it))) {
            stream_putc(stream, ' ');
            cfg_dump(child, stream, ident + level_ident, level_ident);
            ++c;
        }
        
        while((child = cfg_it_next(&it))) {
            stream_putc(stream, '\n');
            stream_putc_count(stream, ' ', ident);
            stream_putc(stream, ',');
            stream_putc(stream, ' ');

            cfg_dump(child, stream, ident + level_ident, level_ident);
            ++c;
        }

        if (c == 1) {
            stream_putc(stream, ' ');
        }
        else if (c > 1) {
            stream_putc(stream, '\n');
            stream_putc_count(stream, ' ', ident);
        }
            
        stream_putc(stream, ']');

        break;
    }
    case CPE_CFG_TYPE_STRUCT: {
        stream_putc(stream, '{');

        c = 0;
        cfg_it_init(&it, cfg);

        if ((child = cfg_it_next(&it))) {
            stream_printf(stream, " %s=", cfg_name(child));
            cfg_dump(child, stream, ident + level_ident, level_ident);
            ++c;
        }
        
        while((child = cfg_it_next(&it))) {
            stream_putc(stream, '\n');
            stream_putc_count(stream, ' ', ident);

            stream_printf(stream, ", %s=", cfg_name(child));
            cfg_dump(child, stream, ident + level_ident, level_ident);
            ++c;
        }

        if (c == 1) {
            stream_putc(stream, ' ');
        }
        else if (c > 1) {
            stream_putc(stream, '\n');
            stream_putc_count(stream, ' ', ident);
        }
            
        stream_putc(stream, '}');

        break;
    }
    default:
        dr_ctype_print_to_stream(stream, cfg_data(cfg), cfg->m_type, 0);
        break;
    }
}

void cfg_dump_inline(cfg_t cfg, write_stream_t stream) {
    struct cfg_it it;
    cfg_t child;
    int c;

    if(cfg == 0) return;

    switch(cfg->m_type) {
    case CPE_CFG_TYPE_SEQUENCE: {
        stream_putc(stream, '[');

        c = 0;
        cfg_it_init(&it, cfg);

        if ((child = cfg_it_next(&it))) {
            stream_putc(stream, ' ');
            cfg_dump_inline(child, stream);
            ++c;
        }
        
        while((child = cfg_it_next(&it))) {
            stream_putc(stream, ',');
            stream_putc(stream, ' ');

            cfg_dump_inline(child, stream);
            ++c;
        }

        if (c > 0) {
            stream_putc(stream, ' ');
        }
            
        stream_putc(stream, ']');

        break;
    }
    case CPE_CFG_TYPE_STRUCT: {
        stream_putc(stream, '{');

        c = 0;
        cfg_it_init(&it, cfg);

        if ((child = cfg_it_next(&it))) {
            stream_printf(stream, " %s=", cfg_name(child));
            cfg_dump_inline(child, stream);
            ++c;
        }
        
        while((child = cfg_it_next(&it))) {
            stream_printf(stream, ", %s=", cfg_name(child));
            cfg_dump_inline(child, stream);
            ++c;
        }

        if (c > 0) {
            stream_putc(stream, ' ');
        }
            
        stream_putc(stream, '}');

        break;
    }
    default:
        dr_ctype_print_to_stream(stream, cfg_data(cfg), cfg->m_type, 0);
        break;
    }
}
