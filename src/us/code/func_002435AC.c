/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_2.CPP, func_80214310_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/game_object.h"

typedef f32 Mtx4[4][4];


f32 func_002671B4(CGameObjectInstance *actor);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
f32 func_00298470(f32 x);
void func_0021AE6C(CQuaternion *q, CGameObjectInstance *actor);
void func_00211114(CQuaternion *q, Mtx4 *mtx);
void func_0020F85C(Mtx4 *mtx, CVector3 *in, CVector3 *out);
f32 func_0021153C(f32 x);
void func_0020F040(CVector3 *v);

/* Whether target is within range of viewer and inside its horizontal (across) and vertical (up) view angles. */
s32 func_002435AC(CGameObjectInstance *viewer, CGameObjectInstance *target, f32 range, f32 across, f32 up) {
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
    from.y += func_002671B4(viewer) * 0.9f;
    to = target->header.m_vPos;
    to.y += target->m_CollisionYOffset;
    to.y += func_002671B4(target) * 0.9f;
    func_0020EF2C(&dir, &to, &from);
    distance = func_00298470(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (range < distance) {
        return 0;
    }
    func_0021AE6C(&rotation, viewer);
    func_00211114(&rotation, &matrix);
    ahead.x = 0.0f;
    ahead.y = 0.0f;
    ahead.z = distance;
    func_0020F85C(&matrix, &ahead, &forward);
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
    angle = func_0021153C(flatDir.x * flatForward.x + flatDir.y * flatForward.y + flatDir.z * flatForward.z);
    if (angle < zero) {
        angle = -angle;
    }
    if (across < angle) {
        return 0;
    }
    *(volatile f32 *)&pitchDir.x = 0.0f;
    pitchDir.z = dirLength;
    pitchDir.y = dir.y;
    func_0020F040(&pitchDir);
    *(volatile f32 *)&pitchForward.x = 0.0f;
    pitchForward.z = forwardLength;
    pitchForward.y = forward.y;
    func_0020F040(&pitchForward);
    angle = func_0021153C(pitchDir.x * pitchForward.x + pitchDir.y * pitchForward.y + pitchDir.z * pitchForward.z);
    if (up < ((angle < 0.0f) ? -angle : angle)) {
        return 0;
    }
    return 1;
}
