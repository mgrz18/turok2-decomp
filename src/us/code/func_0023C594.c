#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);

/* Returns 1 when point lies on the inner side of all four edges of the quad corners. */
s32 func_0023C594(Vec3 *corners, Vec3 point) {
    Vec3 edge;
    Vec3 toPoint;
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
