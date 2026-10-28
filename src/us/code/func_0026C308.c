/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/ANIM/80275E44.CPP, func_802760BC_de (inlined), with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x0 */ u8 pad0[2];
    /* 0x2 */ u16 flags;
    /* 0x4 */ Vec3f *vertices[3];
} Triangle;

extern f32 func_00298470(f32);                                  /* sqrtf */
extern f32 func_0021153C(f32);                                  /* acosf */
extern void func_002105A4(f32 mtx[4][4], f32 angle);            /* rotation about Y */
extern void func_0020F85C(f32 mtx[4][4], Vec3f *in, Vec3f *out); /* out = mtx * in */

/* Yaw of a triangle edge (func_0026C240). */
static __inline__ f32 edge_yaw(Triangle *tri, s32 edge) {
    Vec3f *v0;
    Vec3f *v1;
    f32 dx, dz;
    f32 mag;
    f32 result;

    v1 = tri->vertices[edge];
    v0 = tri->vertices[(edge + 1) % 3];
    dx = v0->x - v1->x;
    dz = v0->z - v1->z;
    mag = func_00298470(dx * dx + dz * dz);
    if (mag == 0.0f) {
        return 0.0f;
    }
    result = func_0021153C(dx / mag);
    if (!(dz < 0.0f)) {
        result = -result;
    }
    return result;
}

/* Horizontal unit vector facing along a triangle edge's yaw. */
Vec3f *func_0026C308(Vec3f *out, Triangle *tri, s32 edge) {
    Vec3f dir;
    Vec3f forward;
    f32 mtx[4][4];

    func_002105A4(mtx, edge_yaw(tri, edge));
    forward.x = 0;
    forward.y = 0;
    forward.z = 1.0f;
    func_0020F85C(mtx, &forward, &dir);
    *out = dir;
    return out;
}
