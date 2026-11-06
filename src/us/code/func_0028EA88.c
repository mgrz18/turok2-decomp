/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_15.c, func_8023CC08_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern Link_func_8023CC08_de *gPager_chain_tail;
extern Link_func_8023CC08_de *gPager_chain_head;
extern Node_func_8023CC08_de *VM_FindRegion(u32 arg0);
Link_func_8023CC08_de *VM_ReclaimLRUPage(void) {
    extern void VM_UnmapTLBEntry(void *arg0);
    Link_func_8023CC08_de *entry;
    Node_func_8023CC08_de *node;
    entry = gPager_chain_head;
    if (entry->prev != 0) {
        entry->prev->next = *(Link_func_8023CC08_de * volatile *)&entry->next;
    } else {
        gPager_chain_head = entry->next;
        (*(Link_func_8023CC08_de * volatile *)&entry->next)->prev = 0;
    }
    if (entry->next != 0) {
        entry->next->prev = *(Link_func_8023CC08_de * volatile *)&entry->prev;
    } else {
        gPager_chain_tail = entry->prev;
        (*(Link_func_8023CC08_de * volatile *)&entry->prev)->next = 0;
    }
    VM_UnmapTLBEntry(entry);
    if (entry->id != 0xFFFF) {
        node = VM_FindRegion(entry->id);
        node->data[entry->id - node->start] = 0xFF;
        entry->id = 0xFFFF;
    }
    entry->fieldC = 0;
    return entry;
}
