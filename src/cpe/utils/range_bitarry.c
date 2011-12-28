#include <assert.h>
#include "cpe/utils/range_bitarry.h"

int cpe_range_free_from_bitarray(
    cpe_range_allocrator_t ra,
    cpe_ba_t ba,
    int32_t baStartPos,
    size_t ba_capacity)
{
    int32_t start;
    int32_t end;
    int i;

    start = end = -1;

    for(i = 0; i < ba_capacity; ++i, ++baStartPos) {
        if (cpe_ba_get(ba, i) == cpe_ba_false) continue;

        if (end < 0) {
            start = baStartPos;
            end = baStartPos + 1;
        }
        else {
            if (end == baStartPos) {
                ++end;
            }
            else {
                if (cpe_range_free_range(ra, start, end) != 0) return -1;

                start = baStartPos;
                end = baStartPos + 1;
            }
        }
    }

    if (!(end < 0)) {
        if (cpe_range_free_range(ra, start, end) != 0) return -1;
    }

    return 0;
}
