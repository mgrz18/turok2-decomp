#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Triangle {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 *v[3];
} Triangle;

void func_0020EEF8(Vec3 *out, Vec3 *a, Vec3 *b);    /* a + b */
void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);    /* a - b */
void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);
f32 func_00298470(f32 x);                           /* sqrtf */

/* Centre of tri, as the mean of its edges' midpoints. */
static __inline__ void triCentre(Vec3 *out, Triangle *tri) {
    Vec3 mid0;
    Vec3 mid1;
    Vec3 mid2;

    func_0020EEF8(&mid0, tri->v[0], tri->v[1]);
    func_0020EF60(&mid0, &mid0, 0.5f);
    func_0020EEF8(&mid1, tri->v[1], tri->v[2]);
    func_0020EF60(&mid1, &mid1, 0.5f);
    func_0020EEF8(&mid2, tri->v[2], tri->v[0]);
    func_0020EF60(&mid2, &mid2, 0.5f);
    func_0020EEF8(out, &mid0, &mid1);
    func_0020EEF8(out, out, &mid2);
    func_0020EF60(out, out, 1.0f / 3);
}

/* Distance between the centres of two triangles. */
f32 func_0026B858(Triangle *a, Triangle *b) {
    Vec3 diff;
    Vec3 centreA;
    Vec3 centreB;

    triCentre(&centreA, a);
    triCentre(&centreB, b);
    func_0020EF2C(&diff, &centreA, &centreB);
    return func_00298470(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}
