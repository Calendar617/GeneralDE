#include <assert.h>
#include "cpe/utils/stream.h"
#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_manage.h"

static void dr_lib_dump_meta(write_stream_t stream, LPDRMETA meta, int ident) {
    stream_putc(stream, '\n');
    stream_putc_count(stream, ' ', ident);
    
    stream_printf(stream, "%s[%d]:", dr_meta_name(meta), dr_meta_id(meta));
}

void dr_lib_dump(write_stream_t stream, LPDRMETALIB metaLib, int ident) {
    int i;
    int meta_count;

    stream_putc_count(stream, ' ', ident);

    if (metaLib == NULL) {
        stream_printf(stream, "invalid METALIB!!!");
        return;
    }

    stream_printf(
        stream, "meta-lib: name=%s, version=%d, build-version=%d, size=%d", 
        dr_lib_name(metaLib), dr_lib_version(metaLib), dr_lib_build_version(metaLib), dr_lib_size(metaLib));

    meta_count = dr_lib_meta_num(metaLib);
    for(i = 0; i < meta_count; ++i) {
        dr_lib_dump_meta(stream, dr_lib_meta_at(metaLib, i), ident + 4);
    }

    stream_putc(stream, '\n');
}
