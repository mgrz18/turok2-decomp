#include "structs/vector.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002165F8.c, func_002165F8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void AI_ComputeFrameMotion(CGameObjectInstance *a, CAI *b, CAIFrameMotion *o) {
    CVector3__Subtract(&o->m_vDelta, &a->header.m_vPos, &b->m_vPosLastFrame);
    o->m_vDir = o->m_vDelta;
    CVector3__Normalize(&o->m_vDir);
    o->m_Dist = func_00298470(o->m_vDelta.x * o->m_vDelta.x + o->m_vDelta.y * o->m_vDelta.y + o->m_vDelta.z * o->m_vDelta.z);
    o->m_vFlatDelta.y = 0;
    o->m_vFlatDelta.x = o->m_vDelta.x;
    o->m_vFlatDelta.z = o->m_vDelta.z;
    o->m_vFlatDir = o->m_vFlatDelta;
    CVector3__Normalize(&o->m_vFlatDir);
    o->m_FlatDist = func_00298470(o->m_vFlatDelta.x * o->m_vFlatDelta.x + o->m_vFlatDelta.y * o->m_vFlatDelta.y + o->m_vFlatDelta.z * o->m_vFlatDelta.z);
    o->m_Turn = a->m_RotY - b->m_RotYLastFrame;
}
