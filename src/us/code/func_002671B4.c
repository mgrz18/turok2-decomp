/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024D274.c, func_0024D274.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/player.h"

extern f32 func_002683CC(CGameObjectInstance *a);
extern f32 D_800A7D9C;
extern f32 D_800A7DA0;

/* NOTE: Turok 2 differs from Rage Wars in case 11 (subtracts two owner values
 * and falls through to case 1/4) and has no cases 13/14.
 * The object's collision height, by the type of its intelligence (a ducking
 * player's lower). */
f32 func_002671B4(CGameObjectInstance *a) {
    CIntelligence *d;
    f32 r;
    if (a->header.m_Type == 1 && a->m_AI.m_Health <= 0) {
        return func_002683CC(a);
    }
    switch (a->header.m_pIntelligence->m_Type) {
    case 11:
        if (a->header.m_Type == 1 && (a->m_dwFlags & 0x300000)) {
            return a->header.m_pIntelligence->m_Player.m_CollisionHeight
                   - ((CPlayer *) a->m_AI.m_pParent)->m_ActualCameraVertEyeOffset
                   - ((CPlayer *) a->m_AI.m_pParent)->m_DuckOffset;
        }
        /* fallthrough */
    case 1:
    case 4:
        return a->header.m_pIntelligence->m_Enemy.m_CollisionHeight;
    case 8:
        d = a->header.m_pIntelligence;
        r = d->m_Pickup.m_dwFlags & 1; /* dead store, needed for scheduling */
        r = D_800A7D9C;
        if (!(d->m_Pickup.m_dwFlags & 1)) {
            r = d->m_Pickup.m_CollisionHeight;
        }
        return r;
    case 7:
        return a->header.m_pIntelligence->m_Destructible.m_CollisionHeight;
    case 6:
        return D_800A7DA0;
    case 0:
    case 5:
    case 10:
    case 12:
        return a->header.m_pIntelligence->m_Static.m_CollisionHeight;
    default:
        return 0.0f;
    }
}
