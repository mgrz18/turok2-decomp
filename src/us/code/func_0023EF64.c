/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002AD940.c, func_002AD940.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/pickup.h"

CPickupRow14 *Pickup_FindAmmoEntry(s32 id) {
    s32 i;
    CPickupRow14 *p = sAmmoPickups;
    for (i = 0x23; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return ((void *)0);
}
