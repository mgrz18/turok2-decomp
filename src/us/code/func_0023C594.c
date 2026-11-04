#include "common.h"
#include "structs/vector.h"

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);

/* Returns 1 when point lies on the inner side of all four edges of the quad corners. */
s32 func_0023C594(CVector3 *corners, CVector3 point) {
    CVector3 edge;
    CVector3 toPoint;
    s32 i;
    s32 next;

    for (i = 0; i < 4; i++) {
        next = (i == 3) ? 0 : i + 1;
        func_0020EF2C(&edge, &corners[next], &corners[i]);
        func_0020EF2C(&toPoint, &point, &corners[i]);
        if (edge.x * toPoint.x + edge.y * toPoint.y + edge.z * toPoint.z < 0.0f) {
            return 0;
        }
    }
    return 1;
}
