/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024E2EC.c, func_0024E2EC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
extern f32 D_800A7EC4;
extern f32 D_800A7EC8;
extern f32 D_800A7ECC;
f32 func_00268304(CGameObjectInstance *a) {
    switch (a->header.m_pIntelligence->m_Type) {
    case 11:
        return a->header.m_pIntelligence->m_Player.m_CollisionRadius;
    case 1:
        return a->header.m_pIntelligence->m_Enemy.m_CollisionWallRadius;
    case 2:
        if (a->header.m_pIntelligence->m_Platform.m_CollisionType == 0) {
            return D_800A7EC4;
        }
        return a->header.m_pIntelligence->m_Platform.m_CollisionRadius;
    case 5:
        return a->header.m_pIntelligence->m_Throwable.m_CollisionRadius;
    case 8:
        if (a->header.m_pIntelligence->m_Pickup.m_dwFlags & 1) {
            return D_800A7EC8;
        }
        return a->header.m_pIntelligence->m_Pickup.m_CollisionRadius;
    default:
        return D_800A7ECC;
    }
}
