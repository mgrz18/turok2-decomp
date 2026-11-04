#include "common.h"
#include "structs/game_object.h"

/* Squared distance between two points. */
f32 func_0020F680(CVector3 *arg0, CVector3 *arg1) {
    f32 temp_f0;
    f32 temp_f1;
    f32 temp_f3;

    temp_f1 = arg0->x - arg1->x;
    temp_f3 = arg0->y - arg1->y;
    temp_f0 = arg0->z - arg1->z;
    return (temp_f1 * temp_f1) + (temp_f3 * temp_f3) + (temp_f0 * temp_f0);
}
