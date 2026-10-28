#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

/* A vertex of this kind of triangle keeps its height after z. */
typedef struct Vertex {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 y;
} Vertex;

typedef struct Triangle {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vertex *v[3];
} Triangle;

extern Triangle *D_800B2600;    /* triangle whose normal is cached */
extern Vec3 D_800F2000;         /* its normal */

void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);    /* a - b */
void func_0020EFDC(Vec3 *out, Vec3 *a, Vec3 *b);    /* cross product */

/* tri's face normal, cached for the last triangle asked about. */
static __inline__ Vec3 triNormal(Triangle *tri) {
    Vec3 edge1;
    Vec3 edge2;

    if (tri != D_800B2600) {
        edge1.x = tri->v[1]->x - tri->v[0]->x;
        edge1.y = tri->v[1]->y - tri->v[0]->y;
        edge1.z = tri->v[1]->z - tri->v[0]->z;
        edge2.x = tri->v[2]->x - tri->v[1]->x;
        edge2.y = tri->v[2]->y - tri->v[1]->y;
        edge2.z = tri->v[2]->z - tri->v[1]->z;
        func_0020EFDC(&D_800F2000, &edge2, &edge1);
    }
    D_800B2600 = tri;
    return D_800F2000;
}

/*
 * Where the segment from -> to crosses tri's plane, going against its normal:
 * *t gets the fraction along the segment (1 when moving away from the plane);
 * returns whether the crossing lies strictly between the ends.
 */
s32 func_0026A798(Triangle *tri, Vec3 *from, Vec3 *to, f32 *t) {
    Vec3 normal;
    Vec3 dir;
    Vec3 v;
    Vec3 diff;
    f32 denom;
    f32 frac;

    if (tri == NULL) {
        return 0;
    }
    normal = triNormal(tri);
    func_0020EF2C(&dir, to, from);
    v.x = tri->v[0]->x;
    v.y = tri->v[0]->y;
    v.z = tri->v[0]->z;
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
