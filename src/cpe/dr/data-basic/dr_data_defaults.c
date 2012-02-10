#include <assert.h>
#include <string.h>
#include "cpe/pal/pal_strings.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"

int dr_entry_set_defaults(void * inout, LPDRMETAENTRY entry, int policy) {
    const void * defaultValue = dr_entry_dft_value(entry);
    if (defaultValue) {
        memcpy(inout, defaultValue, entry->m_unitsize);
    }
    else {
        if (! (policy & DR_SET_DEFAULTS_POLICY_NO_DEFAULT_IGNORE)) {
            bzero(inout, entry->m_unitsize);
        }
    }

    return 0;
}

struct SProcessStack{
    LPDRMETA m_meta;
    LPDRMETAENTRY m_entry;
    int m_entry_pos;
    int m_array_pos;
    char * m_data;
};

void dr_meta_set_defaults(void * inout, LPDRMETA meta, int policy) {
    struct SProcessStack processStack[CPE_DR_MAX_LEVEL];
    int stackPos;

    assert(inout);

    processStack[0].m_meta = meta;
    processStack[0].m_entry = dr_meta_entry_at(meta, 0);
    processStack[0].m_entry_pos = 0;
    processStack[0].m_array_pos = 0;
    processStack[0].m_data = (char *)inout;

    for(stackPos = 0; stackPos >= 0;) {
        struct SProcessStack * curStack;

        assert(stackPos < CPE_DR_MAX_LEVEL);

        curStack = &processStack[stackPos];
        if (curStack->m_meta == NULL) {
            --stackPos;
            continue;
        }

        for(; curStack->m_entry_pos < curStack->m_meta->m_entry_count;
            ++curStack->m_entry_pos, curStack->m_entry = dr_meta_entry_at(curStack->m_meta, curStack->m_entry_pos)
            )
        {
            size_t elementSize;
        LOOPENTRY:

            if (curStack->m_entry == 0) continue;

            elementSize = dr_entry_element_size(curStack->m_entry);
            if (elementSize == 0) continue;

            for(; curStack->m_array_pos < curStack->m_entry->m_array_count; ++curStack->m_array_pos) {
                char * entryData = curStack->m_data + curStack->m_entry->m_data_start_pos + (elementSize * curStack->m_array_pos);

                if (curStack->m_entry->m_type <= CPE_DR_TYPE_COMPOSITE) {
                    if (stackPos + 1 < CPE_DR_MAX_LEVEL) {
                        struct SProcessStack * nextStack;
                        nextStack = &processStack[stackPos + 1];

                        nextStack->m_meta = dr_entry_ref_meta(curStack->m_entry);
                        if (nextStack->m_meta == 0) break;

                        nextStack->m_entry = dr_meta_entry_at(nextStack->m_meta, 0);
                        nextStack->m_data = entryData;
                        nextStack->m_entry_pos = 0;
                        nextStack->m_array_pos = 0;

                        ++curStack->m_array_pos;
                        ++stackPos;
                        curStack = nextStack;
                        goto LOOPENTRY;
                    }
                }
                else {
                    dr_entry_set_defaults(entryData, curStack->m_entry, policy);
                }
            }
        }

        --stackPos;
    }
}
