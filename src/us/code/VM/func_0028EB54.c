/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_15.c, func_8023CCD4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * globals/pager.h (round 29, part r2: Rage Wars' OwnerRecAt8V2 is PageFreeRequest,
 * Node_func_8023CCD4_de PageRegion). */

#include "common.h"
#include "structs/pager.h"
#include "globals/pager.h"
#include "protos/ultra.h"
#include "protos/vm.h"

/* Unlinks the region at req->address, drops its resident pages and hands it back. */
void VM_FreeRegion(PageFreeRequest *req) {
    PageRegion *node;
    PageRegion *prev;
    u8 *slot;
    s32 i;
    u32 key;
    node = &gPager_regions;
    key = req->address >> 12;
    prev = 0;
    while (node != 0) {
        if (node->start == key) {
            break;
        }
        prev = node;
        node = node->next;
    }
    prev->next = node->next;
    slot = node->map;
    for (i = 0; i < node->pages; i++) {
        if (*slot != 0xFF) {
            VM_UnmapTLBEntry(&gPager_slots[*slot]);
            gPager_slots[*slot].id = 0xFFFF;
        }
        slot++;
    }
    osSendMesg(req->reply, node, 1);
}
