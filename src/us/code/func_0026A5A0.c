#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Triangle {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 *v[3];
} Triangle;

extern Triangle *D_800B2608;    /* triangle whose normal is cached */
extern Vec3 D_800F2020;         /* its normal */

void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);    /* a - b */
void func_0020EFDC(Vec3 *out, Vec3 *a, Vec3 *b);    /* cross product */

/* tri's face normal, cached for the last triangle asked about; straight up without one. */
static __inline__ Vec3 triNormal(Triangle *tri) {
    Vec3 edge1;
    Vec3 edge2;

    if (tri == NULL) {
        D_800F2020.x = 0.0f;
        D_800F2020.z = 0.0f;
        D_800F2020.y = 1.0f;
    } else if (tri != D_800B2608) {
        func_0020EF2C(&edge1, tri->v[1], tri->v[0]);
        func_0020EF2C(&edge2, tri->v[2], tri->v[1]);
        func_0020EFDC(&D_800F2020, &edge1, &edge2);
    }
    D_800B2608 = tri;
    return D_800F2020;
}

/*
 * Where the segment from -> to crosses tri's plane, going against its normal:
 * *t gets the fraction along the segment (1 when moving away from the plane);
 * returns whether the crossing lies strictly between the ends.
 */
s32 func_0026A5A0(Triangle *tri, Vec3 *from, Vec3 *to, f32 *t) {
    Vec3 normal;
    Vec3 dir;
    Vec3 v;
    Vec3 diff;
    f32 denom;
    f32 frac;

    normal = triNormal(tri);
    func_0020EF2C(&dir, to, from);
    if (tri != NULL) {
        v = *tri->v[0];
    } else {
        v.x = v.y = v.z = 0.0f;
    }
    denom = normal.x * dir.x + normal.y * dir.y + normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    func_0020EF2C(&diff, from, &v);
    frac = -(normal.x * diff.x + normal.y * diff.y + normal.z * diff.z) / denom;
    *t = frac;
    return frac > 0.0f && frac < 1.0f;
}
