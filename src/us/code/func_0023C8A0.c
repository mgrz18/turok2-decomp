#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quad {
    Vec3 corner[4]; /* 0x00 */
    Vec3 normal;    /* 0x30 */
} Quad;

/* Height of the quad's plane at (x, z); a vertical quad gives the mean corner height. */
f32 func_0023C8A0(Quad *quad, f32 x, f32 z) {
    Vec3 normal;
    Vec3 origin;

    normal = quad->normal;
    if (normal.y == 0.0f) {
        return (quad->corner[0].y + quad->corner[1].y + quad->corner[2].y + quad->corner[3].y) * 0.25f;
    }
    origin = quad->corner[0];
    return ((origin.z - z) * normal.z + (origin.x - x) * normal.x + origin.y * normal.y) / normal.y;
}
