/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_15.c, func_8023CCD4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pager.h"

extern void VM_FreeRegion(OwnerRecAt8V2 *arg0);
void VM_FreeRegion(OwnerRecAt8V2 *arg0) {
    extern s32 osSendMesg(s32, Node_func_8023CCD4_de *, s32);
    extern void VM_UnmapTLBEntry(Slot_func_8023CCD4_de *);
    Node_func_8023CCD4_de *node;
    Node_func_8023CCD4_de *prev;
    u8 *slot;
    s32 i;
    u32 key;
    node = &gPager_regions;
    key = arg0->handle >> 12;
    prev = 0;
    while (node != 0) {
        if (node->id == key) {
            break;
        }
        prev = node;
        node = node->next;
    }
    prev->next = node->next;
    slot = node->slots;
    for (i = 0; i < node->count; i++) {
        if (*slot != 0xFF) {
            VM_UnmapTLBEntry(&gPager_slots[*slot]);
            gPager_slots[*slot].id = 0xFFFF;
        }
        slot++;
    }
    osSendMesg(arg0->queue, node, 1);
}
