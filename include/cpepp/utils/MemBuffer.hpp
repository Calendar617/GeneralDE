#ifndef CPEPP_UTILS_MEMBUFFER_H
#define CPEPP_UTILS_MEMBUFFER_H
#include "cpe/utils/buffer.h"
#include "ClassCategory.hpp"

namespace Cpe { namespace Utils {

class MemBuffer : public Noncopyable {
public:
    explicit MemBuffer(mem_allocrator_t allocrator);
    ~MemBuffer();

    struct Trunk : public SimulateObject {
        operator struct mem_buffer_trunk * (void) const { return (struct mem_buffer_trunk *)this; }

        void * data(void) { return mem_trunk_data(*this); }
        size_t capacity(void) const { return mem_trunk_capacity(*this); }
        size_t size(void) const { return mem_trunk_size(*this); } 
    };

    class TrunkIterator {
    public:
        TrunkIterator() : m_next(NULL) {}
        TrunkIterator(struct mem_buffer_trunk * trunk) : m_next(trunk) {}
        TrunkIterator(TrunkIterator const & o) : m_next(o.m_next) {}

        TrunkIterator & operator=(TrunkIterator const & o) { m_next = o.m_next; return *this; }
        TrunkIterator & operator=(struct mem_buffer_trunk * trunk) { m_next = trunk; return *this; }

        Trunk * next();
    private:
        struct mem_buffer_trunk * m_next;
    };

    operator mem_buffer_t () { return &m_buf; }

    void * make_continuous(size_t reserve) { return mem_buffer_make_continuous(&m_buf, reserve); }
    void * make_exactly(void) { return mem_buffer_make_exactly(&m_buf); }

    void * alloc(size_t len) { return mem_buffer_alloc(&m_buf, len); }

    ssize_t append(const void * data, size_t len) { return mem_buffer_append(&m_buf, data, len); }
    ssize_t append(char data) { return mem_buffer_append_char(&m_buf, data); }
    ssize_t read(char * buf, size_t capacity) { return mem_buffer_read(buf, capacity, &m_buf); }

    char * strdup(const char * d) { return mem_buffer_strdup(&m_buf, d); }
    char * strdup(const char * d, size_t len) { return mem_buffer_strdup_len(&m_buf, d, len); }
    void strcat(const char * d);

    TrunkIterator trunks(void) { return TrunkIterator(mem_buffer_trunk_first(&m_buf)); }

private:
    struct mem_buffer m_buf;
};

}}

#endif
