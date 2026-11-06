#include "common.h"
#include "structs/light.h"

static __inline__ void clamp255(f32 *v) {
    if (*v < 0.0f) {
        *v = 0.0f;
    } else if (*v > 255.0f) {
        *v = 255.0f;
    }
}

void ScreenTint_UpdateMode(void) {
    f32 alpha;
    f32 fade;

    clamp255(&gBlendState_m_Color[0]);
    clamp255(&gBlendState_m_Color[1]);
    clamp255(&gBlendState_m_Color[2]);
    clamp255(&gBlendState_m_Color[3]);
    clamp255(&gBlendState_m_Color[4]);

    fade = gBlendState_m_Color[3];
    alpha = gBlendState_m_Color[4];
    if (alpha == 255.0f && fade == 0.0f) {
        gBlendState = 0;
    } else if (alpha != 255.0f && fade == 0.0f) {
        gBlendState = 1;
        gBlendState_m_Color[3] = gBlendState_m_Color[4];
    } else if (alpha == 255.0f && fade != 0.0f) {
        gBlendState = 3;
    } else if (alpha != 255.0f && fade != 0.0f) {
        gBlendState = 6;
    }
}
