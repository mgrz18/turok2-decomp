/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/8027451C.CPP, cross_edges, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x0 */ u8 pad0[2];
    /* 0x2 */ u16 flags;
    /* 0x4 */ Vec3f *vertices[3];
} Triangle;

extern s32 func_0026AFC0(Triangle *tri);                      /* is the triangle a slope? */
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a - b */
extern void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a x b */
extern f32 func_00298470(f32);                                /* sqrtf */
extern f32 func_0021153C(f32);                                /* acosf */

extern Triangle *D_800B2608;    /* triangle of the cached raw normal */
extern Vec3f D_800F2020;        /* cached raw normal */

/* Raw (unnormalised) normal of a triangle, cached for the last triangle. */
static __inline__ Vec3f raw_normal(Triangle *tri) {
    Vec3f edge0;
    Vec3f edge1;

    if (tri == NULL) {
        D_800F2020.x = 0;
        D_800F2020.z = 0;
        D_800F2020.y = 1.0f;
    } else if (tri != D_800B2608) {
        func_0020EF2C(&edge0, tri->vertices[1], tri->vertices[0]);
        func_0020EF2C(&edge1, tri->vertices[2], tri->vertices[1]);
        func_0020EFDC(&D_800F2020, &edge0, &edge1);
    }
    D_800B2608 = tri;
    return D_800F2020;
}

/* Downhill yaw of a sloped triangle; 0 when it is not a slope or is level. */
f32 func_0026C434(Triangle *tri) {
    Vec3f normal;
    f32 mag;
    f32 result;

    if (!func_0026AFC0(tri)) {
        return 0.0f;
    }
    normal = raw_normal(tri);
    mag = func_00298470(normal.x * normal.x + normal.z * normal.z);
    if (mag == 0.0f) {
        return 0.0f;
    }
    result = func_0021153C(normal.z / mag);
    if (!(normal.x > 0.0f)) {
        result = -result;
    }
    return result;
}
