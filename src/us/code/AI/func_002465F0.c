#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/vector.h"
#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00217224.c, func_00217224.
 * The same code in Turok 2, on CGameObjectInstance (Rage Wars' B04Obj view: the position at 0x4,
 * the height at 0x54, m_CollisionYOffset). */

/* The squared distance from the object's collision centre (its position raised by
 * m_CollisionYOffset) to `v`. */
f32 AI_DistSqToPointAtHeight(CGameObjectInstance *o, CVector3 v) {
    CVector3__Subtract(&v, &v, &o->header.m_vPos);
    v.y = v.y - o->m_CollisionYOffset;
    return v.x * v.x + v.y * v.y + v.z * v.z;
}
