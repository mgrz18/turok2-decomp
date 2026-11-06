/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002170A0.c, func_002170A0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/pickup.h"

f32 Random_Blend(f32, f32);
void GameObject_DropPickupAt(void *, s32, s32, s32, s32, f32);
typedef struct { u8 pad0[4]; s32 x, y, z; /* pos at 0x4 */ } B04O;
void AI_DropPickupsOnce(B04O *o, u32 *flags, u32 bit, u32 m1, u32 m2) {
    CDropPickup *e;
    if (!(*flags & bit)) {
        *flags |= bit;
        for (e = sDropPickups; e->m_Mask != 0; e++) {
            if (m1 & e->m_Mask)
                GameObject_DropPickupAt(o, o->x, o->y, o->z, e->m_Id, Random_Blend(450.0f, 600.0f));
        }
        for (e = sDropPickups2; e->m_Mask != 0; e++) {
            if (m2 & e->m_Mask)
                GameObject_DropPickupAt(o, o->x, o->y, o->z, e->m_Id, Random_Blend(450.0f, 600.0f));
        }
    }
}
