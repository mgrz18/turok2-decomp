/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002AD9DC.c, func_002AD9DC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/pickup.h"

CPickupRow10 *Pickup_FindItemEntry(s32 id) {
    s32 i;
    CPickupRow10 *p = sItemPickups;
    for (i = 14; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return ((void *)0);
}
