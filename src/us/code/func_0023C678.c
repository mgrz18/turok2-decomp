#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

/* Point-in-quad test in one axis-aligned projection of the quad; the edge winding follows the sign of the normal. */
s32 func_0023C678(CollisionQuad *quad, f32 u, f32 v) {
    s32 i;
    CVector3 *from;
    CVector3 *to;
    f32 d;

    for (i = 0; i < 4; i++) {
        if (0.0f < quad->normal.x) {
            to = &quad->corner[i];
            from = &quad->corner[(i + 1) & 3];
        } else {
            from = &quad->corner[i];
            to = &quad->corner[(i + 1) & 3];
        }
        d = (to->z - from->z) * (u - from->y) + (from->y - to->y) * (v - from->z);
        if (d < 0.0f) {
            return 0;
        }
    }
    return 1;
}
