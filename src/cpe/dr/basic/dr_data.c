#include <assert.h>
#include <string.h>
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"

int dr_entry_set_defaults(void * inout, LPDRMETAENTRY entry) {
    const void * defaultValue = dr_entry_dft_value(entry);
    if (defaultValue) {
        memcpy(inout, defaultValue, entry->m_unitsize);
    }
    else {
        bzero(inout, entry->m_unitsize);
    }

    return 0;
}

void dr_meta_set_defaults(void * inout, LPDRMETA meta) {
    assert(inout);

    struct {
        LPDRMETA m_meta;
        int m_entry_pos;
        void * m_data;
    } processStack[CPE_DR_MAX_LEVEL];
    int stackPos;

    bzero(inout, dr_meta_size(meta));

    processStack[0].m_meta = meta;
    processStack[0].m_entry_pos = 0;
    processStack[0].m_data = inout;

    for(stackPos = 0; stackPos >= 0;) {
        assert(stackPos < CPE_DR_MAX_LEVEL);

        LPDRMETA curMeta = processStack[stackPos].m_meta;
        int curEntryPos = processStack[stackPos].m_entry_pos;
        void * data = processStack[stackPos].m_data;

        if (curMeta == NULL) {
            --stackPos;
            continue;
        }

        for(; curEntryPos < curMeta->m_entry_count; ++curEntryPos) {
            LPDRMETAENTRY entry = dr_meta_entry_at(curMeta, curEntryPos);
            void * entryData = ((char *)data) + entry->m_data_start_pos;

            if (entry->m_type <= CPE_DR_TYPE_COMPOSITE) {
                if (stackPos + 1 < CPE_DR_MAX_LEVEL) {
                    processStack[stackPos + 1].m_meta = dr_entry_ref_meta(entry);
                    processStack[stackPos + 1].m_data = entryData;
                    processStack[stackPos + 1].m_entry_pos =0;
                    break;
                }
            }
            else {
                const void * defaultValue = dr_entry_dft_value(entry);
                if (defaultValue) {
                    memcpy(entryData , defaultValue, entry->m_unitsize);
                }
            }
        }

        if (curEntryPos >= curMeta->m_entry_count) {
            --stackPos;
        }
        else {
            curMeta->m_entry_count = curEntryPos + 1;
            ++stackPos;
        }
    }
}
