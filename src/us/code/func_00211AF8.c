#include "common.h"
#include "globals/engine.h"


s32 Timer_Countdown(f32 *timer) {
    f32 t = *timer - gFrameIncrement;
    *timer = t;
    if (t <= 0.0f) {
        *timer = 0.0f;
        return 1;
    }
    return 0;
}
