#include "common.h"
#include "structs/vector.h"

typedef struct Plane {
    u8 pad0[0x30];
    CVector3 normal;        /* 0x30 */
} Plane;

void CVector3__Subtract(CVector3 *out, CVector3 *a, void *b);
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);

/* Plane_IntersectSegmentT, inlined. */
static __inline__ s32 PlaneSegmentT(Plane *plane, CVector3 *start, CVector3 *end, f32 *t) {
    CVector3 dir;
    CVector3 diff;
    f32 denom;

    CVector3__Subtract(&dir, end, start);
    denom = plane->normal.x * dir.x + plane->normal.y * dir.y + plane->normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    CVector3__Subtract(&diff, start, plane);
    *t = -(plane->normal.x * diff.x + plane->normal.y * diff.y + plane->normal.z * diff.z) / denom;
    return 1;
}

/* Where the segment from start to end crosses plane (from its front), in *hit. */
s32 Plane_IntersectSegment(Plane *plane, CVector3 *start, CVector3 *end, CVector3 *hit) {
    f32 t;

    if (PlaneSegmentT(plane, start, end, &t)) {
        CVector3__Lerp(hit, t, start, end);
        return 1;
    }
    return 0;
}
