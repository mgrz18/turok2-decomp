/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002170A0.c, func_002170A0.
 * The same code in Turok 2, on Turok 2's types: the position goes to GameObject_DropPickupAt as
 * one CVector3 by value (round 29: Rage Wars' view passed its three words, which the port had to
 * route through GameObject_DropPickupAt_i3). */
#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"
#include "globals/pickup.h"

/* Drops, once (the first time `bit` is set in *flags), a pickup for each mask bit of m1 in
 * sDropPickups and of m2 in sDropPickups2, around the object. */
void AI_DropPickupsOnce(CInstanceHeader *o, u32 *flags, u32 bit, u32 m1, u32 m2) {
    CDropPickup *e;
    if (!(*flags & bit)) {
        *flags |= bit;
        for (e = sDropPickups; e->m_Mask != 0; e++) {
            if (m1 & e->m_Mask)
                GameObject_DropPickupAt(o, o->m_vPos, e->m_Id, Random_Blend(450.0f, 600.0f));
        }
        for (e = sDropPickups2; e->m_Mask != 0; e++) {
            if (m2 & e->m_Mask)
                GameObject_DropPickupAt(o, o->m_vPos, e->m_Id, Random_Blend(450.0f, 600.0f));
        }
    }
}
