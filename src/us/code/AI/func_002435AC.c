/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_2.CPP, func_80214310_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/game_object.h"

typedef f32 Mtx4[4][4];


f32 GameObject_GetCollisionHeight(CGameObjectInstance *actor);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
f32 func_00298470(f32 x);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar) */
#define CGameObjectInstance__GetRotation CGameObjectInstance__GetRotation_sret
#endif
void CGameObjectInstance__GetRotation(CQuaternion *q, CGameObjectInstance *actor);
void CQuaternion__ToMatrix(CQuaternion *q, Mtx4 *mtx);
void CMtxF__TransformPoint(Mtx4 *mtx, CVector3 *in, CVector3 *out);
f32 Math_AcosLookup(f32 x);
void CVector3__Normalize(CVector3 *v);

/* Whether target is within range of viewer and inside its horizontal (across) and vertical (up) view angles. */
s32 AI_IsInViewCone(CGameObjectInstance *viewer, CGameObjectInstance *target, f32 range, f32 across, f32 up) {
    CVector3 from;
    CVector3 to;
    CVector3 dir;
    CVector3 ahead;
    CVector3 forward;
    CVector3 flatDir;
    CVector3 flatForward;
    CVector3 pitchDir;
    CVector3 pitchForward;
    CQuaternion rotation;
    Mtx4 matrix;
    f32 distance;
    f32 dirLength;
    f32 forwardLength;
    f32 angle;
    f32 zero;

    from = viewer->header.m_vPos;
    from.y += viewer->m_CollisionYOffset;
    from.y += GameObject_GetCollisionHeight(viewer) * 0.9f;
    to = target->header.m_vPos;
    to.y += target->m_CollisionYOffset;
    to.y += GameObject_GetCollisionHeight(target) * 0.9f;
    CVector3__Subtract(&dir, &to, &from);
    distance = func_00298470(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (range < distance) {
        return 0;
    }
    CGameObjectInstance__GetRotation(&rotation, viewer);
    CQuaternion__ToMatrix(&rotation, &matrix);
    ahead.x = 0.0f;
    ahead.y = 0.0f;
    ahead.z = distance;
    CMtxF__TransformPoint(&matrix, &ahead, &forward);
    across *= 0.5f;
    up *= 0.5f;
    flatDir.x = dir.x;
    flatDir.y = 0.0f;
    zero = *(volatile f32 *)&flatDir.y;
    flatDir.z = dir.z;
    dirLength = func_00298470(flatDir.x * flatDir.x + flatDir.z * flatDir.z);
    if (dirLength != zero) {
        if (across) { /* identical branches, as in Rage Wars: they set the register allocation */
            flatDir.x /= dirLength;
            flatDir.z /= dirLength;
        } else {
            flatDir.x /= dirLength;
            flatDir.z /= dirLength;
        }
    }
    flatForward.x = forward.x;
    flatForward.y = zero;
    flatForward.z = forward.z;
    forwardLength = func_00298470(flatForward.x * flatForward.x + flatForward.z * flatForward.z);
    if (forwardLength != zero) {
        flatForward.x /= forwardLength;
        flatForward.z /= forwardLength;
    }
    angle = Math_AcosLookup(flatDir.x * flatForward.x + flatDir.y * flatForward.y + flatDir.z * flatForward.z);
    if (angle < zero) {
        angle = -angle;
    }
    if (across < angle) {
        return 0;
    }
    *(volatile f32 *)&pitchDir.x = 0.0f;
    pitchDir.z = dirLength;
    pitchDir.y = dir.y;
    CVector3__Normalize(&pitchDir);
    *(volatile f32 *)&pitchForward.x = 0.0f;
    pitchForward.z = forwardLength;
    pitchForward.y = forward.y;
    CVector3__Normalize(&pitchForward);
    angle = Math_AcosLookup(pitchDir.x * pitchForward.x + pitchDir.y * pitchForward.y + pitchDir.z * pitchForward.z);
    if (up < ((angle < 0.0f) ? -angle : angle)) {
        return 0;
    }
    return 1;
}
