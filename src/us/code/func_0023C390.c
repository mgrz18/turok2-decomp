#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

extern void CVector3__Subtract(CVector3 *out, CVector3 *a, void *b);

s32 Plane_IntersectSegmentT(CollisionQuad *plane, CVector3 *start, CVector3 *end, f32 *t) {
    CVector3 dir;
    CVector3 diff;
    f32 denom;

    CVector3__Subtract(&dir, end, start);
    denom = plane->normal.x * dir.x + plane->normal.y * dir.y + plane->normal.z * dir.z;
    if (denom >= 0.0f) {
        *t = 1.0f;
        return 0;
    }
    CVector3__Subtract(&diff, start, &plane->corner[0]);
    *t = -(plane->normal.x * diff.x + plane->normal.y * diff.y + plane->normal.z * diff.z) / denom;
    return 1;
}
