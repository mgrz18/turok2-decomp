/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8024D018_3.CPP, func_8024D728_de, with Turok 2's layouts (Turok 2 reads the normal from the region). */
#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quat {
    f32 x, y, z, w;
} Quat;

typedef struct Actor {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ void *region;
} Actor;

extern f32 D_800DD78C;

void func_0026BD20(Vec3 *normal, void *region);
void func_0020EFDC(Vec3 *out, Vec3 *a, Vec3 *b);     /* cross product */
void func_0020F040(Vec3 *v);                         /* normalize */
f32 func_0021153C(f32 x);                            /* acos */
f32 func_002982F0(f32 x);                            /* sin */
f32 func_002974C0(f32 x);                            /* cos */

/* Tilt of an actor toward the floor normal under it. */
Quat func_00268F3C(Actor *obj) {
    Vec3 normal;
    Vec3 up;
    Vec3 axis;
    Quat q;
    f32 angle;

    func_0026BD20(&normal, obj->region);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    func_0020EFDC(&axis, &up, &normal);
    func_0020F040(&axis);
    angle = func_0021153C(up.x * normal.x + up.y * normal.y + up.z * normal.z) * 0.5f;
    D_800DD78C = func_002982F0(angle);
    q.x = axis.x * D_800DD78C;
    q.y = axis.y * D_800DD78C;
    q.z = axis.z * D_800DD78C;
    q.w = func_002974C0(angle);
    return q;
}
