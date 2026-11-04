#include "common.h"
#include "structs/game_object.h"

/* out = b * a (quaternions, x y z w). */
void func_00210F68(CQuaternion *out, CQuaternion *a, CQuaternion *b) {
    out->x = (((b->w * a->x) - (b->z * a->y)) + (b->y * a->z) + (b->x * a->w));
    out->y = ((((b->z * a->x) + (b->w * a->y)) - (b->x * a->z)) + (b->y * a->w));
    out->z = ((-b->y * a->x) + (b->x * a->y) + (b->w * a->z) + (b->z * a->w));
    out->w = ((((-b->x * a->x) - (b->y * a->y)) - (b->z * a->z)) + (b->w * a->w));
}
