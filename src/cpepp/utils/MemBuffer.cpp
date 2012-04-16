#include "cpepp/utils/MemBuffer.hpp"

namespace Cpe { namespace Utils {

MemBuffer::MemBuffer(mem_allocrator_t allocrator) {
    mem_buffer_init(&m_buf, allocrator);
}

MemBuffer::~MemBuffer() {
    mem_buffer_clear(&m_buf);
}

MemBuffer::Trunk *
MemBuffer::TrunkIterator::next() {
    struct mem_buffer_trunk * r = m_next;
    if (m_next) {
        m_next = mem_buffer_trunk_next(m_next);
    }
    return (Trunk*)r;
}

}}
