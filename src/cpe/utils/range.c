#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "cpe/utils/range.h"

int cpe_range_mgr_init(cpe_range_mgr_t ra, mem_allocrator_t alloc) {
    if (ra == NULL) return 0;

    ra->m_alloc = alloc;
    ra->m_range_capacity = 0;
    ra->m_range_count = 0;
    ra->m_ranges = NULL;

    return 0;
}

void cpe_range_mgr_fini(cpe_range_mgr_t ra) {
    assert(ra);

    if (ra->m_ranges) {
        mem_free(ra->m_alloc, ra->m_ranges);
        ra->m_ranges = NULL;
    }

    ra->m_range_count = 0;
    ra->m_range_capacity = 0;
    ra->m_alloc = NULL;
}

void cpe_range_mgr_clear(cpe_range_mgr_t ra) {
    assert(ra);
    ra->m_range_capacity = 0;
}

int cpe_range_mgr_is_empty(cpe_range_mgr_t ra) {
    return ra->m_range_capacity == 0 ? 1 : 0;
}

void cpe_range_mgr_dump(write_stream_t stream, cpe_range_mgr_t ra) {
    int i;
    for(i = 0; i < ra->m_range_count; ++i) {
        if (i > 0) stream_putc(stream, ',');

        stream_printf(
            stream, "[%d~%d)",
            ra->m_ranges[i].m_start,
            ra->m_ranges[i].m_end);
    }
}

ptr_int_t cpe_range_get_one(cpe_range_mgr_t ra) {
    struct cpe_range r;

    assert(ra);

    r = cpe_range_get_range(ra, 1);

    return r.m_start;
}

int cpe_range_put_one(cpe_range_mgr_t ra, ptr_int_t value) {
    return cpe_range_put_range(ra, value, value + 1);
}

struct cpe_range
cpe_range_get_range(cpe_range_mgr_t ra, size_t capacity) {
    struct cpe_range r;

    assert(ra);

    if (ra->m_range_count == 0 || capacity <= 0) {
        r.m_start = -1;
        r.m_end = -1;
    }
    else {
        struct cpe_range * allocFrom = ra->m_ranges;
        assert(allocFrom);

        r.m_start = allocFrom->m_start;
        if (allocFrom->m_end > allocFrom->m_start + capacity) {
            r.m_end = r.m_start + capacity;
            allocFrom->m_start += capacity;
        }
        else {
            r.m_end = allocFrom->m_end;
            --ra->m_range_count;
            if (ra->m_range_count > 0) {
                memmove(ra->m_ranges, ra->m_ranges + 1, sizeof(struct cpe_range) * ra->m_range_count);
            }
        }
    }

    return r;
}

