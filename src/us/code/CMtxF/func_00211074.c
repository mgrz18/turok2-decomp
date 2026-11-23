#include "common.h"
#include "structs/game_object.h"

f32 func_00298470(f32);

/* Normalises a quaternion. */
void CQuaternion__Normalize(CQuaternion *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f2;

    temp_f12 = arg0->x;
    temp_f2 = arg0->y;
    temp_f1 = arg0->z;
    temp_f0 = arg0->w;
    temp_f0_2 = func_00298470((temp_f12 * temp_f12) + (temp_f2 * temp_f2) + (temp_f1 * temp_f1) + (temp_f0 * temp_f0));
    if (temp_f0_2 != 0.0f) {
        temp_f1_2 = 1.0f / temp_f0_2;
        arg0->x = (f32) (arg0->x * temp_f1_2);
        arg0->y = (f32) (arg0->y * temp_f1_2);
        arg0->z = (f32) (arg0->z * temp_f1_2);
        arg0->w = (f32) (arg0->w * temp_f1_2);
    }
}
