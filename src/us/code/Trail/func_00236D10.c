#include "common.h"
#include "structs/swoosh.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_3/802A6AC0_2.CPP, func_802A6450_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* The live swoosh of owner arg1 with id arg2, or 0. */
void *SwooshSystem_FindSwoosh(CSwooshSystem *arg0, sptr arg1, s32 arg2) {
    Swoosh *node;
    node = arg0->m_SwooshPool.used.head;
    if (node != 0) {
        do {
            if ((sptr)node->owner == arg1) {
                if (node->id == arg2) {
                    return node;
                }
            }
            node = node->next;
        } while (node != 0);
    }
    return 0;
}
