#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

void Platform_GetCollisionBox(void *object, CVector3 *points, sptr arg2);
void Quad_SetBoxBottom(CollisionQuad *quad, CVector3 *points);
f32 Quad_GetHeightAt(CollisionQuad *quad, f32 x, f32 z);

/* Height at (x, z) of the face built from points 0-3 of the object's box. */
f32 Platform_GetTopHeight(void *object, f32 x, f32 z) {
    CVector3 points[8];
    CollisionQuad quad;

    Platform_GetCollisionBox(object, points, 0);
    Quad_SetBoxBottom(&quad, points);
    return Quad_GetHeightAt(&quad, x, z);
}
