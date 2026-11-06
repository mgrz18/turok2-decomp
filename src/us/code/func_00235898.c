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

    clamp255(&gBlendState.m_Color.x);
    clamp255(&gBlendState.m_Color.y);
    clamp255(&gBlendState.m_Color.z);
    clamp255(&gBlendFade[0]);
    clamp255(&gBlendFade[1]);

    fade = gBlendFade[0];
    alpha = gBlendFade[1];
    if (alpha == 255.0f && fade == 0.0f) {
        gBlendState.m_Mode = 0;
    } else if (alpha != 255.0f && fade == 0.0f) {
        gBlendState.m_Mode = 1;
        gBlendFade[0] = gBlendFade[1];
    } else if (alpha == 255.0f && fade != 0.0f) {
        gBlendState.m_Mode = 3;
    } else if (alpha != 255.0f && fade != 0.0f) {
        gBlendState.m_Mode = 6;
    }
}
