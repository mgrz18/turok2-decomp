#include "common.h"
#include "m2c_macros.h"


f32 Ease_CubicOut(f32 arg0) {
    f32 temp_f12;

    temp_f12 = 1.0f - arg0;
    return 1.0f - (temp_f12 * temp_f12 * temp_f12);
}
