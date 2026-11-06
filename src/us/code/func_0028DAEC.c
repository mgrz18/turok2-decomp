/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0023BC64.c, func_0023BC64.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern B09SlotA gPager_entries[];
extern B09LNode gPager_slots[];
extern B09SlotCtl gPager_expanded;
s32 VM_AcquireNextTLB(void) {
    s16 idx = gPager_expanded.cur++;
    if ((s16)gPager_expanded.cur >= 0x18) {
        gPager_expanded.cur = gPager_expanded.start;
    }
    if (gPager_entries[idx].a != 0xFF) {
        gPager_slots[gPager_entries[idx].a].tlb = 0xFF;
        gPager_entries[idx].a = 0xFF;
    }
    if (gPager_entries[idx].b != 0xFF) {
        gPager_slots[gPager_entries[idx].b].tlb = 0xFF;
        gPager_entries[idx].b = 0xFF;
    }
    return idx;
}
