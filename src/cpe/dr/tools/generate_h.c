#include "generate_ops.h"

int cpe_dr_generate_h(write_stream_t stream, dr_metalib_source_t source, cpe_dr_generate_ctx_t ctx) {
    const char * lib_name;

    assert(source);
    assert(stream);

    if ((lib_name = dr_metalib_source_libname(source))) {
        stream_printf(stream, "#ifndef DR_GENERATED_H_%s_%s_INCLEDED\n", "", "");
        stream_printf(stream, "#define DR_GENERATED_H_%s_%s_INCLEDED\n", "", "");
    }
    else {
        stream_printf(stream, "#ifndef DR_GENERATED_H_%s_INCLEDED\n", "", "");
        stream_printf(stream, "#define DR_GENERATED_H_%s_INCLEDED\n", "", "");
    }

    stream_printf(stream, "#endif\n");

    return 0;
}
