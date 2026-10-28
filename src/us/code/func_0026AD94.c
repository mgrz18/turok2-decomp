#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quat {
    f32 x, y, z, w;
} Quat;

typedef struct Triangle {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 *v[3];
} Triangle;

extern f32 D_800DD78C;

f32 func_00298470(f32 x);   /* sqrtf */
f32 func_0021153C(f32 x);   /* acosf */
f32 func_002982F0(f32 x);   /* sinf */
f32 func_002974C0(f32 x);   /* cosf */
void func_00210F68(Quat *out, Quat *a, Quat *b);    /* quaternion product */

/* Heading of tri's edge from vertex i to the next one, seen from above. */
static __inline__ f32 edgeHeading(Triangle *tri, s32 i) {
    Vec3 *a = tri->v[i];
    Vec3 *b = tri->v[(i + 1) % 3];
    f32 dx = b->x - a->x;
    f32 dz = b->z - a->z;
    f32 len = func_00298470(dx * dx + dz * dz);
    f32 angle;

    if (len == 0.0f) {
        return 0.0f;
    }
    angle = func_0021153C(dx / len);
    if (!(dz < 0.0f)) {
        angle = -angle;
    }
    return angle;
}

/* Orientation lying along tri's edge i: turned to its heading, then tipped a quarter turn. */
Quat func_0026AD94(Triangle *tri, s32 i) {
    Quat heading;
    Quat tilt;
    Quat result;
    f32 s;
    f32 half;

    {   /* its own temporary: a function-wide one costs a register copy */
        f32 t = func_002982F0(edgeHeading(tri, i) * 0.5f);
        heading.x = 0.0f;
        heading.y = t;
        heading.z = 0.0f;
        D_800DD78C = t;
    }
    heading.w = func_002974C0(edgeHeading(tri, i) * 0.5f);
    half = -0.785398245f;
    s = func_002982F0(half);
    D_800DD78C = s;
    tilt.x = s;
    tilt.y = 0.0f;
    tilt.z = 0.0f;
    tilt.w = func_002974C0(half);
    func_00210F68(&result, &tilt, &heading);
    return result;
}
