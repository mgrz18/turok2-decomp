/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0023BC64.c, func_0023BC64.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * globals/pager.h (round 29, part r2: Rage Wars' B09SlotA is PageEntry (a = team, b = slot), B09LNode
 * PageMapping (tlb = slot), B09SlotCtl gPager.expanded/mapped). */

#include "common.h"
#include "structs/pager.h"
#include "globals/pager.h"

/* The next TLB slot to use, round-robin from gPager.mapped; the pages it held lose it. */
s32 VM_AcquireNextTLB(void) {
    s16 *cursor = &gPager_expanded; /* gPager.expanded, then gPager.mapped */
    s16 idx = cursor[0]++;
    if (cursor[0] >= 0x18) {
        cursor[0] = cursor[1];
    }
    if (gPager_entries[idx].team != 0xFF) {
        gPager_slots[gPager_entries[idx].team].slot = 0xFF;
        gPager_entries[idx].team = 0xFF;
    }
    if (gPager_entries[idx].slot != 0xFF) {
        gPager_slots[gPager_entries[idx].slot].slot = 0xFF;
        gPager_entries[idx].slot = 0xFF;
    }
    return idx;
}
