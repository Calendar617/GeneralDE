#include "cpe/utils/buffer.h"
#include "cpe/utils/stream_mem.h"
#include "cpe/utils/stream_buffer.h"
#include "cpe/cfg/tests-env/with_cfg.hpp"

namespace cpe { namespace cfg { namespace testenv {

cfg_t with_cfg::t_cfg_create(void) {
    cfg_t cfg = cfg_create(t_tmp_allocrator());
    EXPECT_TRUE(cfg) << "t_cfg_create: create cfg fail!";
    return cfg;
}

cfg_t with_cfg::t_cfg_parse(const char * input) {
    if (input == 0) return 0;

    cfg_t cfg = cfg_create(t_tmp_allocrator());
    EXPECT_TRUE(cfg) << "t_cfg_parse: create cfg fail!";
    if (cfg == 0) return 0;

    read_stream_mem inputStream = CPE_READ_STREAM_MEM_INITIALIZER(input, strlen(input));
    EXPECT_EQ(
        0,
        cfg_read_with_name(cfg, "", (read_stream_t)&inputStream, cfg_merge_use_new, 0))
        << "parse cfg fail!\ninput:\n" << input;

    return cfg_struct_find_cfg(cfg, "");
}

const char * with_cfg::t_cfg_dump(cfg_t cfg, int ident, int level_ident) {
    struct mem_buffer buffer;
    mem_buffer_init(&buffer, t_tmp_allocrator());

    write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(&buffer);

    cfg_dump(cfg, (write_stream_t)&stream, ident, level_ident);

    stream_putc((write_stream_t)&stream, 0);

    return (const char *)mem_buffer_make_continuous(&buffer, 0);
}

const char * with_cfg::t_cfg_dump_inline(cfg_t cfg) {
    struct mem_buffer buffer;
    mem_buffer_init(&buffer, t_tmp_allocrator());

    write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(&buffer);

    cfg_dump_inline(cfg, (write_stream_t)&stream);

    stream_putc((write_stream_t)&stream, 0);

    return (const char *)mem_buffer_make_continuous(&buffer, 0);
}

}}}
