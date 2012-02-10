#include <assert.h>
#include <string.h>
#include "cpe/pal/pal_strings.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_ctypes_op.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"

struct CopySameEntryProcessStack{
    LPDRMETA m_des_meta;
    LPDRMETAENTRY m_des_entry;
    int m_des_entry_pos;
    char * m_des_data;

    LPDRMETA m_src_meta;
    char const * m_src_data;

    int m_array_pos;
};

int dr_meta_copy_check_type_copyable(int desType, int srcType) {
    if (desType <= CPE_DR_TYPE_COMPOSITE) {
        return srcType == desType ? 1 : 0;
    }
    else {
        return srcType > CPE_DR_TYPE_COMPOSITE ? 1 : 0;
    }
}

int dr_meta_copy_check_array_copyable(int desArrayCount, int srcArrayCount) {
    return (desArrayCount == 1 && srcArrayCount == 1)
        || (desArrayCount != 1 && srcArrayCount != 1)
        ? 1
        : 0;
}

void dr_meta_copy_same_entry(
    void * desData, size_t desCapacity, LPDRMETA desMeta,
    void const * srcData, size_t srcCapacity, LPDRMETA srcMeta, 
    int policy, error_monitor_t em)
{
    struct CopySameEntryProcessStack processStack[CPE_DR_MAX_LEVEL];
    int stackPos;

    assert(desData);
    assert(desMeta);
    assert(srcData);
    assert(srcMeta);

    processStack[0].m_des_meta = desMeta;
    processStack[0].m_des_entry = dr_meta_entry_at(desMeta, 0);
    processStack[0].m_des_entry_pos = 0;
    processStack[0].m_array_pos = 0;
    processStack[0].m_des_data = (char *)desData;
    processStack[0].m_src_meta = srcMeta;
    processStack[0].m_src_data = (const char *)srcData;
    

    for(stackPos = 0; stackPos >= 0;) {
        struct CopySameEntryProcessStack * curStack;

        assert(stackPos < CPE_DR_MAX_LEVEL);

        curStack = &processStack[stackPos];
        if (curStack->m_des_meta == NULL) {
            --stackPos;
            continue;
        }

        for(; curStack->m_des_entry_pos < curStack->m_des_meta->m_entry_count
                && curStack->m_des_entry
                ; ++curStack->m_des_entry_pos
                , curStack->m_array_pos = 0
                , curStack->m_des_entry = dr_meta_entry_at(curStack->m_des_meta, curStack->m_des_entry_pos)
            )
        {
            size_t desElementSize, srcElementSize;
            LPDRMETAENTRY srcEntry;
            size_t desMaxArrayCount, srcMaxArrayCount;

        LOOPENTRY:
            desElementSize = dr_entry_element_size(curStack->m_des_entry);
            if (desElementSize == 0) continue;

            srcEntry = dr_meta_find_entry_by_name(curStack->m_src_meta, dr_entry_name(curStack->m_des_entry));
            if (srcEntry == 0) continue;

            srcElementSize = dr_entry_element_size(srcEntry);
            if (srcElementSize == 0) continue;

            if (!dr_meta_copy_check_type_copyable(curStack->m_des_entry->m_type, srcEntry->m_type)) continue;

            if (!dr_meta_copy_check_array_copyable(curStack->m_des_entry->m_array_count, srcEntry->m_array_count)) continue;

            desMaxArrayCount = curStack->m_des_entry->m_array_count;
            srcMaxArrayCount = srcEntry->m_array_count;
            if (srcMaxArrayCount != 1) {
                LPDRMETAENTRY srcRefer = dr_entry_array_refer_entry(srcEntry);
                if (srcRefer) {
                    uint64_t readBuf;
                    if (dr_ctype_try_read_uint64(
                            &readBuf, 
                            curStack->m_src_data + srcEntry->m_array_refer_data_start_pos,
                            srcEntry->m_type,
                            0) == 0)
                    {
                        srcMaxArrayCount = readBuf;
                    }
                }
            }

            for(; curStack->m_array_pos < desMaxArrayCount && curStack->m_array_pos < srcMaxArrayCount
                    ; ++curStack->m_array_pos)
            {
                char * desEntryData = curStack->m_des_data + curStack->m_des_entry->m_data_start_pos + (desElementSize * curStack->m_array_pos);
                const char * srcEntryData = curStack->m_src_data + srcEntry->m_data_start_pos + (srcElementSize * curStack->m_array_pos);

                if (curStack->m_des_entry->m_type <= CPE_DR_TYPE_COMPOSITE) {
                    if (stackPos + 1 < CPE_DR_MAX_LEVEL) {
                        struct CopySameEntryProcessStack * nextStack;
                        nextStack = &processStack[stackPos + 1];

                        nextStack->m_des_meta = dr_entry_ref_meta(curStack->m_des_entry);
                        if (nextStack->m_des_meta == 0) break;
                        nextStack->m_des_entry = dr_meta_entry_at(nextStack->m_des_meta, 0);
                        nextStack->m_des_data = desEntryData;
                        nextStack->m_des_entry_pos = 0;
                        nextStack->m_array_pos = 0;

                        nextStack->m_src_meta = dr_entry_ref_meta(srcEntry);
                        if (nextStack->m_src_meta == 0) break;
                        nextStack->m_src_data = srcEntryData;

                        ++curStack->m_array_pos;
                        ++stackPos;
                        curStack = nextStack;
                        goto LOOPENTRY;
                    }
                }
                else {
                    dr_entry_set_from_ctype(desEntryData, srcEntryData, srcEntry->m_type, curStack->m_des_entry, 0);
                }
            }

            if (curStack->m_des_entry->m_array_count != 1) {
                LPDRMETAENTRY refer = dr_entry_array_refer_entry(curStack->m_des_entry);
                if (refer) {
                    dr_entry_set_from_int32(
                        curStack->m_des_data + curStack->m_des_entry->m_array_refer_data_start_pos,
                        curStack->m_array_pos,
                        refer,
                        0);
                }
            }
        }

        --stackPos;
    }
}
