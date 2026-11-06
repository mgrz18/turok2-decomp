/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_12.c, func_8023C084_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pager.h"

extern void func_0028EEC0(int arg0);
void VM_UnmapTLBEntry(Mapping *mapping) {
    extern void func_0028EE40(s32, s32, s32, s32, s32, s32);
    s32 slot;
    if (mapping->slot == 0xFF) {
        return;
    }
    if (mapping->flags & 1) {
        gPager_entries[mapping->slot].slot = 0xFF;
        slot = mapping->slot;
        if (gPager_entries[slot].team == 0xFF) {
            goto unmap;
        }
        func_0028EE40(slot, 0, gPager_entries[slot].id << 12,
                      ((s32)&gVMPages & 0x3FFFFFF) + (gPager_entries[slot].team << 12), -1, 7);
    } else {
        gPager_entries[mapping->slot].team = 0xFF;
        slot = mapping->slot;
        if (gPager_entries[slot].slot == 0xFF) {
        unmap:
            func_0028EEC0(slot);
            gPager_entries[mapping->slot].id = 0xFFFF;
        } else {
            func_0028EE40(slot, 0, gPager_entries[slot].id << 12, -1,
                          ((s32)&gVMPages & 0x3FFFFFF) + (gPager_entries[slot].slot << 12), 7);
        }
    }
    mapping->slot = 0xFF;
}
