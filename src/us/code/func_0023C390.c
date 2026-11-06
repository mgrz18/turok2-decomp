#include "common.h"
#include "structs/vector.h"

typedef struct Plane {
    u8 pad0[0x30];
    CVector3 normal;        /* 0x30 */
} Plane;

extern void CVector3__Subtract(CVector3 *out, CVector3 *a, void *b);
extern f32 D_800A66A4;

s32 Plane_IntersectSegmentT(Plane *plane, CVector3 *start, CVector3 *end, f32 *t) {
    CVector3 dir;
    CVector3 diff;
    f32 denom;

    CVector3__Subtract(&dir, end, start);
    denom = plane->normal.x * dir.x + plane->normal.y * dir.y + plane->normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = D_800A66A4;
        return 0;
    }
    CVector3__Subtract(&diff, start, plane);
    *t = -(plane->normal.x * diff.x + plane->normal.y * diff.y + plane->normal.z * diff.z) / denom;
    return 1;
}
