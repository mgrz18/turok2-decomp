#include "common.h"
#include "structs/camera.h"
#include "globals/camera.h"


s32 SavedPlayers_IsActive(void);

/* Moves value toward target by step, without overshooting; jumps straight
 * there when SavedPlayers_IsActive says so. While it moves, gScreenClearTime is set
 * to 30. */
f32 CCameraPool__StepToward(CCameraPool *pool, f32 value, f32 target, f32 step) {
    if (SavedPlayers_IsActive() != 0) {
        value = target;
    }
    if (value < target) {
        value += step;
        gScreenClearTime = 30.0f;
        if (value > target) {
            value = target;
        }
    } else if (value > target) {
        value -= step;
        gScreenClearTime = 30.0f;
        if (value < target) {
            value = target;
        }
    }
    return value;
}
