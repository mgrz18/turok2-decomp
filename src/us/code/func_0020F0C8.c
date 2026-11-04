#include "structs/vector.h"

/* Point on segment a-b at height y. */
void func_0020F0C8(CVector3 *out, CVector3 *a, CVector3 *b, f32 y) {
    f32 dy = b->y - a->y;
    f32 t;

    if (dy == 0.0f) {
        *out = *a;
        return;
    }
    t = (y - a->y) / dy;
    out->x = a->x + t * (b->x - a->x);
    out->y = y;
    out->z = a->z + t * (b->z - a->z);
}
