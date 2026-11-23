#include "common.h"

/* De-faked in round 21 (j4): the draft loaded the value in two identical arms under an uninitialised
 * test; the plain clamp matches. */

/* Clamps *value to 0..255. */
void Color_ClampByte(f32 *value) {
    f32 v = *value;

    if (v < 0.0f) {
        *value = 0.0f;
    } else if (v > 255.0f) {
        *value = 255.0f;
    }
}
