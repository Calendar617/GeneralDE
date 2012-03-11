#include <assert.h>
#include "cpe/dr/dr_ctypes_info.h"
#include "cpe/dr/dr_metalib_builder.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "generate_ops.h"

static void cpe_dr_generate_h_macros(write_stream_t stream, dr_metalib_source_t source, cpe_dr_generate_ctx_t ctx) {
    struct dr_metalib_source_element_it element_it;
    dr_metalib_source_element_t element;
    int macro_value;

    dr_metalib_source_elements(&element_it, source);
    while((element = dr_metalib_source_element_next(&element_it))) {
       if (dr_metalib_source_element_type(element) != dr_metalib_source_element_type_macro) continue;

       if (dr_lib_find_macro_value(&macro_value, ctx->m_metalib, dr_metalib_source_element_name(element)) == 0) {
           stream_printf(
               stream, "\n#define %s (%d)",
               dr_metalib_source_element_name(element),
               macro_value);
       }
    }
}

static void cpe_dr_generate_h_metas(write_stream_t stream, dr_metalib_source_t source, cpe_dr_generate_ctx_t ctx) {
    struct dr_metalib_source_element_it element_it;
    dr_metalib_source_element_t element;
    int curent_pack;
    int packed;

    curent_pack = 0;
    packed = 0;
    dr_metalib_source_elements(&element_it, source);
    while((element = dr_metalib_source_element_next(&element_it))) {
        LPDRMETA meta;
        int entry_pos;

        if (dr_metalib_source_element_type(element) != dr_metalib_source_element_type_meta) continue;

        meta = dr_lib_find_meta_by_name(ctx->m_metalib, dr_metalib_source_element_name(element));
        if (meta == NULL) continue;

        if (dr_meta_align(meta) != curent_pack) {
            stream_printf(stream, "\n#pragma pack(1)\n");
            curent_pack = dr_meta_align(meta);
            packed = 1;
        }

        stream_printf(stream, "\nstruct %s {", dr_meta_name(meta));

        for(entry_pos = 0; entry_pos < dr_meta_entry_num(meta); ++entry_pos) {
            LPDRMETAENTRY entry = dr_meta_entry_at(meta, entry_pos);

            stream_printf(stream, "\n");
            stream_printf(stream, "    ");

            switch(dr_entry_type(entry)) {
            case CPE_DR_TYPE_UNION:
            case CPE_DR_TYPE_STRUCT: {
                LPDRMETA ref_meta;
                ref_meta = dr_entry_ref_meta(entry);
                if (ref_meta == NULL) continue;

                stream_printf(stream, "%s %s %s", dr_type_name(dr_entry_type(entry)), dr_meta_name(ref_meta), dr_entry_name(entry));
                break;
            }
            case CPE_DR_TYPE_STRING: {
                stream_printf(stream, "char %s[%d]", dr_entry_name(entry), dr_entry_size(entry));
                break;
            }
            default: {
                stream_printf(stream, "%s_t %s", dr_type_name(dr_entry_type(entry)), dr_entry_name(entry));
                break;
            }
            }

            if (dr_entry_array_count(entry) != 1) {
                stream_printf(stream, "[%d]", dr_entry_array_count(entry) < 1 ? 1 : dr_entry_array_count(entry));
            }

            stream_printf(stream, ";");
        }

        stream_printf(stream, "\n};\n");
    }

    if (packed) {
        stream_printf(stream, "\n#pragma pack()\n\n");
    }
}

int cpe_dr_generate_h(write_stream_t stream, dr_metalib_source_t source, cpe_dr_generate_ctx_t ctx) {
    const char * lib_name;

    assert(source);
    assert(stream);

    if ((lib_name = dr_metalib_source_libname(source))) {
        stream_printf(stream, "#ifndef DR_GENERATED_H_%s_%s_INCLEDED\n", lib_name, dr_metalib_source_name(source));
        stream_printf(stream, "#define DR_GENERATED_H_%s_%s_INCLEDED\n", lib_name, dr_metalib_source_name(source));
    }
    else {
        stream_printf(stream, "#ifndef DR_GENERATED_H_%s_INCLEDED\n", dr_metalib_source_name(source));
        stream_printf(stream, "#define DR_GENERATED_H_%s_INCLEDED\n", dr_metalib_source_name(source));
    }
    stream_printf(stream, "#include \"cpe/pal/pal_types.h\"\n");

    cpe_dr_generate_h_macros(stream, source, ctx);

    stream_printf(
        stream, 
        "\n"
        "#ifdef __cplusplus\n"
        "extern \"C\" {\n"
        "#endif\n");

    cpe_dr_generate_h_metas(stream, source, ctx);

    stream_printf(
        stream, 
        "#ifdef __cplusplus\n"
        "}\n"
        "#endif\n"
        "\n");

    stream_printf(stream, "#endif\n");

    return 0;
}
