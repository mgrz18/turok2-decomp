#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

/* Height of the quad's plane at (x, z); a vertical quad gives the mean corner height. */
f32 Quad_GetHeightAt(CollisionQuad *quad, f32 x, f32 z) {
    CVector3 normal;
    CVector3 origin;

    normal = quad->normal;
    if (normal.y == 0.0f) {
        return (quad->corner[0].y + quad->corner[1].y + quad->corner[2].y + quad->corner[3].y) * 0.25f;
    }
    origin = quad->corner[0];
    return ((origin.z - z) * normal.z + (origin.x - x) * normal.x + origin.y * normal.y) / normal.y;
}
