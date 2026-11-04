#include "common.h"
#include "structs/vector.h"

typedef struct Plane {
    u8 pad0[0x30];
    CVector3 normal;        /* 0x30 */
} Plane;

void func_0020EF2C(CVector3 *out, CVector3 *a, void *b);
void func_0020EF8C(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);

/* func_0023C390, inlined. */
static __inline__ s32 PlaneSegmentT(Plane *plane, CVector3 *start, CVector3 *end, f32 *t) {
    CVector3 dir;
    CVector3 diff;
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
s32 func_0023C478(Plane *plane, CVector3 *start, CVector3 *end, CVector3 *hit) {
    f32 t;

    if (PlaneSegmentT(plane, start, end, &t)) {
        func_0020EF8C(hit, t, start, end);
        return 1;
    }
    return 0;
}
