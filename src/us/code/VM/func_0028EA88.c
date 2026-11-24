/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_15.c, func_8023CC08_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * globals/pager.h (round 29, part r2: Rage Wars' Link_func_8023CC08_de is PageMapping,
 * Node_func_8023CC08_de PageRegion). */

#include "common.h"
#include "structs/pager.h"
#include "globals/pager.h"
#include "protos/vm.h"

/* Takes the least recently used mapping off the chain and frees its page. */
PageMapping *VM_ReclaimLRUPage(void) {
    PageMapping *entry;
    PageRegion *region;
    entry = gPager_chain_head;
    if (entry->prev != NULL) {
        entry->prev->next = *(PageMapping * volatile *)&entry->next;
    } else {
        gPager_chain_head = entry->next;
        (*(PageMapping * volatile *)&entry->next)->prev = NULL;
    }
    if (entry->next != NULL) {
        entry->next->prev = *(PageMapping * volatile *)&entry->prev;
    } else {
        gPager_chain_tail = entry->prev;
        (*(PageMapping * volatile *)&entry->prev)->next = NULL;
    }
    VM_UnmapTLBEntry(entry);
    if (entry->id != 0xFFFF) {
        region = VM_FindRegion(entry->id);
        region->map[entry->id - region->start] = 0xFF;
        entry->id = 0xFFFF;
    }
    entry->age = 0;
    return entry;
}
