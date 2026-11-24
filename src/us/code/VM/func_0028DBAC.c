/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0023BD24.c, func_0023BD24.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * globals/pager.h (round 29, part r2: Rage Wars' B09LNode is PageMapping (slot = id, id = index,
 * tlb = slot), B09SlotA PageEntry (key = id, a = team, b = slot), B09List gPager.chain). */

#include "common.h"
#include "structs/pager.h"
#include "globals/pager.h"
#include "protos/vm.h"

static __inline__ void m03_unlink(PageMapping *n) {
    if (n->prev != NULL) {
        n->prev->next = n->next;
    } else {
        gPager.chain.head = n->next;
        n->next->prev = NULL;
    }
    if (n->next != NULL) {
        n->next->prev = n->prev;
    } else {
        gPager.chain.tail = n->prev;
        n->prev->next = NULL;
    }
}
static __inline__ void m03_push(PageMapping *n) {
    n->next = NULL;
    n->prev = gPager.chain.tail;
    if (gPager.chain.tail != NULL) {
        gPager.chain.tail->next = n;
    }
    gPager.chain.tail = n;
    if (gPager.chain.head == NULL) {
        gPager.chain.head = n;
    }
}
/* Maps virtual page `page` to mapping n's frame: an even/odd TLB pair shares one entry, so the
 * entry of its twin page (pair: the region's page -> mapping table) is reused when it has one. */
void VM_MapTLBEntry(s16 page, PageMapping *n, s32 unused, u8 *pair) {
    u8 *p = pair;
    s32 idx;
    PageMapping *q;
    if (page & 1) {
        p = pair - 1;
    }
    if (p[0] != 0xFF && gPager_slots[p[0]].slot != 0xFF) {
        idx = gPager_slots[p[0]].slot;
        gPager_entries[(s16)idx].slot = n->index;
    } else if (p[1] != 0xFF && gPager_slots[p[1]].slot != 0xFF) {
        idx = gPager_slots[p[1]].slot;
        gPager_entries[(s16)idx].team = n->index;
    } else {
        idx = VM_AcquireNextTLB();
        if (page & 1) {
            gPager_entries[(s16)idx].slot = n->index;
            if (p[0] != 0xFF) {
                q = &gPager_slots[p[0]];
                gPager_entries[(s16)idx].team = q->index;
                q->slot = idx;
                m03_unlink(q);
                m03_push(q);
            }
        } else {
            gPager_entries[(s16)idx].team = n->index;
            if (p[1] != 0xFF) {
                q = &gPager_slots[p[1]];
                gPager_entries[(s16)idx].slot = q->index;
                q->slot = idx;
                m03_unlink(q);
                m03_push(q);
            }
        }
        gPager_entries[(s16)idx].id = page & 0xFFFE;
    }
    n->id = page;
    n->slot = idx;
    *pair = n->index;
    func_0028EE40((s16)idx, 0, (void *)(gPager_entries[(s16)idx].id << 12),
             gPager_entries[(s16)idx].team != 0xFF ? ((u32)gVMPages & 0x3FFFFFF) + (gPager_entries[(s16)idx].team << 12) : -1,
             gPager_entries[(s16)idx].slot != 0xFF ? ((u32)gVMPages & 0x3FFFFFF) + (gPager_entries[(s16)idx].slot << 12) : -1, 7);
}
