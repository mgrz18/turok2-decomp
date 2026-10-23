#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Plane {
    u8 pad0[0x30];
    Vec3 normal;        /* 0x30 */
} Plane;

extern void func_0020EF2C(Vec3 *out, Vec3 *a, Plane *b);

s32 func_0023C2B0(Plane *plane, Vec3 *a, Vec3 *b) {
    Vec3 d;
    s32 sideA;
    s32 sideB;

    func_0020EF2C(&d, a, plane);
    sideA = (plane->normal.x * d.x + plane->normal.y * d.y + plane->normal.z * d.z >= 0.0f) ? 0 : 1;
    func_0020EF2C(&d, b, plane);
    sideB = (plane->normal.x * d.x + plane->normal.y * d.y + plane->normal.z * d.z >= 0.0f) ? 0 : 1;
    return sideA ^ sideB;
}
