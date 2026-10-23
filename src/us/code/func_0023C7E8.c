#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quad {
    Vec3 corner[4]; /* 0x00 */
    Vec3 normal;    /* 0x30 */
} Quad;

/* Point-in-quad test in one axis-aligned projection of the quad; the edge winding follows the sign of the normal. */
s32 func_0023C7E8(Quad *quad, f32 u, f32 v) {
    s32 i;
    Vec3 *from;
    Vec3 *to;
    f32 d;

    for (i = 0; i < 4; i++) {
        if (0.0f < quad->normal.z) {
            to = &quad->corner[i];
            from = &quad->corner[(i + 1) & 3];
        } else {
            from = &quad->corner[i];
            to = &quad->corner[(i + 1) & 3];
        }
        d = (to->y - from->y) * (u - from->x) + (from->x - to->x) * (v - from->y);
        if (d < 0.0f) {
            return 0;
        }
    }
    return 1;
}
