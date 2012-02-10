#include <assert.h>
#include <string.h>
#include "cpe/pal/pal_strings.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"

struct CopySameEntryProcessStack{
    LPDRMETA m_des_meta;
    int m_des_entry_pos;
    char * m_des_data;

    LPDRMETA m_src_meta;
    char const * m_src_data;
};

int dr_meta_copy_check_type_copyable(int desType, int srcType) {
    if (desType <= CPE_DR_TYPE_COMPOSITE) {
        return srcType == desType ? 1 : 0;
    }
    else {
        return srcType > CPE_DR_TYPE_COMPOSITE ? 1 : 0;
    }
}

void dr_meta_copy_same_entry(void * desData, LPDRMETA desMeta, void const * srcData, LPDRMETA srcMeta, int policy, error_monitor_t em) {
    struct CopySameEntryProcessStack processStack[CPE_DR_MAX_LEVEL];
    int stackPos;

    assert(desData);
    assert(desMeta);
    assert(srcData);
    assert(srcMeta);

    processStack[0].m_des_meta = desMeta;
    processStack[0].m_des_entry_pos = 0;
    processStack[0].m_des_data = (char*)desData;
    processStack[0].m_src_meta = srcMeta;
    processStack[0].m_src_data = (char const *)srcData;

    for(stackPos = 0; stackPos >= 0;) {
        LPDRMETA curDesMeta;
        int curDesEntryPos;
        char * curDesData;
        LPDRMETA curSrcMeta;
        const char * curSrcData;

        assert(stackPos < CPE_DR_MAX_LEVEL);

        curDesMeta = processStack[stackPos].m_des_meta;
        curDesEntryPos = processStack[stackPos].m_des_entry_pos;
        curDesData = processStack[stackPos].m_des_data;
        curSrcMeta = processStack[stackPos].m_src_meta;
        curSrcData = processStack[stackPos].m_src_data;

        if (curDesMeta == NULL) {
            --stackPos;
            continue;
        }

        for(; curDesEntryPos < curDesMeta->m_entry_count; ++curDesEntryPos) {
            LPDRMETAENTRY curDesEntry;
            char * curDesEntryData;

            LPDRMETAENTRY curSrcEntry;
            const char * curSrcEntryData;

            curDesEntry = dr_meta_entry_at(curDesMeta, curDesEntryPos);
            curDesEntryData = curDesData + curDesEntry->m_data_start_pos;

            curSrcEntry = dr_meta_find_entry_by_name(curSrcMeta, dr_entry_name(curDesEntry));
            if (curSrcEntry == 0) continue;

            if (!dr_meta_copy_check_type_copyable(curDesEntry->m_type, curSrcEntry->m_type)) continue;

            if (curDesEntry->m_type <= CPE_DR_TYPE_COMPOSITE) {
                if (stackPos + 1 < CPE_DR_MAX_LEVEL) {
                    processStack[stackPos + 1].m_des_meta = dr_entry_ref_meta(curDesEntry);
                    processStack[stackPos + 1].m_des_data = curDesEntryData;
                    processStack[stackPos + 1].m_des_entry_pos = 0;
                    processStack[stackPos + 1].m_src_meta = dr_entry_ref_meta(curSrcEntry);
                    processStack[stackPos + 1].m_src_data = curSrcEntryData;
                    break;
                }
            }
            else {
                dr_entry_set_from_ctype(
                    curDesEntryData,
                    curSrcEntryData,
                    curSrcEntry->m_type,
                    curDesEntry,
                    em);
            }
        }

        if (curDesEntryPos >= curDesMeta->m_entry_count) {
            --stackPos;
            if (stackPos >= 0) {
                processStack[stackPos].m_des_entry_pos++;
            }
        }
        else {
            ++stackPos;
        }
    }
}
