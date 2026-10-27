/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_2.CPP, func_80214310_de, with Turok 2's layouts. */

#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quat {
    f32 x, y, z, w;
} Quat;

typedef f32 Mtx4[4][4];

typedef struct Actor {
    char pad0[4];
    Vec3 pos;          /* 0x04 */
    char pad10[0x54 - 0x10];
    f32 eye;           /* 0x54 */
} Actor;

f32 func_002671B4(Actor *actor);
void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);
f32 func_00298470(f32 x);
void func_0021AE6C(Quat *q, Actor *actor);
void func_00211114(Quat *q, Mtx4 *mtx);
void func_0020F85C(Mtx4 *mtx, Vec3 *in, Vec3 *out);
f32 func_0021153C(f32 x);
void func_0020F040(Vec3 *v);

/* Whether target is within range of viewer and inside its horizontal (across) and vertical (up) view angles. */
s32 func_002435AC(Actor *viewer, Actor *target, f32 range, f32 across, f32 up) {
    Vec3 from;
    Vec3 to;
    Vec3 dir;
    Vec3 ahead;
    Vec3 forward;
    Vec3 flatDir;
    Vec3 flatForward;
    Vec3 pitchDir;
    Vec3 pitchForward;
    Quat rotation;
    Mtx4 matrix;
    f32 distance;
    f32 dirLength;
    f32 forwardLength;
    f32 angle;
    f32 zero;

    from = viewer->pos;
    from.y += viewer->eye;
    from.y += func_002671B4(viewer) * 0.9f;
    to = target->pos;
    to.y += target->eye;
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
