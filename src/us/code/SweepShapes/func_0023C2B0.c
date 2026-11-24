#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);

s32 Plane_SegmentCrosses(CollisionQuad *plane, CVector3 *a, CVector3 *b) {
    CVector3 d;
    s32 sideA;
    s32 sideB;

    CVector3__Subtract(&d, a, (CVector3 *) plane);
    sideA = (plane->normal.x * d.x + plane->normal.y * d.y + plane->normal.z * d.z >= 0.0f) ? 0 : 1;
    CVector3__Subtract(&d, b, &plane->corner[0]);
    sideB = (plane->normal.x * d.x + plane->normal.y * d.y + plane->normal.z * d.z >= 0.0f) ? 0 : 1;
    return sideA ^ sideB;
}
