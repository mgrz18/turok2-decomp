#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Plane {
    u8 pad0[0x30];
    Vec3 normal;        /* 0x30 */
} Plane;

void func_0020EF2C(Vec3 *out, Vec3 *a, void *b);
void func_0020EF8C(Vec3 *out, f32 t, Vec3 *a, Vec3 *b);

/* func_0023C390, inlined. */
static __inline__ s32 PlaneSegmentT(Plane *plane, Vec3 *start, Vec3 *end, f32 *t) {
    Vec3 dir;
    Vec3 diff;
    f32 denom;

    func_0020EF2C(&dir, end, start);
    denom = plane->normal.x * dir.x + plane->normal.y * dir.y + plane->normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    func_0020EF2C(&diff, start, plane);
    *t = -(plane->normal.x * diff.x + plane->normal.y * diff.y + plane->normal.z * diff.z) / denom;
    return 1;
}

/* Where the segment from start to end crosses plane (from its front), in *hit. */
s32 func_0023C478(Plane *plane, Vec3 *start, Vec3 *end, Vec3 *hit) {
    f32 t;

    if (PlaneSegmentT(plane, start, end, &t)) {
        func_0020EF8C(hit, t, start, end);
        return 1;
    }
    return 0;
}
