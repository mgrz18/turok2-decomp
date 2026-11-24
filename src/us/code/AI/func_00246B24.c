/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00217594.c, func_00217594.
 * The same code in Turok 2, on Turok 2's types (the position as one CVector3 by value, see
 * AI_DropPickupsOnce). */
#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"
#include "structs/pickup.h"

/* Drops a pickup for each entry of the 0-ended table `e` whose mask meets `mask`. */
void AI_DropPickupsByMask(CInstanceHeader *o, s32 a1, u32 mask, CDropPickup *e) {
    for (; e->m_Mask != 0; e++) {
        if (mask & e->m_Mask) {
            GameObject_DropPickupAt(o, o->m_vPos, e->m_Id, Random_Blend(4.5e2f, 6.0e2f));
        }
    }
}
