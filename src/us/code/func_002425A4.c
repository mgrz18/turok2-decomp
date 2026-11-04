#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

void func_00241960(void *object, CVector3 *points, s32 arg2);
void func_0023C9CC(CollisionQuad *quad, CVector3 *points);
f32 func_0023C8A0(CollisionQuad *quad, f32 x, f32 z);

/* Height at (x, z) of the face built from points 0-3 of the object's box. */
f32 func_002425A4(void *object, f32 x, f32 z) {
    CVector3 points[8];
    CollisionQuad quad;

    func_00241960(object, points, 0);
    func_0023C9CC(&quad, points);
    return func_0023C8A0(&quad, x, z);
}
