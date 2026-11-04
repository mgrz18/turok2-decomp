#include "common.h"
#include "structs/vector.h"

typedef struct Quad {
    CVector3 corner[4]; /* 0x00 */
    CVector3 normal;    /* 0x30 */
} Quad;

/* Point-in-quad test in one axis-aligned projection of the quad; the edge winding follows the sign of the normal. */
s32 func_0023C730(Quad *quad, f32 u, f32 v) {
    s32 i;
    CVector3 *from;
    CVector3 *to;
    f32 d;

    for (i = 0; i < 4; i++) {
        if (quad->normal.y < 0.0f) {
            to = &quad->corner[i];
            from = &quad->corner[(i + 1) & 3];
        } else {
            from = &quad->corner[i];
            to = &quad->corner[(i + 1) & 3];
        }
        d = (to->z - from->z) * (u - from->x) + (from->x - to->x) * (v - from->z);
        if (d <= 0.0f) {
            return 0;
        }
    }
    return 1;
}
