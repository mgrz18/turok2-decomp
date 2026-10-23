#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Plane {
    u8 pad0[0x30];
    Vec3 normal;        /* 0x30 */
} Plane;

extern void func_0020EF2C(Vec3 *out, Vec3 *a, void *b);
extern f32 D_800A66A4;

s32 func_0023C390(Plane *plane, Vec3 *start, Vec3 *end, f32 *t) {
    Vec3 dir;
    Vec3 diff;
    f32 denom;

    func_0020EF2C(&dir, end, start);
    denom = plane->normal.x * dir.x + plane->normal.y * dir.y + plane->normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = D_800A66A4;
        return 0;
    }
    func_0020EF2C(&diff, start, plane);
    *t = -(plane->normal.x * diff.x + plane->normal.y * diff.y + plane->normal.z * diff.z) / denom;
    return 1;
}