static void cpe_range_merge_neighbers(cpe_range_mgr_t ra, int beginPos) {
    int keepPos;
    int removeCount;
    int checkPos;
    struct cpe_range * curRange;

    keepPos = beginPos;
    removeCount = 0;

    curRange = ra->m_ranges + beginPos;

    if (beginPos > 0) {
        struct cpe_range * preRange = curRange - 1;
        if (preRange->m_end >= curRange->m_start) {
            preRange->m_end = curRange->m_end;
            curRange = preRange;
            ++removeCount;
            --keepPos;
        }
    }

    for(checkPos = beginPos + 1; checkPos < ra->m_range_count; ++checkPos, ++removeCount) {
        struct cpe_range * nextRange = ra->m_ranges + checkPos;
        if (nextRange->m_start <= curRange->m_end) {
            if (nextRange->m_end > curRange->m_end) {
                curRange->m_end = nextRange->m_end;
            }
        }
        else {
            break;
        }
    }

    if (removeCount > 0) {
        ra->m_range_count -= removeCount;
        if ((size_t(keepPos + 1) < ra->m_range_count) {
            memmove(
                ra->m_ranges + keepPos + 1,
                ra->m_ranges + keepPos + 1 + removeCount,
                sizeof(struct cpe_range) * (ra->m_range_count - (keepPos + 1)));
        }
    }
}

static int cpe_range_find_next_pos(cpe_range_mgr_t ra, ptr_int_t start) {
    struct cpe_range * curRange;
    int beginPos, endPos, curPos;

    for(beginPos = 0, endPos = ra->m_range_count, curPos = (endPos - beginPos - 1) / 2;
        beginPos < endPos;
        curPos = beginPos + (endPos - beginPos - 1) / 2)
    {
        curRange = ra->m_ranges + curPos;
            
        if (curRange->m_start < start) {
            beginPos = curPos + 1;
        }
        else if (curRange->m_start > start) {
            endPos = curPos;
        }
        else {
            break;
        }
    }

    return (curPos < ra->m_range_count) && (ra->m_ranges[curPos].m_start >= start)
        ? curPos
        : ra->m_range_count;
}

int cpe_range_put_range(cpe_range_mgr_t ra, ptr_int_t start, ptr_int_t end) {
    if (start < 0 || end <= 0 || end < start) return -1;
    if (end == start) return 0;

    assert(ra);

    if (ra->m_range_count == 0) {
        if (cpe_range_mgr_reserve_for_put(ra, 1) != 0) return -1;
        assert(ra->m_ranges);
        ra->m_ranges[0].m_start = start;
        ra->m_ranges[0].m_end = end;
        ++ra->m_range_count;
        return 0;
    }
    else {
        int insertPos = cpe_range_find_next_pos(ra, start);

        if (insertPos < ra->m_range_count) {
            struct cpe_range * curRange = ra->m_ranges + insertPos;

            assert(curRange->m_start >= start);

            if (end >= curRange->m_start) {
                curRange->m_start = start;

                if (end > curRange->m_end) {
                    curRange->m_end = end;
                }

                cpe_range_merge_neighbers(ra, insertPos);
            }
            else if (insertPos > 0 && ra->m_ranges[insertPos - 1].m_end >= start) {
                ra->m_ranges[insertPos - 1].m_end = end;
                cpe_range_merge_neighbers(ra, insertPos - 1);
            }
            else {
                if (cpe_range_mgr_reserve_for_put(ra, 1) != 0) return -1;

                memmove(
                    ra->m_ranges + insertPos + 1,
                    ra->m_ranges + insertPos,
                    sizeof(struct cpe_range) * (ra->m_range_count - insertPos));

                curRange->m_start = start;
                curRange->m_end = end;

                ++ra->m_range_count;
            }
        }
        else {
            if (ra->m_ranges[ra->m_range_count - 1].m_end >= start) {
                ra->m_ranges[ra->m_range_count - 1].m_end = end;
            }
            else {
                if (cpe_range_mgr_reserve_for_put(ra, 1) != 0) return -1;
                assert(ra->m_ranges);
                ra->m_ranges[ra->m_range_count].m_start = start;
                ra->m_ranges[ra->m_range_count].m_end = end;
                ++ra->m_range_count;
            }
        }
    }

    return 0;
}

struct cpe_range
cpe_range_find(cpe_range_mgr_t ra, ptr_int_t value) {
    int pos = cpe_range_find_next_pos(ra, value);
    if (pos < ra->m_range_count && ra->m_ranges[pos].m_start == value) {
        return ra->m_ranges[pos];
    }

    if (pos > 0 && ra->m_ranges[pos - 1].m_end > value) {
        return ra->m_ranges[pos - 1];
    }

    return cpe_range_invalid;
}

int cpe_range_mgr_reserve_for_put(cpe_range_mgr_t ra, int put_count) {
    size_t newCapacity;
    size_t requireCount;
    struct cpe_range * newBuf;

    requireCount = ra->m_range_count + put_count;
    newCapacity = ra->m_range_capacity;

    while(requireCount >= newCapacity) {
        newCapacity =
            newCapacity == 0
            ? 256
            : newCapacity << 1;
    }

    if (newCapacity != ra->m_range_capacity) {
        newBuf = (struct cpe_range*)mem_alloc(ra->m_alloc, sizeof(struct cpe_range) * newCapacity);
        if (newBuf == NULL) return -1;

        if (ra->m_range_count > 0) {
            memcpy(newBuf, ra->m_ranges, sizeof(struct cpe_range) * ra->m_range_count);
        }

        if (ra->m_ranges) mem_free(ra->m_alloc, ra->m_ranges);

        ra->m_ranges = newBuf;
        ra->m_range_capacity = newCapacity;
    }

    return 0;
}

int cpe_range_is_valid(struct cpe_range r) {
    return r.m_start >= 0 && r.m_end >= r.m_start
        ? 1
        : 0;
}

int cpe_range_size(struct cpe_range r) {
    return cpe_range_is_valid(r)
        ? (r.m_end - r.m_start)
        : -1;
}

void cpe_range_mgr_ranges(cpe_range_it_t it, cpe_range_mgr_t ra) {
    assert(it);
    assert(ra);

    it->m_mgr = ra;
    it->m_pos = 0;
}

struct cpe_range cpe_range_it_next(cpe_range_it_t it) {
    assert(it);
    if (it->m_mgr == NULL || it->m_mgr->m_ranges == NULL) return cpe_range_invalid;

    if (it->m_pos < it->m_mgr->m_range_count) {
        return it->m_mgr->m_ranges[it->m_pos++];
    }
    else {
        return cpe_range_invalid;
    }
}

struct cpe_range cpe_range_invalid = { -1, -1 };
