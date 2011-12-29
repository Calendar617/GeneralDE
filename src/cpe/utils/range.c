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

int cpe_range_get_one(cpe_range_mgr_t ra) {
    struct cpe_range r;

    assert(ra);

    r = cpe_range_get_range(ra, 1);

    return r.m_start;
}

int cpe_range_put_one(cpe_range_mgr_t ra, int value) {
    return cpe_range_put_range(ra, value, value + 1);
}

struct cpe_range cpe_range_get_range(cpe_range_mgr_t ra, size_t capacity) {
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

static int cpe_range_reserve_range_buf(cpe_range_mgr_t ra) {
    size_t newCapacity;
    struct cpe_range * newBuf;

    if (ra->m_range_count + 1 < ra->m_range_capacity) return 0;

    newCapacity = ra->m_range_capacity + 256;
    newBuf = mem_alloc(ra->m_alloc, sizeof(struct cpe_range) * newCapacity);
    if (newBuf == NULL) return -1;

    if (ra->m_range_count > 0) {
        memcpy(newBuf, ra->m_ranges, sizeof(struct cpe_range) * ra->m_range_count);
    }

    if (ra->m_ranges) mem_free(ra->m_alloc, ra->m_ranges);

    ra->m_ranges = newBuf;
    ra->m_range_capacity = newCapacity;

    return 0;
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
        if (keepPos + 1 < ra->m_range_count) {
            memmove(
                ra->m_ranges + keepPos + 1,
                ra->m_ranges + keepPos + 1 + removeCount,
                sizeof(struct cpe_range) * (ra->m_range_count - (keepPos + 1)));
        }
    }
}

static int cpe_range_find_next_pos(cpe_range_mgr_t ra, int start) {
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

int cpe_range_put_range(cpe_range_mgr_t ra, int start, int end) {
    if (start < 0 || end <= 0 || end < start) return -1;
    if (end == start) return 0;

    assert(ra);

    if (ra->m_range_count == 0) {
        if (cpe_range_reserve_range_buf(ra) != 0) return -1;
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
                if (cpe_range_reserve_range_buf(ra) != 0) return -1;

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
                if (cpe_range_reserve_range_buf(ra) != 0) return -1;
                assert(ra->m_ranges);
                ra->m_ranges[ra->m_range_count].m_start = start;
                ra->m_ranges[ra->m_range_count].m_end = end;
                ++ra->m_range_count;
            }
        }
    }

    return 0;
}

cpe_range_t
cpe_range_find(cpe_range_mgr_t ra, int value) {
    int pos = cpe_range_find_next_pos(ra, value);
    if (pos < ra->m_range_count && ra->m_ranges[pos].m_start == value) {
        return &ra->m_ranges[pos];
    }

    if (pos > 0 && ra->m_ranges[pos - 1].m_end > value) {
        return &ra->m_ranges[pos - 1];
    }

    return NULL;
}
