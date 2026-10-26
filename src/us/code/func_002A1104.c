#include "common.h"

/* Round a float to the nearest s16, saturating at the s16 range. */
s16 func_002A1104(f32 x) {
    if (x >= 0.0f) {
        x += 0.5f;
        if (x > 32767.0f) {
            x = 32767.0f;
        }
    } else {
        x -= 0.5f;
        if (x < -32768.0f) {
            x = -32768.0f;
        }
    }
    return x;
}
