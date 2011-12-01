#include <assert.h>
#include <string.h>
#include "gd/cfg/cfg_manage.h"
#include "cfg_internal_ops.h"

int gd_cfg_seq_count(gd_cfg_t cfg) {
    return ((struct gd_cfg_seq *)cfg)->m_count;
}

gd_cfg_t gd_cfg_seq_at(gd_cfg_t cfg, int pos) {
    struct gd_cfg_seq_block * block;
    struct gd_cfg_seq * s;

    assert(cfg);
    if (cfg->m_type != GD_CFG_TYPE_SEQUENCE) return NULL;
    if (pos < 0) return NULL;

    s = (struct gd_cfg_seq *)cfg;

    block = &s->m_block_head;
    while(pos >= GD_CFG_SEQ_BLOCK_ITEM_COUNT) {
        pos -= GD_CFG_SEQ_BLOCK_ITEM_COUNT;
        block = block->m_next;
        if (block == NULL) return NULL;
    }

    assert(block);
    assert(pos < GD_CFG_SEQ_BLOCK_ITEM_COUNT);

    return block->m_items[pos];
}

gd_cfg_t gd_cfg_seq_item_create(struct gd_cfg_seq * s, int type, size_t capacity) {
    gd_cfg_t rv;
    int insertIdx;
    struct gd_cfg_seq_block * head;
    struct gd_cfg_seq_block ** insertBlocl;

    assert(s);
    assert(s->m_manage);

    insertIdx = s->m_count;
    head = &s->m_block_head;
    insertBlocl = &head;

    while(insertIdx >= GD_CFG_SEQ_BLOCK_ITEM_COUNT) {
        insertIdx -= GD_CFG_SEQ_BLOCK_ITEM_COUNT;

        assert(*insertBlocl);
        insertBlocl = &(*insertBlocl)->m_next;
        if (*insertBlocl == NULL) {
            *insertBlocl = (struct gd_cfg_seq_block *)
                mem_alloc(s->m_manage->m_alloc, sizeof(struct gd_cfg_seq_block));
            if (*insertBlocl == NULL) return NULL;
            (*insertBlocl)->m_next = NULL;
        }
    }

    assert(*insertBlocl);
    assert(insertIdx < GD_CFG_SEQ_BLOCK_ITEM_COUNT);

    rv = (gd_cfg_t)mem_alloc(s->m_manage->m_alloc, sizeof(struct gd_cfg) + capacity);
    if (rv == NULL) return NULL;

    rv->m_manage = s->m_manage;
    rv->m_type = type;
    rv->m_parent = (gd_cfg_t)s;

    (*insertBlocl)->m_items[insertIdx] = rv;

    return rv;
}

int gd_cfg_seq_find_in_block(struct gd_cfg_seq_block * b, int count, gd_cfg_t cfg) {
    int i;

    for(i = 0; i < count; ++i) {
        if (b->m_items[i] == cfg) {
            return i;
        }
    }

    return -1;
}

void gd_cfg_seq_item_delete(struct gd_cfg_seq * s, gd_cfg_t cfg) {
    struct gd_cfg_seq_block * b;
    int leftCount;
    int posInB;

    /*find position*/
    b = &s->m_block_head;
    leftCount = s->m_count;
    posInB = -1;

    while(leftCount && b) {
        int countInB = 
            leftCount > GD_CFG_SEQ_BLOCK_ITEM_COUNT
            ? GD_CFG_SEQ_BLOCK_ITEM_COUNT
            : leftCount;

        posInB = gd_cfg_seq_find_in_block(b, countInB, cfg);
        if (posInB < 0) {
            leftCount -= countInB;
            b = b->m_next;
        }
        else {
            leftCount -= (posInB + 1);
            break;
        }
    }

    if (posInB < 0) return; //not found

    /*free cfg*/
    gd_cfg_fini(cfg);
    mem_free(s->m_manage->m_alloc, cfg);
    cfg = NULL;

    /*move next here*/
    while(leftCount && b) {
        int leftCountInB = 
            (leftCount + posInB + 1) > GD_CFG_SEQ_BLOCK_ITEM_COUNT
            ? GD_CFG_SEQ_BLOCK_ITEM_COUNT - (posInB + 1)
            : leftCount;

        if (leftCountInB > 0) {
            memmove(&b->m_items[posInB], b->m_items[posInB + 1], sizeof(gd_cfg_t) * leftCountInB);
            leftCount -= leftCountInB;
            posInB = 0;
        }

        if (leftCount && b->m_next) {
            b->m_items[GD_CFG_SEQ_BLOCK_ITEM_COUNT - 1] = b->m_next->m_items[0];
            leftCount -= 1;
        }

        b = b->m_next;
    }

    --s->m_count;
}

void gd_cfg_seq_init(struct gd_cfg_seq * s) {
    s->m_count = 0;
    s->m_block_head.m_next = NULL;
}

static void gd_cfg_seq_fini_block(mem_allocrator_t alloc, struct gd_cfg_seq_block * b, int count) {
    int i;

    for(i = 0; i < count; ++i) {
        gd_cfg_fini(b->m_items[i]);
        mem_free(alloc, b->m_items[i]);
    }
}

void gd_cfg_seq_fini(struct gd_cfg_seq * s) {
    struct gd_cfg_seq_block * b;
    int blockFreeSize;
    
    b = &s->m_block_head;
    while(b) {
        struct gd_cfg_seq_block * n = b->m_next;

        blockFreeSize = 
            s->m_count > GD_CFG_SEQ_BLOCK_ITEM_COUNT
            ? GD_CFG_SEQ_BLOCK_ITEM_COUNT
            : s->m_count;

        gd_cfg_seq_fini_block(s->m_manage->m_alloc, b, blockFreeSize);

        s->m_count -= blockFreeSize;

        if (b != &s->m_block_head) {
            mem_free(s->m_manage->m_alloc, b);
        }

        b = n;
    }

    s->m_count = 0;
    s->m_block_head.m_next = NULL;
}
