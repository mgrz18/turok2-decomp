#include "common.h"
#include "structs/camera.h"

extern f32 D_800F5A40[];

s32 func_0028D0E0(void);

/* Moves value toward target by step, without overshooting; jumps straight
 * there when func_0028D0E0 says so. While it moves, D_800F5A40[1] is set
 * to 30. */
f32 func_002820FC(CCameraPool *pool, f32 value, f32 target, f32 step) {
    if (func_0028D0E0() != 0) {
        value = target;
    }
    if (value < target) {
        value += step;
        D_800F5A40[1] = 30.0f;
        if (value > target) {
            value = target;
        }
    } else if (value > target) {
        value -= step;
        D_800F5A40[1] = 30.0f;
        if (value < target) {
            value = target;
        }
    }
    return value;
}
