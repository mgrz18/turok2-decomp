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
void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);
void func_0020EFDC(Vec3 *out, Vec3 *a, Vec3 *b);    /* cross product */
void func_0020F040(Vec3 *v);                        /* normalize */

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

/* The velocity something slides with down the slope of tri. */
Vec3 func_00269DF0(Triangle *tri, f32 speed) {
    Vec3 normal;
    Vec3 up;
    Vec3 side;
    Vec3 downhill;
    Vec3 down;
    Vec3 result;

    if (tri == NULL) {
        result.x = result.y = result.z = 0.0f;
        return result;
    } else {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        normal = triNormal(tri);
        func_0020EFDC(&side, &normal, &up);
        func_0020EFDC(&downhill, &normal, &side);
        func_0020F040(&downhill);
        if (downhill.y > 0.0f) {
            func_0020EF60(&downhill, &downhill, -1.0f);
        }
        down.x = 0.0f;    /* set but never used */
        down.z = 0.0f;
        down.y = -1.0f;
        func_0020EF60(&result, &downhill, -downhill.y * -speed * 10.24f);
    }
    return result;
}
