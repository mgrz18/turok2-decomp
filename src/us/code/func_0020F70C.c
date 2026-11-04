#include "common.h"
#include "structs/game_object.h"

f32 func_00298470(f32);         /* sqrtf */

/* Clamps the vector length to arg1. */
void func_0020F70C(CVector3 *arg0, f32 arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f1;
    f32 temp_f2;

    temp_f2 = arg0->x;
    temp_f1 = arg0->y;
    temp_f0 = arg0->z;
    temp_f12 = (temp_f2 * temp_f2) + (temp_f1 * temp_f1) + (temp_f0 * temp_f0);
    if ((arg1 * arg1) < temp_f12) {
        temp_f0_2 = arg1 / func_00298470(temp_f12);
        arg0->x = (f32) (arg0->x * temp_f0_2);
        arg0->y = (f32) (arg0->y * temp_f0_2);
        arg0->z = (f32) (arg0->z * temp_f0_2);
    }
}
