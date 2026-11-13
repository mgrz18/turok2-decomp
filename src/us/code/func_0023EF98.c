/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002AD90C.c, func_002AD90C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/pickup.h"

CPickupRow18 *Pickup_FindHealthEntry(s32 id) {
    s32 i;
#ifdef TARGET_PORT
    CPickupRow18 *p = (CPickupRow18 *) sHealthPickups; /* typed for gendata (structs/pickup.h) */
#else
    CPickupRow18 *p = sHealthPickups;
#endif
    for (i = 6; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return ((void *)0);
}
