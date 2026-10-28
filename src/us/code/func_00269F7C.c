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
f32 func_00298470(f32 x);                           /* sqrtf */

/* tri's face normal, cached for the last triangle asked about. */
static __inline__ Vec3 triNormal(Triangle *tri) {
    Vec3 edge1;
    Vec3 edge2;

    if (tri != D_800B2608) {
        func_0020EF2C(&edge1, tri->v[1], tri->v[0]);
        func_0020EF2C(&edge2, tri->v[2], tri->v[1]);
        func_0020EFDC(&D_800F2020, &edge1, &edge2);
    }
    D_800B2608 = tri;
    return D_800F2020;
}

/* Height of tri's plane above (x, z). */
static __inline__ f32 triHeight(Triangle *tri, f32 x, f32 z) {
    Vec3 normal;
    Vec3 v;

    if (tri == NULL) {
        return -14336.0f;
    }
    normal = triNormal(tri);
    if (normal.y == 0.0f) {
        return (tri->v[0]->y + tri->v[1]->y + tri->v[2]->y) * (1.0f / 3);
    }
    v = *tri->v[0];
    return ((v.z - z) * normal.z + (v.x - x) * normal.x + v.y * normal.y) / normal.y;
}

/* Cosine of the slope climbed going from (x1, z1) to (x2, z2) on tri; 1 when not climbing. */
f32 func_00269F7C(Triangle *tri, f32 x1, f32 z1, f32 x2, f32 z2) {
    f32 h1;
    f32 h2;
    f32 flat;
    f32 total;

    if (tri != NULL) {
        h1 = triHeight(tri, x1, z1);
        h2 = triHeight(tri, x2, z2);
        if (h1 < h2) {
            flat = (x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1);
            total = flat + (h2 - h1) * (h2 - h1);
            if (total != 0.0f) {
                return func_00298470(flat / total);
            }
        }
    }
    return 1.0f;
}
